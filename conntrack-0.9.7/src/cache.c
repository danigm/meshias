/*
 * (C) 2006-2007 by Pablo Neira Ayuso <pablo@netfilter.org>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "cache.h"
#include "jhash.h"
#include "hash.h"
#include "log.h"
#include "us-conntrack.h"
#include "conntrackd.h"

#include <libnetfilter_conntrack/libnetfilter_conntrack.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static uint32_t __hash4(const struct nf_conntrack *ct, struct hashtable *table)
{
	unsigned int a, b;

	a = jhash(nfct_get_attr(ct, ATTR_ORIG_IPV4_SRC), sizeof(uint32_t),
		  ((nfct_get_attr_u8(ct, ATTR_ORIG_L3PROTO) << 16) |
		   (nfct_get_attr_u8(ct, ATTR_ORIG_L4PROTO))));

	b = jhash(nfct_get_attr(ct, ATTR_ORIG_IPV4_DST), sizeof(uint32_t),
		  ((nfct_get_attr_u16(ct, ATTR_ORIG_PORT_SRC) << 16) |
		   (nfct_get_attr_u16(ct, ATTR_ORIG_PORT_DST))));

	/*
	 * Instead of returning hash % table->hashsize (implying a divide)
	 * we return the high 32 bits of the (hash * table->hashsize) that will
	 * give results between [0 and hashsize-1] and same hash distribution,
	 * but using a multiply, less expensive than a divide. See:
	 * http://www.mail-archive.com/netdev@vger.kernel.org/msg56623.html
	 */
	return ((uint64_t)jhash_2words(a, b, 0) * table->hashsize) >> 32;
}

static uint32_t __hash6(const struct nf_conntrack *ct, struct hashtable *table)
{
	unsigned int a, b;

	a = jhash(nfct_get_attr(ct, ATTR_ORIG_IPV6_SRC), sizeof(uint32_t)*4,
		  ((nfct_get_attr_u8(ct, ATTR_ORIG_L3PROTO) << 16) |
		   (nfct_get_attr_u8(ct, ATTR_ORIG_L4PROTO))));

	b = jhash(nfct_get_attr(ct, ATTR_ORIG_IPV6_DST), sizeof(uint32_t)*4,
		  ((nfct_get_attr_u16(ct, ATTR_ORIG_PORT_SRC) << 16) |
		   (nfct_get_attr_u16(ct, ATTR_ORIG_PORT_DST))));

	return ((uint64_t)jhash_2words(a, b, 0) * table->hashsize) >> 32;
}

static uint32_t hash(const void *data, struct hashtable *table)
{
	int ret = 0;
	const struct us_conntrack *u = data;

	switch(nfct_get_attr_u8(u->ct, ATTR_L3PROTO)) {
		case AF_INET:
			ret = __hash4(u->ct, table);
			break;
		case AF_INET6:
			if (!nfct_attr_is_set(u->ct, ATTR_ORIG_IPV6_SRC) ||
			    !nfct_attr_is_set(u->ct, ATTR_ORIG_IPV6_DST)) {
				dlog(LOG_ERR, "missing IPv6 address. "
					      "You forgot to load "
					      "nf_conntrack_ipv6?");
				return 0;
			}

			ret = __hash6(u->ct, table);
			break;
		default:
			dlog(LOG_ERR, "unknown layer 3 proto in hash");
			break;
	}

	return ret;
}

static int compare(const void *data1, const void *data2)
{
	const struct us_conntrack *u1 = data1;
	const struct us_conntrack *u2 = data2;

	return nfct_cmp(u1->ct, u2->ct, NFCT_CMP_ORIG | NFCT_CMP_REPL);
}

struct cache_feature *cache_feature[CACHE_MAX_FEATURE] = {
	[TIMER_FEATURE]		= &timer_feature,
	[LIFETIME_FEATURE]	= &lifetime_feature,
	[WRITE_THROUGH_FEATURE] = &writethrough_feature,
};

struct cache *cache_create(const char *name, 
			   unsigned int features, 
			   struct cache_extra *extra)
{
	size_t size = sizeof(struct us_conntrack);
	int i, j = 0;
	struct cache *c;
	struct cache_feature *feature_array[CACHE_MAX_FEATURE] = {};
	unsigned int feature_offset[CACHE_MAX_FEATURE] = {};
	unsigned int feature_type[CACHE_MAX_FEATURE] = {};

	c = malloc(sizeof(struct cache));
	if (!c)
		return NULL;
	memset(c, 0, sizeof(struct cache));

	strcpy(c->name, name);

	for (i = 0; i < CACHE_MAX_FEATURE; i++) {
		if ((1 << i) & features) {
			feature_array[j] = cache_feature[i];
			feature_offset[j] = size;
			feature_type[i] = j;
			size += cache_feature[i]->size;
			j++;
		}
	}

	memcpy(c->feature_type, feature_type, sizeof(feature_type));

	c->features = malloc(sizeof(struct cache_feature) * j);
	if (!c->features) {
		free(c);
		return NULL;
	}
	memcpy(c->features, feature_array, sizeof(struct cache_feature) * j);
	c->num_features = j;

	c->extra_offset = size;
	c->extra = extra;
	if (extra)
		size += extra->size;

	c->feature_offset = malloc(sizeof(unsigned int) * j);
	if (!c->feature_offset) {
		free(c->features);
		free(c);
		return NULL;
	}
	memcpy(c->feature_offset, feature_offset, sizeof(unsigned int) * j);

	c->h = hashtable_create(CONFIG(hashsize),
				CONFIG(limit),
				size,
				hash,
				compare);

	if (!c->h) {
		free(c->features);
		free(c->feature_offset);
		free(c);
		return NULL;
	}

	return c;
}

void cache_destroy(struct cache *c)
{
	cache_flush(c);
	hashtable_destroy(c->h);
	free(c->features);
	free(c->feature_offset);
	free(c);
}

static void __del_timeout(struct alarm_block *a, void *data);

static struct us_conntrack *__add(struct cache *c, struct nf_conntrack *ct)
{
	unsigned i;
	size_t size = c->h->datasize;
	char buf[size];
	struct us_conntrack *u = (struct us_conntrack *) buf;
	struct nf_conntrack *newct;

	memset(u, 0, size);

	u->cache = c;
	if ((u->ct = newct = nfct_new()) == NULL) {
		errno = ENOMEM;
		return 0;
	}
	memcpy(u->ct, ct, nfct_sizeof(ct));

	u = hashtable_add(c->h, u);
	if (u) {
		char *data = u->data;

		init_alarm(&u->alarm, u, __del_timeout);

        	for (i = 0; i < c->num_features; i++) {
			c->features[i]->add(u, data);
			data += c->features[i]->size;
		}

		if (c->extra && c->extra->add)
			c->extra->add(u, ((char *) u) + c->extra_offset);

		return u;
	}
	free(newct);

	return NULL;
}

struct us_conntrack *cache_add(struct cache *c, struct nf_conntrack *ct)
{
	struct us_conntrack *u;

	u = __add(c, ct);
	if (u) {
		c->add_ok++;
		return u;
	}
	if (errno != EEXIST)
		c->add_fail++;

	return NULL;
}

static struct us_conntrack *__update(struct cache *c, struct nf_conntrack *ct)
{
	size_t size = c->h->datasize;
	char buf[size];
	struct us_conntrack *u = (struct us_conntrack *) buf;

	u->ct = ct;

	u = (struct us_conntrack *) hashtable_test(c->h, u);
	if (u) {
		unsigned i;
		char *data = u->data;

		nfct_copy(u->ct, ct, NFCT_CP_META);

		for (i = 0; i < c->num_features; i++) {
			c->features[i]->update(u, data);
			data += c->features[i]->size;
		}

		if (c->extra && c->extra->update)
			c->extra->update(u, ((char *) u) + c->extra_offset);

		return u;
	} 
	return NULL;
}

struct us_conntrack *cache_update(struct cache *c, struct nf_conntrack *ct)
{
	struct us_conntrack *u;

	u = __update(c, ct);
	if (u) {
		c->upd_ok++;
		return u;
	}
	c->upd_fail++;
	
	return NULL;
}

struct us_conntrack *cache_update_force(struct cache *c,
					struct nf_conntrack *ct)
{
	struct us_conntrack *u;

	if ((u = __update(c, ct)) != NULL) {
		c->upd_ok++;
		return u;
	}
	if ((u = __add(c, ct)) != NULL) {
		c->add_ok++;
		return u;
	}
	c->add_fail++;
	return NULL;
}

int cache_test(struct cache *c, struct nf_conntrack *ct)
{
	size_t size = c->h->datasize;
	char buf[size];
	struct us_conntrack *u = (struct us_conntrack *) buf;
	void *ret;

	u->ct = ct;

	ret = hashtable_test(c->h, u);

	return ret != NULL;
}

static void __del2(struct cache *c, struct us_conntrack *u)
{
	unsigned i;
	char *data = u->data;
	struct nf_conntrack *p = u->ct;

	for (i = 0; i < c->num_features; i++) {
		c->features[i]->destroy(u, data);
		data += c->features[i]->size;
	}

	if (c->extra && c->extra->destroy)
		c->extra->destroy(u, ((char *) u) + c->extra_offset);

	hashtable_del(c->h, u);
	free(p);
}

static int __del(struct cache *c, struct nf_conntrack *ct)
{
	size_t size = c->h->datasize;
	char buf[size];
	struct us_conntrack *u = (struct us_conntrack *) buf;

	u->ct = ct;

	u = (struct us_conntrack *) hashtable_test(c->h, u);
	if (u) {
		del_alarm(&u->alarm);
		__del2(c, u);
		return 1;
	}
	return 0;
}

int cache_del(struct cache *c, struct nf_conntrack *ct)
{
	if (__del(c, ct)) {
		c->del_ok++;
		return 1;
	}
	c->del_fail++;

	return 0;
}

static void __del_timeout(struct alarm_block *a, void *data)
{
	struct us_conntrack *u = (struct us_conntrack *) data;

	__del2(u->cache, u);
}

struct us_conntrack *
cache_del_timeout(struct cache *c, struct nf_conntrack *ct, int timeout)
{
	size_t size = c->h->datasize;
	char buf[size];
	struct us_conntrack *u = (struct us_conntrack *) buf;

	if (timeout <= 0)
		cache_del(c, ct);

	u->ct = ct;

	u = (struct us_conntrack *) hashtable_test(c->h, u);
	if (u) {
		if (!alarm_pending(&u->alarm)) {
			add_alarm(&u->alarm, timeout, 0);
			/*
			 * increase stats even if this entry was not really
			 * removed yet. We do not want to make people think
			 * that the replication protocol does not work 
			 * properly.
			 */
			c->del_ok++;
			return u;
		}
	}
	return NULL;
}

struct us_conntrack *cache_get_conntrack(struct cache *c, void *data)
{
	return (struct us_conntrack *)((char*)data - c->extra_offset);
}

void *cache_get_extra(struct cache *c, void *data)
{
	return (char*)data + c->extra_offset;
}

void cache_stats(const struct cache *c, int fd)
{
	char buf[512];
	int size;

	size = sprintf(buf, "cache %s:\n"
			    "current active connections:\t%12u\n"
			    "connections created:\t\t%12u\tfailed:\t%12u\n"
			    "connections updated:\t\t%12u\tfailed:\t%12u\n"
			    "connections destroyed:\t\t%12u\tfailed:\t%12u\n\n",
			    			 c->name,
			    			 hashtable_counter(c->h),
			    			 c->add_ok, 
			    			 c->add_fail,
						 c->upd_ok,
						 c->upd_fail,
						 c->del_ok,
						 c->del_fail);
	send(fd, buf, size, 0);
}

void cache_iterate(struct cache *c, 
		   void *data, 
		   int (*iterate)(void *data1, void *data2))
{
	hashtable_iterate(c->h, data, iterate);
}

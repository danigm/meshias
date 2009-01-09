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

#include <string.h>
#include <libnetfilter_conntrack/libnetfilter_conntrack.h>
#include "network.h"

static void addattr(struct netpld *pld, int attr, const void *data, size_t len)
{
	struct netattr *nta;
	int tlen = NTA_LENGTH(len);

	nta = PLD_TAIL(pld);
	nta->nta_attr = htons(attr);
	nta->nta_len = htons(len);
	memcpy(NTA_DATA(nta), data, len);
	pld->len += NTA_ALIGN(tlen);
}

static void __build_u8(const struct nf_conntrack *ct,
		       struct netpld *pld,
		       int attr)
{
	uint8_t data = nfct_get_attr_u8(ct, attr);
	addattr(pld, attr, &data, sizeof(uint8_t));
}

static void __build_u16(const struct nf_conntrack *ct,
			struct netpld *pld,
			int attr)
{
	uint16_t data = nfct_get_attr_u16(ct, attr);
	data = htons(data);
	addattr(pld, attr, &data, sizeof(uint16_t));
}

static void __build_u32(const struct nf_conntrack *ct, 
			struct netpld *pld,
			int attr)
{
	uint32_t data = nfct_get_attr_u32(ct, attr);
	data = htonl(data);
	addattr(pld, attr, &data, sizeof(uint32_t));
}

static void __build_pointer_be(const struct nf_conntrack *ct, 
			       struct netpld *pld,
			       int attr,
			       size_t size)
{
	addattr(pld, attr, nfct_get_attr(ct, attr), size);
}

static void __nat_build_u32(uint32_t data, struct netpld *pld, int attr)
{
	data = htonl(data);
	addattr(pld, attr, &data, sizeof(uint32_t));
}

static void __nat_build_u16(uint16_t data, struct netpld *pld, int attr)
{
	data = htons(data);
	addattr(pld, attr, &data, sizeof(uint16_t));
}

/* XXX: ICMP not supported */
void build_netpld(struct nf_conntrack *ct, struct netpld *pld, int query)
{
	if (nfct_attr_is_set(ct, ATTR_IPV4_SRC))
		__build_pointer_be(ct, pld, ATTR_IPV4_SRC, sizeof(uint32_t));
	if (nfct_attr_is_set(ct, ATTR_IPV4_DST))
		__build_pointer_be(ct, pld, ATTR_IPV4_DST, sizeof(uint32_t));
	if (nfct_attr_is_set(ct, ATTR_IPV6_SRC))
		__build_pointer_be(ct, pld, ATTR_IPV6_SRC, sizeof(uint32_t)*4);
	if (nfct_attr_is_set(ct, ATTR_IPV6_DST))
		__build_pointer_be(ct, pld, ATTR_IPV6_DST, sizeof(uint32_t)*4);
	if (nfct_attr_is_set(ct, ATTR_L3PROTO))
		__build_u8(ct, pld, ATTR_L3PROTO);
	if (nfct_attr_is_set(ct, ATTR_PORT_SRC))
		__build_u16(ct, pld, ATTR_PORT_SRC);
	if (nfct_attr_is_set(ct, ATTR_PORT_DST))
		__build_u16(ct, pld, ATTR_PORT_DST);
	if (nfct_attr_is_set(ct, ATTR_L4PROTO)) {
		uint8_t proto;

		__build_u8(ct, pld, ATTR_L4PROTO);
		proto = nfct_get_attr_u8(ct, ATTR_L4PROTO);
		if (proto == IPPROTO_TCP) {
			if (nfct_attr_is_set(ct, ATTR_TCP_STATE))
				__build_u8(ct, pld, ATTR_TCP_STATE);
		}
	}
	if (nfct_attr_is_set(ct, ATTR_TIMEOUT))
		__build_u32(ct, pld, ATTR_TIMEOUT);
	if (nfct_attr_is_set(ct, ATTR_MARK))
		__build_u32(ct, pld, ATTR_MARK);
	if (nfct_attr_is_set(ct, ATTR_STATUS))
		__build_u32(ct, pld, ATTR_STATUS);

	/* setup the master conntrack */
	if (nfct_attr_is_set(ct, ATTR_MASTER_IPV4_SRC))
		__build_u32(ct, pld, ATTR_MASTER_IPV4_SRC);
	if (nfct_attr_is_set(ct, ATTR_MASTER_IPV4_DST))
		__build_u32(ct, pld, ATTR_MASTER_IPV4_DST);
	if (nfct_attr_is_set(ct, ATTR_MASTER_L3PROTO))
		__build_u8(ct, pld, ATTR_MASTER_L3PROTO);
	if (nfct_attr_is_set(ct, ATTR_MASTER_PORT_SRC))
		__build_u16(ct, pld, ATTR_MASTER_PORT_SRC);
	if (nfct_attr_is_set(ct, ATTR_MASTER_PORT_DST))
		__build_u16(ct, pld, ATTR_MASTER_PORT_DST);
	if (nfct_attr_is_set(ct, ATTR_MASTER_L4PROTO))
		__build_u8(ct, pld, ATTR_MASTER_L4PROTO);

	/*  NAT */
	if (nfct_getobjopt(ct, NFCT_GOPT_IS_SNAT)) {
		uint32_t data = nfct_get_attr_u32(ct, ATTR_REPL_IPV4_DST);
		__nat_build_u32(data, pld, ATTR_SNAT_IPV4);
	}
	if (nfct_getobjopt(ct, NFCT_GOPT_IS_DNAT)) {
		uint32_t data = nfct_get_attr_u32(ct, ATTR_REPL_IPV4_SRC);
		__nat_build_u32(data, pld, ATTR_DNAT_IPV4);
	}
	if (nfct_getobjopt(ct, NFCT_GOPT_IS_SPAT)) {
		uint16_t data = nfct_get_attr_u16(ct, ATTR_REPL_PORT_DST);
		__nat_build_u16(data, pld, ATTR_SNAT_PORT);
	}
	if (nfct_getobjopt(ct, NFCT_GOPT_IS_DPAT)) {
		uint16_t data = nfct_get_attr_u16(ct, ATTR_REPL_PORT_SRC);
		__nat_build_u16(data, pld, ATTR_DNAT_PORT);
	}

	/* NAT sequence adjustment */
	if (nfct_attr_is_set(ct, ATTR_ORIG_NAT_SEQ_CORRECTION_POS))
		__build_u32(ct, pld, ATTR_ORIG_NAT_SEQ_CORRECTION_POS);
	if (nfct_attr_is_set(ct, ATTR_ORIG_NAT_SEQ_OFFSET_BEFORE))
		__build_u32(ct, pld, ATTR_ORIG_NAT_SEQ_OFFSET_BEFORE);
	if (nfct_attr_is_set(ct, ATTR_ORIG_NAT_SEQ_OFFSET_AFTER))
		__build_u32(ct, pld, ATTR_ORIG_NAT_SEQ_OFFSET_AFTER);
	if (nfct_attr_is_set(ct, ATTR_REPL_NAT_SEQ_CORRECTION_POS))
		__build_u32(ct, pld, ATTR_REPL_NAT_SEQ_CORRECTION_POS);
	if (nfct_attr_is_set(ct, ATTR_REPL_NAT_SEQ_OFFSET_BEFORE))
		__build_u32(ct, pld, ATTR_REPL_NAT_SEQ_OFFSET_BEFORE);
	if (nfct_attr_is_set(ct, ATTR_REPL_NAT_SEQ_OFFSET_AFTER))
		__build_u32(ct, pld, ATTR_REPL_NAT_SEQ_OFFSET_AFTER);

	pld->query = query;

	PLD_HOST2NETWORK(pld);
}

%{
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
 *
 * Description: configuration file abstract grammar
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "conntrackd.h"
#include "ignore.h"
#include <syslog.h>
#include <libnetfilter_conntrack/libnetfilter_conntrack_tcp.h>

extern struct state_replication_helper tcp_state_helper;

extern char *yytext;
extern int   yylineno;

struct ct_conf conf;
%}

%union {
	int		val;
	char		*string;
}

%token T_IPV4_ADDR T_IPV4_IFACE T_PORT T_HASHSIZE T_HASHLIMIT T_MULTICAST
%token T_PATH T_UNIX T_REFRESH T_IPV6_ADDR T_IPV6_IFACE
%token T_IGNORE_UDP T_IGNORE_ICMP T_IGNORE_TRAFFIC T_BACKLOG T_GROUP
%token T_LOG T_UDP T_ICMP T_IGMP T_VRRP T_TCP T_IGNORE_PROTOCOL
%token T_LOCK T_STRIP_NAT T_BUFFER_SIZE_MAX_GROWN T_EXPIRE T_TIMEOUT
%token T_GENERAL T_SYNC T_STATS T_RELAX_TRANSITIONS T_BUFFER_SIZE T_DELAY
%token T_SYNC_MODE T_LISTEN_TO T_FAMILY T_RESEND_BUFFER_SIZE
%token T_ALARM T_FTFW T_CHECKSUM T_WINDOWSIZE T_ON T_OFF
%token T_REPLICATE T_FOR T_IFACE 
%token T_ESTABLISHED T_SYN_SENT T_SYN_RECV T_FIN_WAIT 
%token T_CLOSE_WAIT T_LAST_ACK T_TIME_WAIT T_CLOSE T_LISTEN
%token T_SYSLOG T_WRITE_THROUGH T_STAT_BUFFER_SIZE T_DESTROY_TIMEOUT
%token T_MCAST_RCVBUFF T_MCAST_SNDBUFF T_NOTRACK

%token <string> T_IP T_PATH_VAL
%token <val> T_NUMBER
%token <string> T_STRING

%%

configfile :
	   | lines
	   ;

lines : line
      | lines line
      ;

line : ignore_protocol
     | ignore_traffic
     | strip_nat
     | general
     | sync
     | stats
     ;

logfile_bool : T_LOG T_ON
{
	strncpy(conf.logfile, DEFAULT_LOGFILE, FILENAME_MAXLEN);
};

logfile_bool : T_LOG T_OFF
{
};

logfile_path : T_LOG T_PATH_VAL
{
	strncpy(conf.logfile, $2, FILENAME_MAXLEN);
};

syslog_bool : T_SYSLOG T_ON
{
	conf.syslog_facility = DEFAULT_SYSLOG_FACILITY;
};

syslog_bool : T_SYSLOG T_OFF
{
	conf.syslog_facility = -1;
}

syslog_facility : T_SYSLOG T_STRING
{
	if (!strcmp($2, "daemon"))
		conf.syslog_facility = LOG_DAEMON;
	else if (!strcmp($2, "local0"))
		conf.syslog_facility = LOG_LOCAL0;
	else if (!strcmp($2, "local1"))
		conf.syslog_facility = LOG_LOCAL1;
	else if (!strcmp($2, "local2"))
		conf.syslog_facility = LOG_LOCAL2;
	else if (!strcmp($2, "local3"))
		conf.syslog_facility = LOG_LOCAL3;
	else if (!strcmp($2, "local4"))
		conf.syslog_facility = LOG_LOCAL4;
	else if (!strcmp($2, "local5"))
		conf.syslog_facility = LOG_LOCAL5;
	else if (!strcmp($2, "local6"))
		conf.syslog_facility = LOG_LOCAL6;
	else if (!strcmp($2, "local7"))
		conf.syslog_facility = LOG_LOCAL7;
	else {
		fprintf(stderr, "'%s' is not a known syslog facility, "
				"ignoring.\n", $2);
		break;
	}

	if (conf.stats.syslog_facility != -1 &&
	    conf.syslog_facility != conf.stats.syslog_facility)
	    	fprintf(stderr, "WARNING: Conflicting Syslog facility "
				"values, defaulting to General.\n");
};

lock : T_LOCK T_PATH_VAL
{
	strncpy(conf.lockfile, $2, FILENAME_MAXLEN);
};

strip_nat: T_STRIP_NAT
{
	fprintf(stderr, "Notice: StripNAT clause is obsolete. "
			"Please, remove it from conntrackd.conf\n");
};

refreshtime : T_REFRESH T_NUMBER
{
	conf.refresh = $2;
};

expiretime: T_EXPIRE T_NUMBER
{
	conf.cache_timeout = $2;
};

timeout: T_TIMEOUT T_NUMBER
{
	conf.commit_timeout = $2;
};

checksum: T_CHECKSUM T_ON 
{
	conf.mcast.checksum = 0;
};

checksum: T_CHECKSUM T_OFF
{
	conf.mcast.checksum = 1;
};

ignore_traffic : T_IGNORE_TRAFFIC '{' ignore_traffic_options '}';

ignore_traffic_options :
		       | ignore_traffic_options ignore_traffic_option;

ignore_traffic_option : T_IPV4_ADDR T_IP
{
	union inet_address ip;

	memset(&ip, 0, sizeof(union inet_address));

	if (!inet_aton($2, &ip.ipv4)) {
		fprintf(stderr, "%s is not a valid IPv4, ignoring", $2);
		break;
	}

	if (!STATE(ignore_pool)) {
		STATE(ignore_pool) = ignore_pool_create();
		if (!STATE(ignore_pool)) {
			fprintf(stderr, "Can't create ignore pool!\n");
			exit(EXIT_FAILURE);
		}
	}

	if (!ignore_pool_add(STATE(ignore_pool), &ip, AF_INET)) {
		if (errno == EEXIST)
			fprintf(stderr, "IP %s is repeated "
					"in the ignore pool\n", $2);
		if (errno == ENOSPC)
			fprintf(stderr, "Too many IP in the ignore pool!\n");
	}
};

ignore_traffic_option : T_IPV6_ADDR T_IP
{
	union inet_address ip;

	memset(&ip, 0, sizeof(union inet_address));

#ifdef HAVE_INET_PTON_IPV6
	if (inet_pton(AF_INET6, $2, &ip.ipv6) <= 0) {
		fprintf(stderr, "%s is not a valid IPv6, ignoring", $2);
		break;
	}
#else
	fprintf(stderr, "Cannot find inet_pton(), IPv6 unsupported!");
	break;
#endif

	if (!STATE(ignore_pool)) {
		STATE(ignore_pool) = ignore_pool_create();
		if (!STATE(ignore_pool)) {
			fprintf(stderr, "Can't create ignore pool!\n");
			exit(EXIT_FAILURE);
		}
	}

	if (!ignore_pool_add(STATE(ignore_pool), &ip, AF_INET6)) {
		if (errno == EEXIST)
			fprintf(stderr, "IP %s is repeated "
					"in the ignore pool\n", $2);
		if (errno == ENOSPC)
			fprintf(stderr, "Too many IP in the ignore pool!\n");
	}

};

multicast_line : T_MULTICAST '{' multicast_options '}';

multicast_options :
		  | multicast_options multicast_option;

multicast_option : T_IPV4_ADDR T_IP
{
	if (!inet_aton($2, &conf.mcast.in)) {
		fprintf(stderr, "%s is not a valid IPv4 address\n", $2);
		break;
	}

        if (conf.mcast.ipproto == AF_INET6) {
		fprintf(stderr, "Your multicast address is IPv4 but "
		                "is binded to an IPv6 interface? Surely "
				"this is not what you want\n");
		break;
	}

	conf.mcast.ipproto = AF_INET;
};

multicast_option : T_IPV6_ADDR T_IP
{
#ifdef HAVE_INET_PTON_IPV6
	if (inet_pton(AF_INET6, $2, &conf.mcast.in) <= 0) {
		fprintf(stderr, "%s is not a valid IPv6 address\n", $2);
		break;
	}
#else
	fprintf(stderr, "Cannot find inet_pton(), IPv6 unsupported!");
	break;
#endif

	if (conf.mcast.ipproto == AF_INET) {
		fprintf(stderr, "Your multicast address is IPv6 but "
				"is binded to an IPv4 interface? Surely "
				"this is not what you want\n");
		break;
	}

	conf.mcast.ipproto = AF_INET6;

	if (conf.mcast.iface[0] && !conf.mcast.ifa.interface_index6) {
		unsigned int idx;

		idx = if_nametoindex($2);
		if (!idx) {
			fprintf(stderr, "%s is an invalid interface.\n", $2);
			break;
		}

		conf.mcast.ifa.interface_index6 = idx;
		conf.mcast.ipproto = AF_INET6;
	}
};

multicast_option : T_IPV4_IFACE T_IP
{
	if (!inet_aton($2, &conf.mcast.ifa)) {
		fprintf(stderr, "%s is not a valid IPv4 address\n", $2);
		break;
	}

        if (conf.mcast.ipproto == AF_INET6) {
		fprintf(stderr, "Your multicast interface is IPv4 but "
		                "is binded to an IPv6 interface? Surely "
				"this is not what you want\n");
		break;
	}

	conf.mcast.ipproto = AF_INET;
};

multicast_option : T_IPV6_IFACE T_IP
{
	fprintf(stderr, "IPv6_interface not required for IPv6, ignoring.\n");
}

multicast_option : T_IFACE T_STRING
{
	strncpy(conf.mcast.iface, $2, IFNAMSIZ);

	if (conf.mcast.ipproto == AF_INET6) {
		unsigned int idx;

		idx = if_nametoindex($2);
		if (!idx) {
			fprintf(stderr, "%s is an invalid interface.\n", $2);
			break;
		}

		conf.mcast.ifa.interface_index6 = idx;
		conf.mcast.ipproto = AF_INET6;
	}
};

multicast_option : T_BACKLOG T_NUMBER
{
	fprintf(stderr, "Notice: Backlog option inside Multicast clause is "
			"obsolete. Please, remove it from conntrackd.conf.\n");
};

multicast_option : T_GROUP T_NUMBER
{
	conf.mcast.port = $2;
};

multicast_option: T_MCAST_SNDBUFF T_NUMBER
{
	conf.mcast.sndbuf = $2;
};

multicast_option: T_MCAST_RCVBUFF T_NUMBER
{
	conf.mcast.rcvbuf = $2;
};

hashsize : T_HASHSIZE T_NUMBER
{
	conf.hashsize = $2;
};

hashlimit: T_HASHLIMIT T_NUMBER
{
	conf.limit = $2;
};

unix_line: T_UNIX '{' unix_options '}';

unix_options:
	    | unix_options unix_option
	    ;

unix_option : T_PATH T_PATH_VAL
{
	strcpy(conf.local.path, $2);
};

unix_option : T_BACKLOG T_NUMBER
{
	conf.local.backlog = $2;
};

ignore_protocol: T_IGNORE_PROTOCOL '{' ignore_proto_list '}';

ignore_proto_list:
		 | ignore_proto_list ignore_proto
		 ;

ignore_proto: T_NUMBER
{
	if ($1 < IPPROTO_MAX)
		conf.ignore_protocol[$1] = 1;
	else
		fprintf(stderr, "Protocol number `%d' is freak\n", $1);
};

ignore_proto: T_UDP
{
	conf.ignore_protocol[IPPROTO_UDP] = 1;
};

ignore_proto: T_ICMP
{
	conf.ignore_protocol[IPPROTO_ICMP] = 1;
};

ignore_proto: T_VRRP
{
	conf.ignore_protocol[IPPROTO_VRRP] = 1;
};

ignore_proto: T_IGMP
{
	conf.ignore_protocol[IPPROTO_IGMP] = 1;
};

sync: T_SYNC '{' sync_list '}'
{
	if (conf.flags & CTD_STATS_MODE) {
		fprintf(stderr, "ERROR: Cannot use both Stats and Sync "
				"clauses in conntrackd.conf.\n");
		exit(EXIT_FAILURE);
	}
	conf.flags |= CTD_SYNC_MODE;
};

sync_list:
	 | sync_list sync_line;

sync_line: refreshtime
	 | expiretime
	 | timeout
	 | checksum
	 | multicast_line
	 | relax_transitions
	 | delay_destroy_msgs
	 | sync_mode_alarm
	 | sync_mode_ftfw
	 | sync_mode_notrack
	 | listen_to
	 | state_replication
	 | cache_writethrough
	 | destroy_timeout
	 ;

sync_mode_alarm: T_SYNC_MODE T_ALARM '{' sync_mode_alarm_list '}'
{
	conf.flags |= CTD_SYNC_ALARM;
};

sync_mode_ftfw: T_SYNC_MODE T_FTFW '{' sync_mode_ftfw_list '}'
{
	conf.flags |= CTD_SYNC_FTFW;
};

sync_mode_notrack: T_SYNC_MODE T_NOTRACK '{' sync_mode_notrack_list '}'
{
	conf.flags |= CTD_SYNC_NOTRACK;
};

sync_mode_alarm_list:
	      | sync_mode_alarm_list sync_mode_alarm_line;

sync_mode_alarm_line: refreshtime
              		 | expiretime
	     		 | timeout
			 | relax_transitions
			 | delay_destroy_msgs
			 ;

sync_mode_ftfw_list:
	      | sync_mode_ftfw_list sync_mode_ftfw_line;

sync_mode_ftfw_line: resend_queue_size
		   | timeout
		   | window_size
		   ;

sync_mode_notrack_list:
	      | sync_mode_notrack_list sync_mode_notrack_line;

sync_mode_notrack_line: timeout
		   ;


resend_queue_size: T_RESEND_BUFFER_SIZE T_NUMBER
{
	conf.resend_queue_size = $2;
};

window_size: T_WINDOWSIZE T_NUMBER
{
	conf.window_size = $2;
};

destroy_timeout: T_DESTROY_TIMEOUT T_NUMBER
{
	conf.del_timeout = $2;
};

relax_transitions: T_RELAX_TRANSITIONS
{
	fprintf(stderr, "Notice: RelaxTransitions clause is obsolete. "
			"Please, remove it from conntrackd.conf\n");
};

delay_destroy_msgs: T_DELAY
{
	fprintf(stderr, "Notice: DelayDestroyMessages clause is obsolete. "
			"Please, remove it from conntrackd.conf\n");
};

listen_to: T_LISTEN_TO T_IP
{
	union inet_address addr;

#ifdef HAVE_INET_PTON_IPV6
	if (inet_pton(AF_INET6, $2, &addr.ipv6) <= 0)
#endif
		if (inet_aton($2, &addr.ipv4) <= 0) {
			fprintf(stderr, "%s is not a valid IP address\n", $2);
			exit(EXIT_FAILURE);
		}

	if (CONFIG(listen_to_len) == 0 || CONFIG(listen_to_len) % 16) {
		CONFIG(listen_to) = realloc(CONFIG(listen_to),
					    sizeof(union inet_address) *
					    (CONFIG(listen_to_len) + 16));
		if (CONFIG(listen_to) == NULL) {
			fprintf(stderr, "cannot init listen_to array\n");
			exit(EXIT_FAILURE);
		}

		memset(CONFIG(listen_to) + 
		       (CONFIG(listen_to_len) * sizeof(union inet_address)),
		       0, sizeof(union inet_address) * 16);

	}
};

state_replication: T_REPLICATE states T_FOR state_proto;

states:
      | states state;

state_proto: T_TCP;
state: tcp_state;

tcp_state: T_SYN_SENT
{
	state_helper_register(&tcp_state_helper, TCP_CONNTRACK_SYN_SENT);
};
tcp_state: T_SYN_RECV
{
	state_helper_register(&tcp_state_helper, TCP_CONNTRACK_SYN_RECV);
};
tcp_state: T_ESTABLISHED
{
	state_helper_register(&tcp_state_helper, TCP_CONNTRACK_ESTABLISHED);
};
tcp_state: T_FIN_WAIT
{
	state_helper_register(&tcp_state_helper, TCP_CONNTRACK_FIN_WAIT);
};
tcp_state: T_CLOSE_WAIT
{
	state_helper_register(&tcp_state_helper, TCP_CONNTRACK_CLOSE_WAIT);
};
tcp_state: T_LAST_ACK
{
	state_helper_register(&tcp_state_helper, TCP_CONNTRACK_LAST_ACK);
};
tcp_state: T_TIME_WAIT
{
	state_helper_register(&tcp_state_helper, TCP_CONNTRACK_TIME_WAIT);
};
tcp_state: T_CLOSE
{
	state_helper_register(&tcp_state_helper, TCP_CONNTRACK_CLOSE);
};
tcp_state: T_LISTEN
{
	state_helper_register(&tcp_state_helper, TCP_CONNTRACK_LISTEN);
};

cache_writethrough: T_WRITE_THROUGH T_ON
{
	conf.cache_write_through = 1;
};

cache_writethrough: T_WRITE_THROUGH T_OFF
{
	conf.cache_write_through = 0;
};

general: T_GENERAL '{' general_list '}';

general_list:
	    | general_list general_line
	    ;

general_line: hashsize
	    | hashlimit
	    | logfile_bool
	    | logfile_path
	    | syslog_facility
	    | syslog_bool
	    | lock
	    | unix_line
	    | netlink_buffer_size
	    | netlink_buffer_size_max_grown
	    | family
	    ;

netlink_buffer_size: T_BUFFER_SIZE T_NUMBER
{
	conf.netlink_buffer_size = $2;
};

netlink_buffer_size_max_grown : T_BUFFER_SIZE_MAX_GROWN T_NUMBER
{
	conf.netlink_buffer_size_max_grown = $2;
};

family : T_FAMILY T_STRING
{
	if (strncmp($2, "IPv6", strlen("IPv6")) == 0)
		conf.family = AF_INET6;
	else
		conf.family = AF_INET;
};

stats: T_STATS '{' stats_list '}'
{
	if (conf.flags & CTD_SYNC_MODE) {
		fprintf(stderr, "ERROR: Cannot use both Stats and Sync "
				"clauses in conntrackd.conf.\n");
		exit(EXIT_FAILURE);
	}
	conf.flags |= CTD_STATS_MODE;
};

stats_list:
	 | stats_list stat_line
	 ;

stat_line: stat_logfile_bool
	 | stat_logfile_path
	 | stat_syslog_bool
	 | stat_syslog_facility
	 | buffer_size
	 ;

stat_logfile_bool : T_LOG T_ON
{
	strncpy(conf.stats.logfile, DEFAULT_STATS_LOGFILE, FILENAME_MAXLEN);
};

stat_logfile_bool : T_LOG T_OFF
{
};

stat_logfile_path : T_LOG T_PATH_VAL
{
	strncpy(conf.stats.logfile, $2, FILENAME_MAXLEN);
};

stat_syslog_bool : T_SYSLOG T_ON
{
	conf.stats.syslog_facility = DEFAULT_SYSLOG_FACILITY;
};

stat_syslog_bool : T_SYSLOG T_OFF
{
	conf.stats.syslog_facility = -1;
}

stat_syslog_facility : T_SYSLOG T_STRING
{
	if (!strcmp($2, "daemon"))
		conf.stats.syslog_facility = LOG_DAEMON;
	else if (!strcmp($2, "local0"))
		conf.stats.syslog_facility = LOG_LOCAL0;
	else if (!strcmp($2, "local1"))
		conf.stats.syslog_facility = LOG_LOCAL1;
	else if (!strcmp($2, "local2"))
		conf.stats.syslog_facility = LOG_LOCAL2;
	else if (!strcmp($2, "local3"))
		conf.stats.syslog_facility = LOG_LOCAL3;
	else if (!strcmp($2, "local4"))
		conf.stats.syslog_facility = LOG_LOCAL4;
	else if (!strcmp($2, "local5"))
		conf.stats.syslog_facility = LOG_LOCAL5;
	else if (!strcmp($2, "local6"))
		conf.stats.syslog_facility = LOG_LOCAL6;
	else if (!strcmp($2, "local7"))
		conf.stats.syslog_facility = LOG_LOCAL7;
	else {
		fprintf(stderr, "'%s' is not a known syslog facility, "
				"ignoring.\n", $2);
		break;
	}

	if (conf.syslog_facility != -1 &&
	    conf.stats.syslog_facility != conf.syslog_facility)
		fprintf(stderr, "WARNING: Conflicting Syslog facility "
				"values, defaulting to General.\n");
};

buffer_size: T_STAT_BUFFER_SIZE T_NUMBER
{
	fprintf(stderr, "WARNING: LogFileBufferSize is deprecated.\n");
};

%%

int __attribute__((noreturn))
yyerror(char *msg)
{
	fprintf(stderr, "Error parsing config file: ");
	fprintf(stderr, "line (%d), symbol '%s': %s\n", yylineno, yytext, msg);
	exit(EXIT_FAILURE);
}

int
init_config(char *filename)
{
	FILE *fp;

	fp = fopen(filename, "r");
	if (!fp)
		return -1;

	/* Zero may be a valid facility */
	CONFIG(syslog_facility) = -1;
	CONFIG(stats).syslog_facility = -1;

	yyrestart(fp);
	yyparse();
	fclose(fp);

	/* default to IPv4 */
	if (CONFIG(family) == 0)
		CONFIG(family) = AF_INET;

	/* set to default is not specified */
	if (strcmp(CONFIG(lockfile), "") == 0)
		strncpy(CONFIG(lockfile), DEFAULT_LOCKFILE, FILENAME_MAXLEN);

	/* default to 180 seconds of expiration time: cache entries */
	if (CONFIG(cache_timeout) == 0)
		CONFIG(cache_timeout) = 180;

	/* default to 180 seconds: committed entries */
	if (CONFIG(commit_timeout) == 0)
		CONFIG(commit_timeout) = 180;

	/* default to 60 seconds of refresh time */
	if (CONFIG(refresh) == 0)
		CONFIG(refresh) = 60;

	if (CONFIG(resend_queue_size) == 0)
		CONFIG(resend_queue_size) = 262144;

	/* create empty pool */
	if (!STATE(ignore_pool)) {
		STATE(ignore_pool) = ignore_pool_create();
		if (!STATE(ignore_pool)) {
			fprintf(stderr, "Can't create ignore pool!\n");
			exit(EXIT_FAILURE);
		}
	}

	/* default to a window size of 20 packets */
	if (CONFIG(window_size) == 0)
		CONFIG(window_size) = 20;

	/* double of 120 seconds which is common timeout of a final state */
	if (conf.flags & CTD_SYNC_FTFW && CONFIG(del_timeout) == 0)
		CONFIG(del_timeout) = 240;

	return 0;
}

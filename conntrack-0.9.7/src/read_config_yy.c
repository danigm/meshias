/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_IPV4_ADDR = 258,
     T_IPV4_IFACE = 259,
     T_PORT = 260,
     T_HASHSIZE = 261,
     T_HASHLIMIT = 262,
     T_MULTICAST = 263,
     T_PATH = 264,
     T_UNIX = 265,
     T_REFRESH = 266,
     T_IPV6_ADDR = 267,
     T_IPV6_IFACE = 268,
     T_IGNORE_UDP = 269,
     T_IGNORE_ICMP = 270,
     T_IGNORE_TRAFFIC = 271,
     T_BACKLOG = 272,
     T_GROUP = 273,
     T_LOG = 274,
     T_UDP = 275,
     T_ICMP = 276,
     T_IGMP = 277,
     T_VRRP = 278,
     T_TCP = 279,
     T_IGNORE_PROTOCOL = 280,
     T_LOCK = 281,
     T_STRIP_NAT = 282,
     T_BUFFER_SIZE_MAX_GROWN = 283,
     T_EXPIRE = 284,
     T_TIMEOUT = 285,
     T_GENERAL = 286,
     T_SYNC = 287,
     T_STATS = 288,
     T_RELAX_TRANSITIONS = 289,
     T_BUFFER_SIZE = 290,
     T_DELAY = 291,
     T_SYNC_MODE = 292,
     T_LISTEN_TO = 293,
     T_FAMILY = 294,
     T_RESEND_BUFFER_SIZE = 295,
     T_ALARM = 296,
     T_FTFW = 297,
     T_CHECKSUM = 298,
     T_WINDOWSIZE = 299,
     T_ON = 300,
     T_OFF = 301,
     T_REPLICATE = 302,
     T_FOR = 303,
     T_IFACE = 304,
     T_ESTABLISHED = 305,
     T_SYN_SENT = 306,
     T_SYN_RECV = 307,
     T_FIN_WAIT = 308,
     T_CLOSE_WAIT = 309,
     T_LAST_ACK = 310,
     T_TIME_WAIT = 311,
     T_CLOSE = 312,
     T_LISTEN = 313,
     T_SYSLOG = 314,
     T_WRITE_THROUGH = 315,
     T_STAT_BUFFER_SIZE = 316,
     T_DESTROY_TIMEOUT = 317,
     T_MCAST_RCVBUFF = 318,
     T_MCAST_SNDBUFF = 319,
     T_NOTRACK = 320,
     T_IP = 321,
     T_PATH_VAL = 322,
     T_NUMBER = 323,
     T_STRING = 324
   };
#endif
/* Tokens.  */
#define T_IPV4_ADDR 258
#define T_IPV4_IFACE 259
#define T_PORT 260
#define T_HASHSIZE 261
#define T_HASHLIMIT 262
#define T_MULTICAST 263
#define T_PATH 264
#define T_UNIX 265
#define T_REFRESH 266
#define T_IPV6_ADDR 267
#define T_IPV6_IFACE 268
#define T_IGNORE_UDP 269
#define T_IGNORE_ICMP 270
#define T_IGNORE_TRAFFIC 271
#define T_BACKLOG 272
#define T_GROUP 273
#define T_LOG 274
#define T_UDP 275
#define T_ICMP 276
#define T_IGMP 277
#define T_VRRP 278
#define T_TCP 279
#define T_IGNORE_PROTOCOL 280
#define T_LOCK 281
#define T_STRIP_NAT 282
#define T_BUFFER_SIZE_MAX_GROWN 283
#define T_EXPIRE 284
#define T_TIMEOUT 285
#define T_GENERAL 286
#define T_SYNC 287
#define T_STATS 288
#define T_RELAX_TRANSITIONS 289
#define T_BUFFER_SIZE 290
#define T_DELAY 291
#define T_SYNC_MODE 292
#define T_LISTEN_TO 293
#define T_FAMILY 294
#define T_RESEND_BUFFER_SIZE 295
#define T_ALARM 296
#define T_FTFW 297
#define T_CHECKSUM 298
#define T_WINDOWSIZE 299
#define T_ON 300
#define T_OFF 301
#define T_REPLICATE 302
#define T_FOR 303
#define T_IFACE 304
#define T_ESTABLISHED 305
#define T_SYN_SENT 306
#define T_SYN_RECV 307
#define T_FIN_WAIT 308
#define T_CLOSE_WAIT 309
#define T_LAST_ACK 310
#define T_TIME_WAIT 311
#define T_CLOSE 312
#define T_LISTEN 313
#define T_SYSLOG 314
#define T_WRITE_THROUGH 315
#define T_STAT_BUFFER_SIZE 316
#define T_DESTROY_TIMEOUT 317
#define T_MCAST_RCVBUFF 318
#define T_MCAST_SNDBUFF 319
#define T_NOTRACK 320
#define T_IP 321
#define T_PATH_VAL 322
#define T_NUMBER 323
#define T_STRING 324




/* Copy the first part of user declarations.  */
#line 1 "read_config_yy.y"

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


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 39 "read_config_yy.y"
{
	int		val;
	char		*string;
}
/* Line 187 of yacc.c.  */
#line 277 "read_config_yy.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 290 "read_config_yy.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  21
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   143

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  72
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  66
/* YYNRULES -- Number of rules.  */
#define YYNRULES  145
/* YYNRULES -- Number of states.  */
#define YYNSTATES  202

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   324

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    70,     2,    71,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    11,    13,    15,    17,
      19,    21,    23,    26,    29,    32,    35,    38,    41,    44,
      46,    49,    52,    55,    58,    61,    66,    67,    70,    73,
      76,    81,    82,    85,    88,    91,    94,    97,   100,   103,
     106,   109,   112,   115,   118,   123,   124,   127,   130,   133,
     138,   139,   142,   144,   146,   148,   150,   152,   157,   158,
     161,   163,   165,   167,   169,   171,   173,   175,   177,   179,
     181,   183,   185,   187,   189,   195,   201,   207,   208,   211,
     213,   215,   217,   219,   221,   222,   225,   227,   229,   231,
     232,   235,   237,   240,   243,   246,   248,   250,   253,   258,
     259,   262,   264,   266,   268,   270,   272,   274,   276,   278,
     280,   282,   284,   287,   290,   295,   296,   299,   301,   303,
     305,   307,   309,   311,   313,   315,   317,   319,   321,   324,
     327,   330,   335,   336,   339,   341,   343,   345,   347,   349,
     352,   355,   358,   361,   364,   367
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      73,     0,    -1,    -1,    74,    -1,    75,    -1,    74,    75,
      -1,    97,    -1,    86,    -1,    81,    -1,   124,    -1,   100,
      -1,   130,    -1,    19,    45,    -1,    19,    46,    -1,    19,
      67,    -1,    59,    45,    -1,    59,    46,    -1,    59,    69,
      -1,    26,    67,    -1,    27,    -1,    11,    68,    -1,    29,
      68,    -1,    30,    68,    -1,    43,    45,    -1,    43,    46,
      -1,    16,    70,    87,    71,    -1,    -1,    87,    88,    -1,
       3,    66,    -1,    12,    66,    -1,     8,    70,    90,    71,
      -1,    -1,    90,    91,    -1,     3,    66,    -1,    12,    66,
      -1,     4,    66,    -1,    13,    66,    -1,    49,    69,    -1,
      17,    68,    -1,    18,    68,    -1,    64,    68,    -1,    63,
      68,    -1,     6,    68,    -1,     7,    68,    -1,    10,    70,
      95,    71,    -1,    -1,    95,    96,    -1,     9,    67,    -1,
      17,    68,    -1,    25,    70,    98,    71,    -1,    -1,    98,
      99,    -1,    68,    -1,    20,    -1,    21,    -1,    23,    -1,
      22,    -1,    32,    70,   101,    71,    -1,    -1,   101,   102,
      -1,    82,    -1,    83,    -1,    84,    -1,    85,    -1,    89,
      -1,   115,    -1,   116,    -1,   103,    -1,   104,    -1,   105,
      -1,   117,    -1,   118,    -1,   123,    -1,   114,    -1,    37,
      41,    70,   106,    71,    -1,    37,    42,    70,   108,    71,
      -1,    37,    65,    70,   110,    71,    -1,    -1,   106,   107,
      -1,    82,    -1,    83,    -1,    84,    -1,   115,    -1,   116,
      -1,    -1,   108,   109,    -1,   112,    -1,    84,    -1,   113,
      -1,    -1,   110,   111,    -1,    84,    -1,    40,    68,    -1,
      44,    68,    -1,    62,    68,    -1,    34,    -1,    36,    -1,
      38,    66,    -1,    47,   119,    48,   120,    -1,    -1,   119,
     121,    -1,    24,    -1,   122,    -1,    51,    -1,    52,    -1,
      50,    -1,    53,    -1,    54,    -1,    55,    -1,    56,    -1,
      57,    -1,    58,    -1,    60,    45,    -1,    60,    46,    -1,
      31,    70,   125,    71,    -1,    -1,   125,   126,    -1,    92,
      -1,    93,    -1,    76,    -1,    77,    -1,    79,    -1,    78,
      -1,    80,    -1,    94,    -1,   127,    -1,   128,    -1,   129,
      -1,    35,    68,    -1,    28,    68,    -1,    39,    69,    -1,
      33,    70,   131,    71,    -1,    -1,   131,   132,    -1,   133,
      -1,   134,    -1,   135,    -1,   136,    -1,   137,    -1,    19,
      45,    -1,    19,    46,    -1,    19,    67,    -1,    59,    45,
      -1,    59,    46,    -1,    59,    69,    -1,    61,    68,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    64,    64,    65,    68,    69,    72,    73,    74,    75,
      76,    77,    80,    85,    89,    94,    99,   104,   136,   141,
     147,   152,   157,   162,   167,   172,   174,   175,   177,   205,
     239,   241,   242,   244,   261,   296,   313,   318,   336,   342,
     347,   352,   357,   362,   367,   369,   370,   373,   378,   383,
     385,   386,   389,   397,   402,   407,   412,   417,   427,   428,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   446,   451,   456,   461,   462,   464,
     465,   466,   467,   468,   471,   472,   474,   475,   476,   479,
     480,   482,   486,   491,   496,   501,   507,   513,   541,   543,
     544,   546,   547,   549,   553,   557,   561,   565,   569,   573,
     577,   581,   586,   591,   596,   598,   599,   602,   603,   604,
     605,   606,   607,   608,   609,   610,   611,   612,   615,   620,
     625,   633,   643,   644,   647,   648,   649,   650,   651,   654,
     659,   663,   668,   673,   678,   710
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_IPV4_ADDR", "T_IPV4_IFACE", "T_PORT",
  "T_HASHSIZE", "T_HASHLIMIT", "T_MULTICAST", "T_PATH", "T_UNIX",
  "T_REFRESH", "T_IPV6_ADDR", "T_IPV6_IFACE", "T_IGNORE_UDP",
  "T_IGNORE_ICMP", "T_IGNORE_TRAFFIC", "T_BACKLOG", "T_GROUP", "T_LOG",
  "T_UDP", "T_ICMP", "T_IGMP", "T_VRRP", "T_TCP", "T_IGNORE_PROTOCOL",
  "T_LOCK", "T_STRIP_NAT", "T_BUFFER_SIZE_MAX_GROWN", "T_EXPIRE",
  "T_TIMEOUT", "T_GENERAL", "T_SYNC", "T_STATS", "T_RELAX_TRANSITIONS",
  "T_BUFFER_SIZE", "T_DELAY", "T_SYNC_MODE", "T_LISTEN_TO", "T_FAMILY",
  "T_RESEND_BUFFER_SIZE", "T_ALARM", "T_FTFW", "T_CHECKSUM",
  "T_WINDOWSIZE", "T_ON", "T_OFF", "T_REPLICATE", "T_FOR", "T_IFACE",
  "T_ESTABLISHED", "T_SYN_SENT", "T_SYN_RECV", "T_FIN_WAIT",
  "T_CLOSE_WAIT", "T_LAST_ACK", "T_TIME_WAIT", "T_CLOSE", "T_LISTEN",
  "T_SYSLOG", "T_WRITE_THROUGH", "T_STAT_BUFFER_SIZE", "T_DESTROY_TIMEOUT",
  "T_MCAST_RCVBUFF", "T_MCAST_SNDBUFF", "T_NOTRACK", "T_IP", "T_PATH_VAL",
  "T_NUMBER", "T_STRING", "'{'", "'}'", "$accept", "configfile", "lines",
  "line", "logfile_bool", "logfile_path", "syslog_bool", "syslog_facility",
  "lock", "strip_nat", "refreshtime", "expiretime", "timeout", "checksum",
  "ignore_traffic", "ignore_traffic_options", "ignore_traffic_option",
  "multicast_line", "multicast_options", "multicast_option", "hashsize",
  "hashlimit", "unix_line", "unix_options", "unix_option",
  "ignore_protocol", "ignore_proto_list", "ignore_proto", "sync",
  "sync_list", "sync_line", "sync_mode_alarm", "sync_mode_ftfw",
  "sync_mode_notrack", "sync_mode_alarm_list", "sync_mode_alarm_line",
  "sync_mode_ftfw_list", "sync_mode_ftfw_line", "sync_mode_notrack_list",
  "sync_mode_notrack_line", "resend_queue_size", "window_size",
  "destroy_timeout", "relax_transitions", "delay_destroy_msgs",
  "listen_to", "state_replication", "states", "state_proto", "state",
  "tcp_state", "cache_writethrough", "general", "general_list",
  "general_line", "netlink_buffer_size", "netlink_buffer_size_max_grown",
  "family", "stats", "stats_list", "stat_line", "stat_logfile_bool",
  "stat_logfile_path", "stat_syslog_bool", "stat_syslog_facility",
  "buffer_size", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    72,    73,    73,    74,    74,    75,    75,    75,    75,
      75,    75,    76,    76,    77,    78,    78,    79,    80,    81,
      82,    83,    84,    85,    85,    86,    87,    87,    88,    88,
      89,    90,    90,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    92,    93,    94,    95,    95,    96,    96,    97,
      98,    98,    99,    99,    99,    99,    99,   100,   101,   101,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   103,   104,   105,   106,   106,   107,
     107,   107,   107,   107,   108,   108,   109,   109,   109,   110,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     119,   120,   121,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   123,   123,   124,   125,   125,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   127,   128,
     129,   130,   131,   131,   132,   132,   132,   132,   132,   133,
     133,   134,   135,   135,   136,   137
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     2,     2,     2,     1,
       2,     2,     2,     2,     2,     4,     0,     2,     2,     2,
       4,     0,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     4,     0,     2,     2,     2,     4,
       0,     2,     1,     1,     1,     1,     1,     4,     0,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     5,     5,     5,     0,     2,     1,
       1,     1,     1,     1,     0,     2,     1,     1,     1,     0,
       2,     1,     2,     2,     2,     1,     1,     2,     4,     0,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     4,     0,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     4,     0,     2,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     2,     2,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,    19,     0,     0,     0,     0,     3,     4,
       8,     7,     6,    10,     9,    11,    26,    50,   115,    58,
     132,     1,     5,     0,     0,     0,     0,     0,     0,     0,
      25,    27,    53,    54,    56,    55,    52,    49,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   114,   119,
     120,   122,   121,   123,   117,   118,   124,   116,   125,   126,
     127,     0,     0,     0,     0,    95,    96,     0,     0,     0,
      99,     0,     0,    57,    60,    61,    62,    63,    64,    59,
      67,    68,    69,    73,    65,    66,    70,    71,    72,     0,
       0,     0,   131,   133,   134,   135,   136,   137,   138,    28,
      29,    42,    43,    45,    12,    13,    14,    18,   129,   128,
     130,    15,    16,    17,    31,    20,    21,    22,     0,     0,
       0,    97,    23,    24,     0,   112,   113,    94,   139,   140,
     141,   142,   143,   144,   145,     0,     0,    77,    84,    89,
       0,   105,   103,   104,   106,   107,   108,   109,   110,   111,
     100,   102,     0,     0,    44,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    30,    32,     0,     0,     0,
     101,    98,    47,    48,    33,    35,    34,    36,    38,    39,
      37,    41,    40,    74,    79,    80,    81,    78,    82,    83,
       0,     0,    75,    87,    85,    86,    88,    76,    91,    90,
      92,    93
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     7,     8,     9,    49,    50,    51,    52,    53,    10,
      74,    75,    76,    77,    11,    23,    31,    78,   136,   166,
      54,    55,    56,   135,   155,    12,    24,    38,    13,    26,
      79,    80,    81,    82,   167,   187,   168,   194,   169,   199,
     195,   196,    83,    84,    85,    86,    87,   124,   171,   150,
     151,    88,    14,    25,    57,    58,    59,    60,    15,    27,
      93,    94,    95,    96,    97,    98
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -71
static const yytype_int8 yypact[] =
{
      69,   -51,   -47,   -71,   -45,   -43,   -41,    36,    69,   -71,
     -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,
     -71,   -71,   -71,    -1,    61,    -2,     5,    19,   -26,   -19,
     -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -17,
     -15,    -6,    26,    -9,   -13,     7,     4,   -39,   -71,   -71,
     -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,
     -71,    -4,     9,    11,    21,   -71,   -71,   -20,    29,    58,
     -71,    74,    24,   -71,   -71,   -71,   -71,   -71,   -71,   -71,
     -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,    42,
      17,    37,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,
     -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,
     -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,    51,    52,
      53,   -71,   -71,   -71,    60,   -71,   -71,   -71,   -71,   -71,
     -71,   -71,   -71,   -71,   -71,     3,    -3,   -71,   -71,   -71,
      82,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,
     -71,   -71,    40,    56,   -71,   -71,    59,    62,    64,    65,
      66,    67,    57,    68,    70,   -71,   -71,    20,   -12,   -27,
     -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,
     -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,
      71,    72,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,
     -71,   -71
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -71,   -71,   -71,   119,   -71,   -71,   -71,   -71,   -71,   -71,
     -34,   -30,   -70,   -71,   -71,   -71,   -71,   -71,   -71,   -71,
     -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,
     -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,
     -71,   -71,   -71,   -25,   -24,   -71,   -71,   -71,   -71,   -71,
     -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,
     -71,   -71,   -71,   -71,   -71,   -71
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
     156,   157,    28,    64,    39,    40,   111,   112,    41,   158,
     159,    29,   152,    61,   160,   161,    62,    42,    64,    16,
     153,   118,   119,    17,    43,    18,    44,    19,   190,    20,
     113,    62,   191,    45,    63,    64,    21,    46,    89,    65,
      99,    66,    67,    68,   197,   120,   162,   100,    69,    63,
      64,   101,    70,   102,    65,   108,    66,    47,   107,   192,
     163,   164,   131,   132,   103,    71,   114,    72,   165,    48,
      30,   104,   105,   110,   154,   109,    73,   115,    90,   116,
      91,    32,    33,    34,    35,     1,   133,   128,   129,   117,
      92,   183,   127,   106,     2,   121,     3,   186,   193,   198,
       4,     5,     6,   122,   123,   134,   170,   172,   140,   130,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   125,
     126,   137,   138,   139,   173,   174,   180,    22,   175,    36,
     176,   177,    37,   184,   178,   179,   181,   185,   182,   200,
     201,     0,   188,   189
};

static const yytype_int16 yycheck[] =
{
       3,     4,     3,    30,     6,     7,    45,    46,    10,    12,
      13,    12,     9,     8,    17,    18,    11,    19,    30,    70,
      17,    41,    42,    70,    26,    70,    28,    70,    40,    70,
      69,    11,    44,    35,    29,    30,     0,    39,    19,    34,
      66,    36,    37,    38,    71,    65,    49,    66,    43,    29,
      30,    68,    47,    68,    34,    68,    36,    59,    67,    71,
      63,    64,    45,    46,    70,    60,    70,    62,    71,    71,
      71,    45,    46,    69,    71,    68,    71,    68,    59,    68,
      61,    20,    21,    22,    23,    16,    69,    45,    46,    68,
      71,    71,    68,    67,    25,    66,    27,   167,   168,   169,
      31,    32,    33,    45,    46,    68,    24,    67,    48,    67,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    45,
      46,    70,    70,    70,    68,    66,    69,     8,    66,    68,
      66,    66,    71,   167,    68,    68,    68,   167,    68,    68,
      68,    -1,   167,   167
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    16,    25,    27,    31,    32,    33,    73,    74,    75,
      81,    86,    97,   100,   124,   130,    70,    70,    70,    70,
      70,     0,    75,    87,    98,   125,   101,   131,     3,    12,
      71,    88,    20,    21,    22,    23,    68,    71,    99,     6,
       7,    10,    19,    26,    28,    35,    39,    59,    71,    76,
      77,    78,    79,    80,    92,    93,    94,   126,   127,   128,
     129,     8,    11,    29,    30,    34,    36,    37,    38,    43,
      47,    60,    62,    71,    82,    83,    84,    85,    89,   102,
     103,   104,   105,   114,   115,   116,   117,   118,   123,    19,
      59,    61,    71,   132,   133,   134,   135,   136,   137,    66,
      66,    68,    68,    70,    45,    46,    67,    67,    68,    68,
      69,    45,    46,    69,    70,    68,    68,    68,    41,    42,
      65,    66,    45,    46,   119,    45,    46,    68,    45,    46,
      67,    45,    46,    69,    68,    95,    90,    70,    70,    70,
      48,    50,    51,    52,    53,    54,    55,    56,    57,    58,
     121,   122,     9,    17,    71,    96,     3,     4,    12,    13,
      17,    18,    49,    63,    64,    71,    91,   106,   108,   110,
      24,   120,    67,    68,    66,    66,    66,    66,    68,    68,
      69,    68,    68,    71,    82,    83,    84,   107,   115,   116,
      40,    44,    71,    84,   109,   112,   113,    71,    84,   111,
      68,    68
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 12:
#line 81 "read_config_yy.y"
    {
	strncpy(conf.logfile, DEFAULT_LOGFILE, FILENAME_MAXLEN);
}
    break;

  case 13:
#line 86 "read_config_yy.y"
    {
}
    break;

  case 14:
#line 90 "read_config_yy.y"
    {
	strncpy(conf.logfile, (yyvsp[(2) - (2)].string), FILENAME_MAXLEN);
}
    break;

  case 15:
#line 95 "read_config_yy.y"
    {
	conf.syslog_facility = DEFAULT_SYSLOG_FACILITY;
}
    break;

  case 16:
#line 100 "read_config_yy.y"
    {
	conf.syslog_facility = -1;
}
    break;

  case 17:
#line 105 "read_config_yy.y"
    {
	if (!strcmp((yyvsp[(2) - (2)].string), "daemon"))
		conf.syslog_facility = LOG_DAEMON;
	else if (!strcmp((yyvsp[(2) - (2)].string), "local0"))
		conf.syslog_facility = LOG_LOCAL0;
	else if (!strcmp((yyvsp[(2) - (2)].string), "local1"))
		conf.syslog_facility = LOG_LOCAL1;
	else if (!strcmp((yyvsp[(2) - (2)].string), "local2"))
		conf.syslog_facility = LOG_LOCAL2;
	else if (!strcmp((yyvsp[(2) - (2)].string), "local3"))
		conf.syslog_facility = LOG_LOCAL3;
	else if (!strcmp((yyvsp[(2) - (2)].string), "local4"))
		conf.syslog_facility = LOG_LOCAL4;
	else if (!strcmp((yyvsp[(2) - (2)].string), "local5"))
		conf.syslog_facility = LOG_LOCAL5;
	else if (!strcmp((yyvsp[(2) - (2)].string), "local6"))
		conf.syslog_facility = LOG_LOCAL6;
	else if (!strcmp((yyvsp[(2) - (2)].string), "local7"))
		conf.syslog_facility = LOG_LOCAL7;
	else {
		fprintf(stderr, "'%s' is not a known syslog facility, "
				"ignoring.\n", (yyvsp[(2) - (2)].string));
		break;
	}

	if (conf.stats.syslog_facility != -1 &&
	    conf.syslog_facility != conf.stats.syslog_facility)
	    	fprintf(stderr, "WARNING: Conflicting Syslog facility "
				"values, defaulting to General.\n");
}
    break;

  case 18:
#line 137 "read_config_yy.y"
    {
	strncpy(conf.lockfile, (yyvsp[(2) - (2)].string), FILENAME_MAXLEN);
}
    break;

  case 19:
#line 142 "read_config_yy.y"
    {
	fprintf(stderr, "Notice: StripNAT clause is obsolete. "
			"Please, remove it from conntrackd.conf\n");
}
    break;

  case 20:
#line 148 "read_config_yy.y"
    {
	conf.refresh = (yyvsp[(2) - (2)].val);
}
    break;

  case 21:
#line 153 "read_config_yy.y"
    {
	conf.cache_timeout = (yyvsp[(2) - (2)].val);
}
    break;

  case 22:
#line 158 "read_config_yy.y"
    {
	conf.commit_timeout = (yyvsp[(2) - (2)].val);
}
    break;

  case 23:
#line 163 "read_config_yy.y"
    {
	conf.mcast.checksum = 0;
}
    break;

  case 24:
#line 168 "read_config_yy.y"
    {
	conf.mcast.checksum = 1;
}
    break;

  case 28:
#line 178 "read_config_yy.y"
    {
	union inet_address ip;

	memset(&ip, 0, sizeof(union inet_address));

	if (!inet_aton((yyvsp[(2) - (2)].string), &ip.ipv4)) {
		fprintf(stderr, "%s is not a valid IPv4, ignoring", (yyvsp[(2) - (2)].string));
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
					"in the ignore pool\n", (yyvsp[(2) - (2)].string));
		if (errno == ENOSPC)
			fprintf(stderr, "Too many IP in the ignore pool!\n");
	}
}
    break;

  case 29:
#line 206 "read_config_yy.y"
    {
	union inet_address ip;

	memset(&ip, 0, sizeof(union inet_address));

#ifdef HAVE_INET_PTON_IPV6
	if (inet_pton(AF_INET6, (yyvsp[(2) - (2)].string), &ip.ipv6) <= 0) {
		fprintf(stderr, "%s is not a valid IPv6, ignoring", (yyvsp[(2) - (2)].string));
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
					"in the ignore pool\n", (yyvsp[(2) - (2)].string));
		if (errno == ENOSPC)
			fprintf(stderr, "Too many IP in the ignore pool!\n");
	}

}
    break;

  case 33:
#line 245 "read_config_yy.y"
    {
	if (!inet_aton((yyvsp[(2) - (2)].string), &conf.mcast.in)) {
		fprintf(stderr, "%s is not a valid IPv4 address\n", (yyvsp[(2) - (2)].string));
		break;
	}

        if (conf.mcast.ipproto == AF_INET6) {
		fprintf(stderr, "Your multicast address is IPv4 but "
		                "is binded to an IPv6 interface? Surely "
				"this is not what you want\n");
		break;
	}

	conf.mcast.ipproto = AF_INET;
}
    break;

  case 34:
#line 262 "read_config_yy.y"
    {
#ifdef HAVE_INET_PTON_IPV6
	if (inet_pton(AF_INET6, (yyvsp[(2) - (2)].string), &conf.mcast.in) <= 0) {
		fprintf(stderr, "%s is not a valid IPv6 address\n", (yyvsp[(2) - (2)].string));
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

		idx = if_nametoindex((yyvsp[(2) - (2)].string));
		if (!idx) {
			fprintf(stderr, "%s is an invalid interface.\n", (yyvsp[(2) - (2)].string));
			break;
		}

		conf.mcast.ifa.interface_index6 = idx;
		conf.mcast.ipproto = AF_INET6;
	}
}
    break;

  case 35:
#line 297 "read_config_yy.y"
    {
	if (!inet_aton((yyvsp[(2) - (2)].string), &conf.mcast.ifa)) {
		fprintf(stderr, "%s is not a valid IPv4 address\n", (yyvsp[(2) - (2)].string));
		break;
	}

        if (conf.mcast.ipproto == AF_INET6) {
		fprintf(stderr, "Your multicast interface is IPv4 but "
		                "is binded to an IPv6 interface? Surely "
				"this is not what you want\n");
		break;
	}

	conf.mcast.ipproto = AF_INET;
}
    break;

  case 36:
#line 314 "read_config_yy.y"
    {
	fprintf(stderr, "IPv6_interface not required for IPv6, ignoring.\n");
}
    break;

  case 37:
#line 319 "read_config_yy.y"
    {
	strncpy(conf.mcast.iface, (yyvsp[(2) - (2)].string), IFNAMSIZ);

	if (conf.mcast.ipproto == AF_INET6) {
		unsigned int idx;

		idx = if_nametoindex((yyvsp[(2) - (2)].string));
		if (!idx) {
			fprintf(stderr, "%s is an invalid interface.\n", (yyvsp[(2) - (2)].string));
			break;
		}

		conf.mcast.ifa.interface_index6 = idx;
		conf.mcast.ipproto = AF_INET6;
	}
}
    break;

  case 38:
#line 337 "read_config_yy.y"
    {
	fprintf(stderr, "Notice: Backlog option inside Multicast clause is "
			"obsolete. Please, remove it from conntrackd.conf.\n");
}
    break;

  case 39:
#line 343 "read_config_yy.y"
    {
	conf.mcast.port = (yyvsp[(2) - (2)].val);
}
    break;

  case 40:
#line 348 "read_config_yy.y"
    {
	conf.mcast.sndbuf = (yyvsp[(2) - (2)].val);
}
    break;

  case 41:
#line 353 "read_config_yy.y"
    {
	conf.mcast.rcvbuf = (yyvsp[(2) - (2)].val);
}
    break;

  case 42:
#line 358 "read_config_yy.y"
    {
	conf.hashsize = (yyvsp[(2) - (2)].val);
}
    break;

  case 43:
#line 363 "read_config_yy.y"
    {
	conf.limit = (yyvsp[(2) - (2)].val);
}
    break;

  case 47:
#line 374 "read_config_yy.y"
    {
	strcpy(conf.local.path, (yyvsp[(2) - (2)].string));
}
    break;

  case 48:
#line 379 "read_config_yy.y"
    {
	conf.local.backlog = (yyvsp[(2) - (2)].val);
}
    break;

  case 52:
#line 390 "read_config_yy.y"
    {
	if ((yyvsp[(1) - (1)].val) < IPPROTO_MAX)
		conf.ignore_protocol[(yyvsp[(1) - (1)].val)] = 1;
	else
		fprintf(stderr, "Protocol number `%d' is freak\n", (yyvsp[(1) - (1)].val));
}
    break;

  case 53:
#line 398 "read_config_yy.y"
    {
	conf.ignore_protocol[IPPROTO_UDP] = 1;
}
    break;

  case 54:
#line 403 "read_config_yy.y"
    {
	conf.ignore_protocol[IPPROTO_ICMP] = 1;
}
    break;

  case 55:
#line 408 "read_config_yy.y"
    {
	conf.ignore_protocol[IPPROTO_VRRP] = 1;
}
    break;

  case 56:
#line 413 "read_config_yy.y"
    {
	conf.ignore_protocol[IPPROTO_IGMP] = 1;
}
    break;

  case 57:
#line 418 "read_config_yy.y"
    {
	if (conf.flags & CTD_STATS_MODE) {
		fprintf(stderr, "ERROR: Cannot use both Stats and Sync "
				"clauses in conntrackd.conf.\n");
		exit(EXIT_FAILURE);
	}
	conf.flags |= CTD_SYNC_MODE;
}
    break;

  case 74:
#line 447 "read_config_yy.y"
    {
	conf.flags |= CTD_SYNC_ALARM;
}
    break;

  case 75:
#line 452 "read_config_yy.y"
    {
	conf.flags |= CTD_SYNC_FTFW;
}
    break;

  case 76:
#line 457 "read_config_yy.y"
    {
	conf.flags |= CTD_SYNC_NOTRACK;
}
    break;

  case 92:
#line 487 "read_config_yy.y"
    {
	conf.resend_queue_size = (yyvsp[(2) - (2)].val);
}
    break;

  case 93:
#line 492 "read_config_yy.y"
    {
	conf.window_size = (yyvsp[(2) - (2)].val);
}
    break;

  case 94:
#line 497 "read_config_yy.y"
    {
	conf.del_timeout = (yyvsp[(2) - (2)].val);
}
    break;

  case 95:
#line 502 "read_config_yy.y"
    {
	fprintf(stderr, "Notice: RelaxTransitions clause is obsolete. "
			"Please, remove it from conntrackd.conf\n");
}
    break;

  case 96:
#line 508 "read_config_yy.y"
    {
	fprintf(stderr, "Notice: DelayDestroyMessages clause is obsolete. "
			"Please, remove it from conntrackd.conf\n");
}
    break;

  case 97:
#line 514 "read_config_yy.y"
    {
	union inet_address addr;

#ifdef HAVE_INET_PTON_IPV6
	if (inet_pton(AF_INET6, (yyvsp[(2) - (2)].string), &addr.ipv6) <= 0)
#endif
		if (inet_aton((yyvsp[(2) - (2)].string), &addr.ipv4) <= 0) {
			fprintf(stderr, "%s is not a valid IP address\n", (yyvsp[(2) - (2)].string));
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
}
    break;

  case 103:
#line 550 "read_config_yy.y"
    {
	state_helper_register(&tcp_state_helper, TCP_CONNTRACK_SYN_SENT);
}
    break;

  case 104:
#line 554 "read_config_yy.y"
    {
	state_helper_register(&tcp_state_helper, TCP_CONNTRACK_SYN_RECV);
}
    break;

  case 105:
#line 558 "read_config_yy.y"
    {
	state_helper_register(&tcp_state_helper, TCP_CONNTRACK_ESTABLISHED);
}
    break;

  case 106:
#line 562 "read_config_yy.y"
    {
	state_helper_register(&tcp_state_helper, TCP_CONNTRACK_FIN_WAIT);
}
    break;

  case 107:
#line 566 "read_config_yy.y"
    {
	state_helper_register(&tcp_state_helper, TCP_CONNTRACK_CLOSE_WAIT);
}
    break;

  case 108:
#line 570 "read_config_yy.y"
    {
	state_helper_register(&tcp_state_helper, TCP_CONNTRACK_LAST_ACK);
}
    break;

  case 109:
#line 574 "read_config_yy.y"
    {
	state_helper_register(&tcp_state_helper, TCP_CONNTRACK_TIME_WAIT);
}
    break;

  case 110:
#line 578 "read_config_yy.y"
    {
	state_helper_register(&tcp_state_helper, TCP_CONNTRACK_CLOSE);
}
    break;

  case 111:
#line 582 "read_config_yy.y"
    {
	state_helper_register(&tcp_state_helper, TCP_CONNTRACK_LISTEN);
}
    break;

  case 112:
#line 587 "read_config_yy.y"
    {
	conf.cache_write_through = 1;
}
    break;

  case 113:
#line 592 "read_config_yy.y"
    {
	conf.cache_write_through = 0;
}
    break;

  case 128:
#line 616 "read_config_yy.y"
    {
	conf.netlink_buffer_size = (yyvsp[(2) - (2)].val);
}
    break;

  case 129:
#line 621 "read_config_yy.y"
    {
	conf.netlink_buffer_size_max_grown = (yyvsp[(2) - (2)].val);
}
    break;

  case 130:
#line 626 "read_config_yy.y"
    {
	if (strncmp((yyvsp[(2) - (2)].string), "IPv6", strlen("IPv6")) == 0)
		conf.family = AF_INET6;
	else
		conf.family = AF_INET;
}
    break;

  case 131:
#line 634 "read_config_yy.y"
    {
	if (conf.flags & CTD_SYNC_MODE) {
		fprintf(stderr, "ERROR: Cannot use both Stats and Sync "
				"clauses in conntrackd.conf.\n");
		exit(EXIT_FAILURE);
	}
	conf.flags |= CTD_STATS_MODE;
}
    break;

  case 139:
#line 655 "read_config_yy.y"
    {
	strncpy(conf.stats.logfile, DEFAULT_STATS_LOGFILE, FILENAME_MAXLEN);
}
    break;

  case 140:
#line 660 "read_config_yy.y"
    {
}
    break;

  case 141:
#line 664 "read_config_yy.y"
    {
	strncpy(conf.stats.logfile, (yyvsp[(2) - (2)].string), FILENAME_MAXLEN);
}
    break;

  case 142:
#line 669 "read_config_yy.y"
    {
	conf.stats.syslog_facility = DEFAULT_SYSLOG_FACILITY;
}
    break;

  case 143:
#line 674 "read_config_yy.y"
    {
	conf.stats.syslog_facility = -1;
}
    break;

  case 144:
#line 679 "read_config_yy.y"
    {
	if (!strcmp((yyvsp[(2) - (2)].string), "daemon"))
		conf.stats.syslog_facility = LOG_DAEMON;
	else if (!strcmp((yyvsp[(2) - (2)].string), "local0"))
		conf.stats.syslog_facility = LOG_LOCAL0;
	else if (!strcmp((yyvsp[(2) - (2)].string), "local1"))
		conf.stats.syslog_facility = LOG_LOCAL1;
	else if (!strcmp((yyvsp[(2) - (2)].string), "local2"))
		conf.stats.syslog_facility = LOG_LOCAL2;
	else if (!strcmp((yyvsp[(2) - (2)].string), "local3"))
		conf.stats.syslog_facility = LOG_LOCAL3;
	else if (!strcmp((yyvsp[(2) - (2)].string), "local4"))
		conf.stats.syslog_facility = LOG_LOCAL4;
	else if (!strcmp((yyvsp[(2) - (2)].string), "local5"))
		conf.stats.syslog_facility = LOG_LOCAL5;
	else if (!strcmp((yyvsp[(2) - (2)].string), "local6"))
		conf.stats.syslog_facility = LOG_LOCAL6;
	else if (!strcmp((yyvsp[(2) - (2)].string), "local7"))
		conf.stats.syslog_facility = LOG_LOCAL7;
	else {
		fprintf(stderr, "'%s' is not a known syslog facility, "
				"ignoring.\n", (yyvsp[(2) - (2)].string));
		break;
	}

	if (conf.syslog_facility != -1 &&
	    conf.stats.syslog_facility != conf.syslog_facility)
		fprintf(stderr, "WARNING: Conflicting Syslog facility "
				"values, defaulting to General.\n");
}
    break;

  case 145:
#line 711 "read_config_yy.y"
    {
	fprintf(stderr, "WARNING: LogFileBufferSize is deprecated.\n");
}
    break;


/* Line 1267 of yacc.c.  */
#line 2374 "read_config_yy.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 715 "read_config_yy.y"


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


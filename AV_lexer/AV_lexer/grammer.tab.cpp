/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "grammar.y" /* yacc.c:339  */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <iterator>
#include <iostream>
#include <sstream>
#include <regex>
#include "SyntaxTree.h"

int yylex(void);
void yyerror(char const *);

using namespace std;

enum VAR_TYPE
{
	notype,
	dec,
	hex,
	text
};

class dataDef
{
private:
	map< string, map<VAR_TYPE, vector<string>>> comp_holder;

public:

	dataDef() = default;
	~dataDef() = default;

	void addData(string var, VAR_TYPE type, vector<string> lst)
	{
		comp_holder[var] = { { type , lst } };
	}

	VAR_TYPE getType(string var)
	{
		for (auto outer : comp_holder)
		{
			if (outer.first.c_str() == var)
			{
				for (auto inner : outer.second)
				{
					return inner.first;
				}
			}
		}
		return notype;
	}

	vector<string> getData(string var)
	{
		for (auto outer : comp_holder)
		{
			if (outer.first.c_str() == var)
			{
				for (auto inner : outer.second)
				{
					return inner.second;
				}
			}
		}
		return{};
	}

	void clear()
	{
		for (auto outer : comp_holder)
		{
			outer.second.clear();
		}
		comp_holder.clear();
		comp_holder = map< string, map<VAR_TYPE, vector<string>>>();
	}

	int varCount() { return comp_holder.size(); }
}DataHolder;


#line 151 "grammar.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
by #include "grammar.tab.h".  */
#ifndef YY_YY_GRAMMAR_TAB_H_INCLUDED
# define YY_YY_GRAMMAR_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
enum yytokentype
{
	STR = 258,
	NUM = 259,
	VAR = 260,
	ADD = 261,
	SUB = 262,
	EQL = 263,
	GTR = 264,
	LSS = 265,
	LPRN = 266,
	RPRN = 267,
	LSQR = 268,
	TO = 269,
	COMMA = 270,
	IMPORT = 271,
	EXPORT = 272,
	INSENSITIVE = 273,
	RSQR = 274,
	EOL = 275,
	EOF_F = 276,
	LEFT_BRACE = 277,
	RIGHT_BRACE = 278,
	UNK = 279,
	INFO = 280,
	TYPES = 281,
	CHECK = 282,
	OR = 283,
	AND = 284,
	NNOT = 285,
	EXPR = 286,
	AT = 287,
	SIG = 288,
	TXT = 289,
	COLON = 290,
	WIDE = 291,
	ASCII = 292,
	NONALPHA = 293,
	HEX = 294,
	STAR = 295,
	NOT = 296,
	FNAME = 297,
	FSIZE = 298,
	CALL = 299,
	REG = 300
};
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef void * YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse(void);

#endif /* !YY_YY_GRAMMAR_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 248 "grammar.tab.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
/* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
/* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc(YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free(void *); /* INFRINGES ON USER NAME SPACE */
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
	yytype_int16 yyss_alloc;
	YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
local variables YYSIZE and YYSTACKSIZE give the old and new number of
elements in the stack, and YYPTR gives the new location of the
stack.  Advance YYPTR to a properly aligned location for the next
stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   188

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  10
/* YYNRULES -- Number of rules.  */
#define YYNRULES  52
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  182

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   300

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
as returned by yylex, without out-of-bounds checking.  */
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
	2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
	2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
	5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
	15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
	25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
	35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
	45
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
	0,    95,    95,    96,    97,    98,   101,   102,   103,   104,
	107,   108,   111,   112,   115,   116,   120,   126,   131,   137,
	143,   149,   155,   164,   173,   182,   191,   199,   208,   217,
	227,   233,   243,   272,   291,   300,   311,   322,   333,   336,
	337,   340,   341,   345,   350,   355,   377,   399,   421,   432,
	443,   454,   491
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
	"$end", "error", "$undefined", "STR", "NUM", "VAR", "ADD", "SUB", "EQL",
	"GTR", "LSS", "LPRN", "RPRN", "LSQR", "TO", "COMMA", "IMPORT", "EXPORT",
	"INSENSITIVE", "RSQR", "EOL", "EOF_F", "LEFT_BRACE", "RIGHT_BRACE",
	"UNK", "INFO", "TYPES", "CHECK", "OR", "AND", "NNOT", "EXPR", "AT",
	"SIG", "TXT", "COLON", "WIDE", "ASCII", "NONALPHA", "HEX", "STAR", "NOT",
	"FNAME", "FSIZE", "CALL", "REG", "$accept", "program", "body",
	"info_grp", "type_grp", "declr", "var_dec", "check_group", "ch",
	"ch_loop", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
(internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
	0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
	265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
	275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
	285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
	295,   296,   297,   298,   299,   300
};
# endif

#define YYPACT_NINF -24

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-24)))

#define YYTABLE_NINF -12

#define yytable_value_is_error(Yytable_value) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
STATE-NUM.  */
static const yytype_int16 yypact[] =
{
	-13,   -24,   -23,     3,   -12,   -24,   -13,    55,    13,    31,
	26,   -24,    21,    35,   -20,    54,    10,   -24,    36,   -24,
	36,    39,    62,    58,    63,    64,    65,    66,   -24,   -24,
	-1,   -24,   -24,    47,    -3,    67,    68,    69,    70,     1,
	74,    61,   -24,    48,    53,    60,    72,    73,    77,     4,
	8,    75,   -24,    71,    82,    84,    85,   -24,    86,    87,
	88,    -2,    49,    91,    89,   -24,   -24,    92,    93,    94,
	95,    96,    97,    18,    46,    98,    99,    79,    80,    81,
	90,   100,   101,   102,   103,   104,   105,    -6,   107,   110,
	111,    23,   109,   112,   113,   108,   114,   115,   123,   125,
	40,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,
	78,   116,   127,   117,   128,   129,   130,   134,   106,   119,
	120,   121,   -24,   122,   -24,   124,   126,   131,   140,   132,
	-24,   133,   -24,   118,   135,   136,   143,   144,   -24,   -24,
	-24,   -24,   -24,   -24,   -24,   137,   -24,   138,   150,   151,
	141,   139,   142,   145,   -24,   147,   148,   -24,   153,   156,
	-24,   152,   161,   154,   149,   157,   158,   167,   159,   155,
	160,   162,   168,   -24,   -24,   163,   164,   -24,   169,   165,
	166,   -24
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
Performed when YYTABLE does not specify something else to do.  Zero
means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
	5,     4,     0,     0,     0,     1,     2,     0,     0,     9,
	0,    15,     0,    13,    11,     0,    12,     3,    40,    42,
	40,    11,     0,     0,     0,     0,     0,     0,    14,     6,
	39,     7,     8,     0,     0,     0,     0,     0,     0,     0,
	0,     0,    41,     0,     0,     0,     0,     0,     0,     0,
	0,     0,    16,     0,     0,     0,     0,    51,     0,     0,
	0,     0,     0,     0,     0,    17,    30,     0,     0,     0,
	0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
	0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
	0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
	0,    18,    19,    20,    21,    45,    46,    47,    48,    49,
	0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
	0,     0,    28,     0,    29,     0,     0,     0,     0,     0,
	43,     0,    10,     0,     0,     0,     0,     0,    22,    23,
	26,    24,    25,    27,    36,     0,    50,     0,     0,     0,
	0,     0,     0,     0,    44,     0,     0,    31,     0,     0,
	37,     0,     0,     0,     0,     0,     0,     0,     0,     0,
	0,     0,     0,    34,    35,     0,     0,    33,     0,     0,
	0,    32
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
	-24,   176,   -24,    11,   171,   -24,   -24,    33,   -24,   -24
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
	-1,     6,    12,    13,    14,    16,    28,    21,    30,    42
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
positive, shift that token.  If negative, reduce the rule whose
number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
	44,    45,    84,     5,    39,    10,   111,    19,     1,   112,
	40,     4,    70,    46,    47,    23,    72,    73,    74,    48,
	2,    57,    95,     7,     1,    20,    96,   116,    24,    58,
	59,    60,    32,    61,   117,     9,     2,    85,   118,    49,
	50,    51,    52,    41,    17,    71,    25,    26,    27,    75,
	97,    29,   127,    31,    98,   128,    10,    11,   -11,     8,
	15,    11,    22,    19,    10,    33,    34,    43,    35,    36,
	37,    38,    63,    65,    77,    53,    54,    55,    56,    62,
	66,    69,    64,    67,    68,    78,    76,    79,    80,    86,
	129,    81,    82,    83,    87,    89,    90,    88,    91,   101,
	102,   103,   100,    92,    93,    94,    99,     0,     0,   110,
	104,   113,   119,     0,     0,   120,     0,   121,   123,   137,
	105,   106,   107,   108,   109,   114,   115,   125,   122,   126,
	131,   133,   134,   148,   135,   124,   130,   132,   136,   138,
	139,   140,   141,   145,   142,   147,   143,   151,   152,   153,
	149,   144,   146,   155,   156,   165,   159,   163,   154,   150,
	164,   157,   161,   162,   166,   160,   167,   158,   168,   169,
	170,   171,   176,   179,   172,   173,     3,     0,     0,   178,
	174,     0,     0,   177,    18,   175,   181,     0,   180
};

static const yytype_int8 yycheck[] =
{
	3,     4,     4,     0,     5,    25,    12,    27,    21,    15,
	11,    34,     8,    16,    17,     5,     8,     9,    10,    22,
	33,    20,     4,    35,    21,    14,     8,     4,    18,    28,
	29,    30,    21,    32,    11,    22,    33,    39,    15,    42,
	43,    44,    45,    44,    23,    41,    36,    37,    38,    41,
	4,    18,    12,    20,     8,    15,    25,    26,    27,     4,
	34,    26,     8,    27,    25,     3,     8,    20,     5,     5,
	5,     5,    11,    20,     3,     8,     8,     8,     8,     5,
	20,     4,    34,    11,    11,     3,    11,     3,     3,    40,
	12,     5,     5,     5,     3,     3,     3,     8,     4,    20,
	20,    20,     3,     8,     8,     8,     8,    -1,    -1,     4,
	20,     4,     3,    -1,    -1,     3,    -1,     4,     4,    13,
	20,    20,    20,    20,    20,    15,    15,     4,    20,     4,
	3,     3,     3,    15,     4,    20,    20,    20,     4,    20,
	20,    20,    20,     3,    20,    12,    20,     4,     4,    12,
	15,    20,    20,     3,     3,     3,    14,     4,    20,    23,
	4,    20,    15,    15,     3,    20,    12,    28,    19,    12,
	12,     4,     4,     4,    15,    20,     0,    -1,    -1,    15,
	20,    -1,    -1,    20,    13,    23,    20,    -1,    23
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
	0,    21,    33,    47,    34,     0,    47,    35,     4,    22,
	25,    26,    48,    49,    50,    34,    51,    23,    50,    27,
	49,    53,     8,     5,    18,    36,    37,    38,    52,    53,
	54,    53,    49,     3,     8,     5,     5,     5,     5,     5,
	11,    44,    55,    20,     3,     4,    16,    17,    22,    42,
	43,    44,    45,     8,     8,     8,     8,    20,    28,    29,
	30,    32,     5,    11,    34,    20,    20,    11,    11,     4,
	8,    41,     8,     9,    10,    41,    11,     3,     3,     3,
	3,     5,     5,     5,     4,    39,    40,     3,     8,     3,
	3,     4,     8,     8,     8,     4,     8,     4,     8,     8,
	3,    20,    20,    20,    20,    20,    20,    20,    20,    20,
	4,    12,    15,     4,    15,    15,     4,    11,    15,     3,
	3,     4,    20,     4,    20,     4,     4,    12,    15,    12,
	20,     3,    20,     3,     3,     4,     4,    13,    20,    20,
	20,    20,    20,    20,    20,     3,    20,    12,    15,    15,
	23,     4,     4,    12,    20,     3,     3,    20,    28,    14,
	20,    15,    15,     4,     4,     3,     3,    12,    19,    12,
	12,     4,    15,    20,    20,    23,     4,    20,    15,     4,
	23,    20
};

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
	0,    46,    47,    47,    47,    47,    48,    48,    48,    48,
	49,    49,    50,    50,    51,    51,    52,    52,    52,    52,
	52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
	52,    52,    52,    52,    52,    52,    52,    52,    52,    53,
	53,    54,    54,    55,    55,    55,    55,    55,    55,    55,
	55,    55,    55
};

/* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
	0,     2,     2,     7,     1,     0,     3,     3,     3,     0,
	9,     0,     2,     0,     2,     0,     3,     4,     5,     5,
	5,     5,     7,     7,     7,     7,     7,     7,     6,     6,
	4,     9,    17,    14,    13,    13,     7,     9,     0,     2,
	0,     2,     0,     5,     7,     4,     4,     4,     4,     4,
	6,     2,     0
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print(FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
	FILE *yyo = yyoutput;
	YYUSE(yyo);
	if (!yyvaluep)
		return;
# ifdef YYPRINT
	if (yytype < YYNTOKENS)
		YYPRINT(yyoutput, yytoknum[yytype], *yyvaluep);
# endif
	YYUSE(yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print(FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
	YYFPRINTF(yyoutput, "%s %s (",
		yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

	yy_symbol_value_print(yyoutput, yytype, yyvaluep);
	YYFPRINTF(yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print(yytype_int16 *yybottom, yytype_int16 *yytop)
{
	YYFPRINTF(stderr, "Stack now");
	for (; yybottom <= yytop; yybottom++)
	{
		int yybot = *yybottom;
		YYFPRINTF(stderr, " %d", yybot);
	}
	YYFPRINTF(stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print(yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
	unsigned long int yylno = yyrline[yyrule];
	int yynrhs = yyr2[yyrule];
	int yyi;
	YYFPRINTF(stderr, "Reducing stack by rule %d (line %lu):\n",
		yyrule - 1, yylno);
	/* The symbols being reduced.  */
	for (yyi = 0; yyi < yynrhs; yyi++)
	{
		YYFPRINTF(stderr, "   $%d = ", yyi + 1);
		yy_symbol_print(stderr,
			yystos[yyssp[yyi + 1 - yynrhs]],
			&(yyvsp[(yyi + 1) - (yynrhs)])
		);
		YYFPRINTF(stderr, "\n");
	}
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen(const char *yystr)
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
static char *
yystpcpy(char *yydest, const char *yysrc)
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
yytnamerr(char *yyres, const char *yystr)
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
	do_not_strip_quotes:;
	}

	if (!yyres)
		return yystrlen(yystr);

	return yystpcpy(yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
about the unexpected token YYTOKEN for the state stack whose top is
YYSSP.

Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
not large enough to hold the message.  In that case, also set
*YYMSG_ALLOC to the required number of bytes.  Return 2 if the
required number of bytes is too large to store.  */
static int
yysyntax_error(YYSIZE_T *yymsg_alloc, char **yymsg,
	yytype_int16 *yyssp, int yytoken)
{
	YYSIZE_T yysize0 = yytnamerr(YY_NULLPTR, yytname[yytoken]);
	YYSIZE_T yysize = yysize0;
	enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
	/* Internationalized format string. */
	const char *yyformat = YY_NULLPTR;
	/* Arguments of yyformat. */
	char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	/* Number of reported tokens (one for the "unexpected", one per
	"expected"). */
	int yycount = 0;

	/* There are many possibilities here to consider:
	- If this state is a consistent state with a default action, then
	the only way this function was invoked is if the default action
	is an error action.  In that case, don't check for expected
	tokens because there are none.
	- The only way there can be no lookahead present (in yychar) is if
	this state is a consistent state with a default action.  Thus,
	detecting the absence of a lookahead is sufficient to determine
	that there is no unexpected or expected token to report.  In that
	case, just report a simple "syntax error".
	- Don't assume there isn't a lookahead just because this state is a
	consistent state with a default action.  There might have been a
	previous inconsistent state, consistent state with a non-default
	action, or user semantic action that manipulated yychar.
	- Of course, the expected token list depends on states to have
	correct lookahead information, and it depends on the parser not
	to perform extra reductions after fetching a lookahead from the
	scanner and before detecting a syntax error.  Thus, state merging
	(from LALR or IELR) and default reductions corrupt the expected
	token list.  However, the list is correct for canonical LR with
	one exception: it will still contain any token that will not be
	accepted due to an error action in a later state.
	*/
	if (yytoken != YYEMPTY)
	{
		int yyn = yypact[*yyssp];
		yyarg[yycount++] = yytname[yytoken];
		if (!yypact_value_is_default(yyn))
		{
			/* Start YYX at -YYN if negative to avoid negative indexes in
			YYCHECK.  In other words, skip the first -YYN actions for
			this state because they are default actions.  */
			int yyxbegin = yyn < 0 ? -yyn : 0;
			/* Stay within bounds of both yycheck and yytname.  */
			int yychecklim = YYLAST - yyn + 1;
			int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
			int yyx;

			for (yyx = yyxbegin; yyx < yyxend; ++yyx)
				if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
					&& !yytable_value_is_error(yytable[yyx + yyn]))
				{
					if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
					{
						yycount = 1;
						yysize = yysize0;
						break;
					}
					yyarg[yycount++] = yytname[yyx];
					{
						YYSIZE_T yysize1 = yysize + yytnamerr(YY_NULLPTR, yytname[yyx]);
						if (!(yysize <= yysize1
							&& yysize1 <= YYSTACK_ALLOC_MAXIMUM))
							return 2;
						yysize = yysize1;
					}
				}
		}
	}

	switch (yycount)
	{
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
		YYCASE_(0, YY_("syntax error"));
		YYCASE_(1, YY_("syntax error, unexpected %s"));
		YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
		YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
		YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
		YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
	}

	{
		YYSIZE_T yysize1 = yysize + yystrlen(yyformat);
		if (!(yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
			return 2;
		yysize = yysize1;
	}

	if (*yymsg_alloc < yysize)
	{
		*yymsg_alloc = 2 * yysize;
		if (!(yysize <= *yymsg_alloc
			&& *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
			*yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
		return 1;
	}

	/* Avoid sprintf, as that infringes on the user's name space.
	Don't have undefined behavior even if the translation
	produced a string with the wrong number of "%s"s.  */
	{
		char *yyp = *yymsg;
		int yyi = 0;
		while ((*yyp = *yyformat) != '\0')
			if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
			{
				yyp += yytnamerr(yyp, yyarg[yyi++]);
				yyformat += 2;
			}
			else
			{
				yyp++;
				yyformat++;
			}
	}
	return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct(const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
	YYUSE(yyvaluep);
	if (!yymsg)
		yymsg = "Deleting";
	YY_SYMBOL_PRINT(yymsg, yytype, yyvaluep, yylocationp);

	YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
		YYUSE(yytype);
	YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse(void)
{
	int yystate;
	/* Number of tokens to shift before error messages enabled.  */
	int yyerrstatus;

	/* The stacks and their tools:
	'yyss': related to states.
	'yyvs': related to semantic values.

	Refer to the stacks through separate pointers, to allow yyoverflow
	to reallocate them elsewhere.  */

	/* The state stack.  */
	yytype_int16 yyssa[YYINITDEPTH];
	yytype_int16 *yyss;
	yytype_int16 *yyssp;

	/* The semantic value stack.  */
	YYSTYPE yyvsa[YYINITDEPTH];
	YYSTYPE *yyvs;
	YYSTYPE *yyvsp;

	YYSIZE_T yystacksize;

	int yyn;
	int yyresult;
	/* Lookahead token as an internal (translated) token number.  */
	int yytoken = 0;
	/* The variables used to return semantic value and location from the
	action routines.  */
	YYSTYPE yyval;

#if YYERROR_VERBOSE
	/* Buffer for error messages, and its allocated size.  */
	char yymsgbuf[128];
	char *yymsg = yymsgbuf;
	YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

	/* The number of symbols on the RHS of the reduced rule.
	Keep to zero when no symbol should be popped.  */
	int yylen = 0;

	yyssp = yyss = yyssa;
	yyvsp = yyvs = yyvsa;
	yystacksize = YYINITDEPTH;

	YYDPRINTF((stderr, "Starting parse\n"));

	yystate = 0;
	yyerrstatus = 0;
	yynerrs = 0;
	yychar = YYEMPTY; /* Cause a token to be read.  */
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
			yyoverflow(YY_("memory exhausted"),
				&yyss1, yysize * sizeof(*yyssp),
				&yyvs1, yysize * sizeof(*yyvsp),
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
				(union yyalloc *) YYSTACK_ALLOC(YYSTACK_BYTES(yystacksize));
			if (!yyptr)
				goto yyexhaustedlab;
			YYSTACK_RELOCATE(yyss_alloc, yyss);
			YYSTACK_RELOCATE(yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
			if (yyss1 != yyssa)
				YYSTACK_FREE(yyss1);
		}
# endif
#endif /* no yyoverflow */

		yyssp = yyss + yysize - 1;
		yyvsp = yyvs + yysize - 1;

		YYDPRINTF((stderr, "Stack size increased to %lu\n",
			(unsigned long int) yystacksize));

		if (yyss + yystacksize - 1 <= yyssp)
			YYABORT;
	}

	YYDPRINTF((stderr, "Entering state %d\n", yystate));

	if (yystate == YYFINAL)
		YYACCEPT;

	goto yybackup;

	/*-----------.
	| yybackup.  |
	`-----------*/
yybackup:

	/* Do appropriate processing given the current state.  Read a
	lookahead token if we need one and don't already have one.  */

	/* First try to decide what to do without reference to lookahead token.  */
	yyn = yypact[yystate];
	if (yypact_value_is_default(yyn))
		goto yydefault;

	/* Not known => get a lookahead token if don't already have one.  */

	/* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
	if (yychar == YYEMPTY)
	{
		YYDPRINTF((stderr, "Reading a token: "));
		yychar = yylex();
	}

	if (yychar <= YYEOF)
	{
		yychar = yytoken = YYEOF;
		YYDPRINTF((stderr, "Now at end of input.\n"));
	}
	else
	{
		yytoken = YYTRANSLATE(yychar);
		YY_SYMBOL_PRINT("Next token is", yytoken, &yylval, &yylloc);
	}

	/* If the proper action on seeing token YYTOKEN is to reduce or to
	detect an error, take that action.  */
	yyn += yytoken;
	if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
		goto yydefault;
	yyn = yytable[yyn];
	if (yyn <= 0)
	{
		if (yytable_value_is_error(yyn))
			goto yyerrlab;
		yyn = -yyn;
		goto yyreduce;
	}

	/* Count tokens shifted since error; after three, turn off error
	status.  */
	if (yyerrstatus)
		yyerrstatus--;

	/* Shift the lookahead token.  */
	YY_SYMBOL_PRINT("Shifting", yytoken, &yylval, &yylloc);

	/* Discard the shifted token.  */
	yychar = YYEMPTY;

	yystate = yyn;
	YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
		*++yyvsp = yylval;
	YY_IGNORE_MAYBE_UNINITIALIZED_END

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
	'$$ = $1'.

	Otherwise, the following line sets YYVAL to garbage.
	This behavior is undocumented and Bison
	users should not rely upon it.  Assigning to YYVAL
	unconditionally makes the parser a bit smaller, and it avoids a
	GCC warning that YYVAL may be used uninitialized.  */
	yyval = yyvsp[1 - yylen];


	YY_REDUCE_PRINT(yyn);
	switch (yyn)
	{
	case 3:
#line 96 "grammar.y" /* yacc.c:1646  */
	{ return 0; }
#line 1433 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 4:
#line 97 "grammar.y" /* yacc.c:1646  */
	{ return -1; }
#line 1439 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 16:
#line 120 "grammar.y" /* yacc.c:1646  */
	{
		string tp((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[0])))->toCode().c_str());
		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-2])))->toCode(), VAR_TYPE::text,
		{ tp });
		free((yyvsp[-2])); free((yyvsp[0]));
	}
#line 1450 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 17:
#line 126 "grammar.y" /* yacc.c:1646  */
	{
		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-3])))->toCode(), VAR_TYPE::text,
		{ (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode() });
		free((yyvsp[-3])); free((yyvsp[-1]));
	}
#line 1460 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 18:
#line 132 "grammar.y" /* yacc.c:1646  */
	{
		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-3])))->toCode(), VAR_TYPE::text,
		{ "insensitive" , (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode() });
		free((yyvsp[-3])); free((yyvsp[-1]));
	}
#line 1470 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 19:
#line 138 "grammar.y" /* yacc.c:1646  */
	{
		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-3])))->toCode(), VAR_TYPE::text,
		{ "wide" , (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode() });
		free((yyvsp[-3])); free((yyvsp[-1]));
	}
#line 1480 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 20:
#line 144 "grammar.y" /* yacc.c:1646  */
	{
		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-3])))->toCode(), VAR_TYPE::text,
		{ "ascii" , (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode() });
		free((yyvsp[-3])); free((yyvsp[-1]));
	}
#line 1490 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 21:
#line 150 "grammar.y" /* yacc.c:1646  */
	{
		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-3])))->toCode(), VAR_TYPE::text,
		{ "nonalpha" , (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode() });
		free((yyvsp[-3])); free((yyvsp[-1]));
	}
#line 1500 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 22:
#line 156 "grammar.y" /* yacc.c:1646  */
	{
		stringstream ss;
		ss << "True if filename " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode().c_str() << " exist else false";
		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-6])))->toCode(), VAR_TYPE::text,
		{ ss.str() });
		free((yyvsp[-6])); free((yyvsp[-1]));
	}
#line 1512 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 23:
#line 165 "grammar.y" /* yacc.c:1646  */
	{
		stringstream ss;
		ss << "False if filename " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode().c_str() << " exist else true";
		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-6])))->toCode(), VAR_TYPE::text,
		{ ss.str() });
		free((yyvsp[-6])); free((yyvsp[-1]));
	}
#line 1524 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 24:
#line 174 "grammar.y" /* yacc.c:1646  */
	{
		stringstream ss;
		ss << "True if filesize is greater or equal to " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode().c_str();
		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-6])))->toCode(), VAR_TYPE::text,
		{ ss.str() });
		free((yyvsp[-6])); free((yyvsp[-1]));
	}
#line 1536 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 25:
#line 183 "grammar.y" /* yacc.c:1646  */
	{
		stringstream ss;
		ss << "True if filesize is less or equal to " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode().c_str();
		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-6])))->toCode(), VAR_TYPE::text,
		{ ss.str() });
		free((yyvsp[-6])); free((yyvsp[-1]));
	}
#line 1548 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 26:
#line 192 "grammar.y" /* yacc.c:1646  */
	{
		stringstream ss;
		ss << "True if filesize is equal to " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode().c_str();
		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-6])))->toCode(), VAR_TYPE::text,
		{ ss.str() });
		free((yyvsp[-6])); free((yyvsp[-1]));
	}
#line 1560 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 27:
#line 200 "grammar.y" /* yacc.c:1646  */
	{
		stringstream ss;
		ss << "True if filesize is not equal to " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode().c_str();
		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-6])))->toCode(), VAR_TYPE::text,
		{ ss.str() });
		free((yyvsp[-6])); free((yyvsp[-1]));
	}
#line 1572 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 28:
#line 209 "grammar.y" /* yacc.c:1646  */
	{
		stringstream ss;
		ss << "True if filesize is greater than " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode().c_str();
		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-5])))->toCode(), VAR_TYPE::text,
		{ ss.str() });
		free((yyvsp[-5])); free((yyvsp[-1]));
	}
#line 1584 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 29:
#line 218 "grammar.y" /* yacc.c:1646  */
	{
		stringstream ss;
		ss << "True if filesize is less than " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode().c_str();
		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-5])))->toCode(), VAR_TYPE::text,
		{ ss.str() });
		free((yyvsp[-5])); free((yyvsp[-1]));
	}
#line 1596 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 30:
#line 227 "grammar.y" /* yacc.c:1646  */
	{
		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-3])))->toCode(), VAR_TYPE::dec,
		{ (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode() });
		free((yyvsp[-3])); free((yyvsp[-1]));
	}
#line 1606 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 31:
#line 234 "grammar.y" /* yacc.c:1646  */
	{
		stringstream strm;
		strm << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-5])))->toCode() << " " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-4])))->toCode()
			<< " " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-3])))->toCode() << " " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-2])))->toCode();
		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-8])))->toCode(), VAR_TYPE::text,
		{ strm.str() });
		free((yyvsp[-8])); free((yyvsp[-5]));  free((yyvsp[-4])); free((yyvsp[-3]));  free((yyvsp[-2]));
	}
#line 1619 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 32:
#line 244 "grammar.y" /* yacc.c:1646  */
	{
		int start1 = strtol((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-13])))->toCode().c_str(), NULL, 16);
		int start2 = strtol((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-12])))->toCode().c_str(), NULL, 16);

		int range_start = strtol((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-9])))->toCode().c_str(), NULL, 16);
		int range_stop = strtol((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-7])))->toCode().c_str(), NULL, 16);

		int stop1 = strtol((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-4])))->toCode().c_str(), NULL, 16);
		int stop2 = strtol((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-2])))->toCode().c_str(), NULL, 16);

		int r_num = range_start + int(((range_stop - range_start) + 1) * rand() / (RAND_MAX + 1.0));

		stringstream strm;
		strm << std::hex << start1 << " " << std::hex << start2 << " ";

		for (int i = 0; i < r_num; i++)
		{
			int new_num = rand() % 255;
			strm << std::hex << new_num << " ";
		}

		strm << std::hex << stop1 << " " << std::hex << stop2 << "\n";
		string rslt(strm.str());

		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-16])))->toCode(), VAR_TYPE::hex,
		{ rslt });
		free((yyvsp[-16])); free((yyvsp[-13])); free((yyvsp[-12])); free((yyvsp[-9])); free((yyvsp[-7])); free((yyvsp[-4])); free((yyvsp[-2]));
	}
#line 1652 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 33:
#line 273 "grammar.y" /* yacc.c:1646  */
	{
		int v1 = strtol((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-10])))->toCode().c_str(), NULL, 16);
		int v2 = strtol((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-9])))->toCode().c_str(), NULL, 16);

		int o1 = strtol((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-7])))->toCode().c_str(), NULL, 16);
		int o2 = strtol((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-6])))->toCode().c_str(), NULL, 16);
		int o3 = strtol((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-4])))->toCode().c_str(), NULL, 16);

		int l1 = strtol((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-2])))->toCode().c_str(), NULL, 16);

		stringstream strm;
		strm << std::hex << v1 << v2 << o1 << o2 << l1;
		string rslt(strm.str());

		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-13])))->toCode(), VAR_TYPE::hex,
		{ rslt });
		free((yyvsp[-13])); free((yyvsp[-10])); free((yyvsp[-9])); free((yyvsp[-7])); free((yyvsp[-6])); free((yyvsp[-4])); free((yyvsp[-2]));
	}
#line 1675 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 34:
#line 292 "grammar.y" /* yacc.c:1646  */
	{
		vector<string> vec = { "Import:" , (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-8])))->toCode() , (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-6])))->toCode(),
			(reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-4])))->toCode() , (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-2])))->toCode() };
		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-12])))->toCode(), VAR_TYPE::text,
		{ (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-8])))->toCode() , (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-6])))->toCode() ,
			(reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-4])))->toCode() , (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-2])))->toCode() });
		free((yyvsp[-12])); free((yyvsp[-8])); free((yyvsp[-6])); free((yyvsp[-4])); free((yyvsp[-2]));
	}
#line 1688 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 35:
#line 301 "grammar.y" /* yacc.c:1646  */
	{
		vector<string> vec = { "Export:" , (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-8])))->toCode() , (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-6])))->toCode(),
			(reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-4])))->toCode() , (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-2])))->toCode() };
		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-12])))->toCode(), VAR_TYPE::text,
		{ (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-8])))->toCode() , (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-6])))->toCode() ,
			(reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-4])))->toCode() , (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-2])))->toCode() });
		free((yyvsp[-12])); free((yyvsp[-8])); free((yyvsp[-6])); free((yyvsp[-4])); free((yyvsp[-2]));
	}
#line 1701 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 36:
#line 312 "grammar.y" /* yacc.c:1646  */
	{
		stringstream strm;
		strm << "exports " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-2])))->toCode().c_str();
		string rslt(strm.str());

		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-6])))->toCode(), VAR_TYPE::text,
		{ rslt });
		free((yyvsp[-6])); free((yyvsp[-2]));
	}
#line 1715 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 37:
#line 323 "grammar.y" /* yacc.c:1646  */
	{
		stringstream strm;
		strm << "exports " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-4])))->toCode().c_str() << " and "
			<< (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-2])))->toCode().c_str();
		string rslt(strm.str());

		DataHolder.addData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-8])))->toCode(), VAR_TYPE::text,
		{ rslt });
		free((yyvsp[-8])); free((yyvsp[-4])); free((yyvsp[-2]));
	}
#line 1730 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 43:
#line 346 "grammar.y" /* yacc.c:1646  */
	{
		printf("call export %s\n", (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-2])))->toCode().c_str());
	}
#line 1738 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 44:
#line 351 "grammar.y" /* yacc.c:1646  */
	{
		printf("call export %s and %s\n", (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-4])))->toCode().c_str(), (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-2])))->toCode().c_str());
	}
#line 1746 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 45:
#line 355 "grammar.y" /* yacc.c:1646  */
	{
		vector<string> var1 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-3])))->toCode());
		if (var1.size() <= 0) {
			cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-3])))->toCode() << endl;
			exit(1);
		}

		vector<string> var2 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode());
		if (var2.size() <= 0) {
			cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode() << endl;
			exit(1);
		}

		for (int j = 0; j < var1.size(); j++)
			cout << var1[j].c_str() << " ";
		cout << " or ";
		for (int j = 0; j < var2.size(); j++)
			cout << var2[j].c_str() << " ";
		cout << " matching " << endl;
		free((yyvsp[-3])); free((yyvsp[-1]));
		DataHolder.clear();
	}
#line 1773 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 46:
#line 377 "grammar.y" /* yacc.c:1646  */
	{
		vector<string> var1 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-3])))->toCode());
		if (var1.size() <= 0) {
			cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-3])))->toCode() << endl;
			exit(1);
		}

		vector<string> var2 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode());
		if (var2.size() <= 0) {
			cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode() << endl;
			exit(1);
		}

		for (int j = 0; j < var1.size(); j++)
			cout << var1[j].c_str() << " ";
		cout << " and ";
		for (int j = 0; j < var2.size(); j++)
			cout << var2[j].c_str() << " ";
		cout << " matching " << endl;
		free((yyvsp[-3])); free((yyvsp[-1]));
		DataHolder.clear();
	}
#line 1800 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 47:
#line 399 "grammar.y" /* yacc.c:1646  */
	{
		vector<string> var1 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-3])))->toCode());
		if (var1.size() <= 0) {
			cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-3])))->toCode() << endl;
			exit(1);
		}

		vector<string> var2 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode());
		if (var2.size() <= 0) {
			cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode() << endl;
			exit(1);
		}

		for (int j = 0; j < var1.size(); j++)
			cout << var1[j].c_str() << " ";
		cout << " not ";
		for (int j = 0; j < var2.size(); j++)
			cout << var2[j].c_str() << " ";
		cout << " matching " << endl;
		free((yyvsp[-3])); free((yyvsp[-1]));
		DataHolder.clear();
	}
#line 1827 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 48:
#line 422 "grammar.y" /* yacc.c:1646  */
	{
		vector<string> var1 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-3])))->toCode());
		if (var1.size() <= 0) {
			cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-3])))->toCode() << endl;
			exit(1);
		}
		printf("%s at %s \n", var1[0].c_str(), (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode().c_str());
		free((yyvsp[-3])); free((yyvsp[-1]));
		DataHolder.clear();
	}
#line 1842 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 49:
#line 433 "grammar.y" /* yacc.c:1646  */
	{
		vector<string> var1 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-3])))->toCode());
		if (var1.size() <= 0) {
			cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-3])))->toCode() << endl;
			exit(1);
		}
		printf("%s at %s \n", var1[0].c_str(), (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-1])))->toCode().c_str());
		free((yyvsp[-3])); free((yyvsp[-1]));
		DataHolder.clear();
	}
#line 1857 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 50:
#line 444 "grammar.y" /* yacc.c:1646  */
	{
		vector<string> var1 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-4])))->toCode());
		if (var1.size() <= 0) {
			cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-4])))->toCode() << endl;
			exit(1);
		}
		printf("if %s occurs %s times\n", var1[0].c_str(), (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[-2])))->toCode().c_str());
		free((yyvsp[-4])); free((yyvsp[-2]));
		DataHolder.clear();
	}
#line 1872 "grammar.tab.c" /* yacc.c:1646  */
	break;

	case 51:
#line 454 "grammar.y" /* yacc.c:1646  */
	{
		vector<string> var1 = DataHolder.getData((reinterpret_cast<compiler::SyntaxTree *>((yyvsp[0])))->toCode());
		if (var1.size() <= 0) {
			cout << "Error : undeclared variable " << (reinterpret_cast<compiler::SyntaxTree *>((yyvsp[0])))->toCode() << endl;
			exit(1);
		}

		for (int i{ 0 }; i < var1.size(); i++)
		{
			string outpt_uncen = string(var1[i].c_str());
			stringstream ss;

			for (int i{ 0 }; i < (outpt_uncen.length() - 1); i++)
			{
				if (outpt_uncen[i] == '\\')
				{
					if (outpt_uncen[i + 1] == 'n')
					{
						ss << '\n'; i += 1;
					}
					else if (outpt_uncen[i + 1] == 't')
					{
						ss << '\t'; i += 1;
					}
					else if (outpt_uncen[i + 1] == '\\')
					{
						ss << '\\'; i += 1;
					}
				}
				else
					ss << outpt_uncen[i];
			}

			if ((outpt_uncen[outpt_uncen.length() - 1] != 'n' || outpt_uncen[outpt_uncen.length() - 1] != 't' || outpt_uncen[outpt_uncen.length() - 1] != '\\')
				&& outpt_uncen[outpt_uncen.length() - 2] != '\\')
				ss << outpt_uncen[outpt_uncen.length() - 1];

			string outpt(ss.str());
			cout << outpt << " ";
		}
		cout << endl;
		free((yyvsp[0]));
	}
#line 1914 "grammar.tab.c" /* yacc.c:1646  */
	break;


#line 1918 "grammar.tab.c" /* yacc.c:1646  */
	default: break;
	}
	/* User semantic actions sometimes alter yychar, and that requires
	that yytoken be updated with the new translation.  We take the
	approach of translating immediately before every use of yytoken.
	One alternative is translating here after every semantic action,
	but that translation would be missed if the semantic action invokes
	YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
	if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
	incorrect destructor might then be invoked immediately.  In the
	case of YYERROR or YYBACKUP, subsequent parser actions might lead
	to an incorrect destructor call or verbose syntax error message
	before the lookahead is translated.  */
	YY_SYMBOL_PRINT("-> $$ =", yyr1[yyn], &yyval, &yyloc);

	YYPOPSTACK(yylen);
	yylen = 0;
	YY_STACK_PRINT(yyss, yyssp);

	*++yyvsp = yyval;

	/* Now 'shift' the result of the reduction.  Determine what state
	that goes to, based on the state we popped back to and the rule
	number reduced by.  */

	yyn = yyr1[yyn];

	yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
	if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
		yystate = yytable[yystate];
	else
		yystate = yydefgoto[yyn - YYNTOKENS];

	goto yynewstate;


	/*--------------------------------------.
	| yyerrlab -- here on detecting error.  |
	`--------------------------------------*/
yyerrlab:
	/* Make sure we have latest lookahead translation.  See comments at
	user semantic actions for why this is necessary.  */
	yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE(yychar);

	/* If not already recovering from an error, report this error.  */
	if (!yyerrstatus)
	{
		++yynerrs;
#if ! YYERROR_VERBOSE
		yyerror(YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
		{
			char const *yymsgp = YY_("syntax error");
			int yysyntax_error_status;
			yysyntax_error_status = YYSYNTAX_ERROR;
			if (yysyntax_error_status == 0)
				yymsgp = yymsg;
			else if (yysyntax_error_status == 1)
			{
				if (yymsg != yymsgbuf)
					YYSTACK_FREE(yymsg);
				yymsg = (char *)YYSTACK_ALLOC(yymsg_alloc);
				if (!yymsg)
				{
					yymsg = yymsgbuf;
					yymsg_alloc = sizeof yymsgbuf;
					yysyntax_error_status = 2;
				}
				else
				{
					yysyntax_error_status = YYSYNTAX_ERROR;
					yymsgp = yymsg;
				}
			}
			yyerror(yymsgp);
			if (yysyntax_error_status == 2)
				goto yyexhaustedlab;
		}
# undef YYSYNTAX_ERROR
#endif
	}



	if (yyerrstatus == 3)
	{
		/* If just tried and failed to reuse lookahead token after an
		error, discard it.  */

		if (yychar <= YYEOF)
		{
			/* Return failure if at end of input.  */
			if (yychar == YYEOF)
				YYABORT;
		}
		else
		{
			yydestruct("Error: discarding",
				yytoken, &yylval);
			yychar = YYEMPTY;
		}
	}

	/* Else will try to reuse lookahead token after shifting the error
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

	/* Do not reclaim the symbols of the rule whose action triggered
	this YYERROR.  */
	YYPOPSTACK(yylen);
	yylen = 0;
	YY_STACK_PRINT(yyss, yyssp);
	yystate = *yyssp;
	goto yyerrlab1;


	/*-------------------------------------------------------------.
	| yyerrlab1 -- common code for both syntax error and YYERROR.  |
	`-------------------------------------------------------------*/
yyerrlab1:
	yyerrstatus = 3;      /* Each real token shifted decrements this.  */

	for (;;)
	{
		yyn = yypact[yystate];
		if (!yypact_value_is_default(yyn))
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


		yydestruct("Error: popping",
			yystos[yystate], yyvsp);
		YYPOPSTACK(1);
		yystate = *yyssp;
		YY_STACK_PRINT(yyss, yyssp);
	}

	YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
		*++yyvsp = yylval;
	YY_IGNORE_MAYBE_UNINITIALIZED_END


		/* Shift the error token.  */
		YY_SYMBOL_PRINT("Shifting", yystos[yyn], yyvsp, yylsp);

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

#if !defined yyoverflow || YYERROR_VERBOSE
	/*-------------------------------------------------.
	| yyexhaustedlab -- memory exhaustion comes here.  |
	`-------------------------------------------------*/
	yyexhaustedlab :
				   yyerror(YY_("memory exhausted"));
				   yyresult = 2;
				   /* Fall through.  */
#endif

			   yyreturn:
				   if (yychar != YYEMPTY)
				   {
					   /* Make sure we have latest lookahead translation.  See comments at
					   user semantic actions for why this is necessary.  */
					   yytoken = YYTRANSLATE(yychar);
					   yydestruct("Cleanup: discarding lookahead",
						   yytoken, &yylval);
				   }
				   /* Do not reclaim the symbols of the rule whose action triggered
				   this YYABORT or YYACCEPT.  */
				   YYPOPSTACK(yylen);
				   YY_STACK_PRINT(yyss, yyssp);
				   while (yyssp != yyss)
				   {
					   yydestruct("Cleanup: popping",
						   yystos[*yyssp], yyvsp);
					   YYPOPSTACK(1);
				   }
#ifndef yyoverflow
				   if (yyss != yyssa)
					   YYSTACK_FREE(yyss);
#endif
#if YYERROR_VERBOSE
				   if (yymsg != yymsgbuf)
					   YYSTACK_FREE(yymsg);
#endif
				   return yyresult;
}
#line 494 "grammar.y" /* yacc.c:1906  */


std::unique_ptr<compiler::SyntaxTree> root;
void yyerror(char const *x)
{
	printf("Error %s\n", x);
}

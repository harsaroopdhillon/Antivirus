%option noyywrap
%{
  #include <string>
  #include "grammer.tab.h"
  #include "data.h"
  using namespace std;
%}

alpha          [a-zA-Z]
var_beg        $
var           {var_beg}{alpha}([0-9]*{alpha}*)*
text           {alpha}([0-9]*{alpha}*)*
whitespace     [ \r\t\v\f]
linefeed       \n
number        [0-9a-fA-F]+
hex_start     0x
hex           {hex_start}{number}

%%
"//".*
"/*"[^*]*|[*]*"*/"    { }
{hex}                 { yylval = reinterpret_cast<YYSTYPE>(new compiler::Data(yytext)); return HEX;}
{number}              { yylval = reinterpret_cast<YYSTYPE>(new compiler::Data(yytext)); return NUM; }
\"([^\\\"]|\\.)*\"    { yylval = reinterpret_cast<YYSTYPE>(new compiler::Data(yytext)); return STR; }
{var}                 { yylval = reinterpret_cast<YYSTYPE>(new compiler::Data(yytext)); return VAR; }
"/md5:".*             { yylval = reinterpret_cast<YYSTYPE>(new compiler::Data(yytext)); return REG; }
"filename"            { return FNAME; }
"filesize"            { return FSIZE; }
"Signature"           { return SIG; }
"insensitive"         { return INSENSITIVE; }
"wide"                { return WIDE; }
"ascii"               { return ASCII; }
"nonalpha"            { return NONALPHA; }
"info:"               { return INFO; }
"pe.imports"          { return IMPORT; }
"pe.exports"          { return EXPORT; }
"pe.call"             { return CALL;  }
"types:"              { return TYPES; }
"check:"              { return CHECK; }
" or"                 { return OR; }
"at"                  { return AT; }
"and"                 { return AND;}
"not"                 { return NNOT; }
"*"                   { return STAR; }
{text}                { yylval = reinterpret_cast<YYSTYPE>(new compiler::Data(yytext)); return TXT; }
":"                   { return COLON; }
"-"                   { return TO; }
","                   { return COMMA; }
"{"                   { return LEFT_BRACE; }
"}"                   { return RIGHT_BRACE; }
"["                   { return LSQR; }
"]"                   { return RSQR; }
"("                   { return LPRN; }
")"                   { return RPRN; }
"!"                   { return NOT; }
">"                   { return GTR; }
"<"                   { return LSS; }
[=]                   { return EQL; }
[;]                   { return EOL; }
<<EOF>>               { return EOF_F; }
[|]                   { return OR; }
{whitespace}
{linefeed}            ++yylineno;
.                     {}
%%

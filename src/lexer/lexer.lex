%{
#include "lexer.h"
#include <string.h>

#define ERR_NO_ERROR 0
#define ERR_ILLEGAL_INPUT 1
#define ERR_UNTERMINATED_STRING 2
#define ERR_EOF_IN_COMMENT 3
#define ERR_INTEGER_TOO_LARGE 4

int yycolno = 1;
int yycolno_next = 1;
int yyerrno = 0;

char* YYERRMSG[] = {
    "No error",
    "Illegal input",
    "Unterminated string",
    "EOF in comment",
    "Integer too large"
};

union YYSTYPE yylval;

int cmt_level = 0;

#define YY_USER_ACTION {yycolno = yycolno_next; yycolno_next += yyleng;}

%}

%x COMMENT
%x STRING
%option yylineno

whitespace       [ \t]*
digit             [0-9]
alpha             [A-Za-z_]
alpha_num         ({alpha}|{digit})
identifier        {alpha}{alpha_num}*
unsigned_integer  {digit}+
exponent          e[+-]?{digit}+
u                 {unsigned_integer}
real              ({u}\.{u}?|{u}?\.{u}){exponent}?

%%

"{"                    {BEGIN(COMMENT); cmt_level = 1;}
<COMMENT>"{"           {++cmt_level;}
<COMMENT>\n            {yycolno_next = 1;}
<COMMENT><<EOF>>       {yyerrno = ERR_EOF_IN_COMMENT; BEGIN(INITIAL); return TOK_ERROR;}
<COMMENT>"}"           {--cmt_level; if (cmt_level == 0) BEGIN(INITIAL);}
<COMMENT>.            

"'"                {BEGIN(STRING); yylval.strval[0] = '\0';}
<STRING>\n         {yycolno_next = 1; yyerrno = ERR_UNTERMINATED_STRING; BEGIN(INITIAL); return TOK_ERROR;}
<STRING><<EOF>>    {yyerrno = ERR_UNTERMINATED_STRING; BEGIN(INITIAL); return TOK_ERROR;}
<STRING>[^\\']     {strcat(yylval.strval, yytext);}
<STRING>\\n        {strcat(yylval.strval, "\n");}
<STRING>\\t        {strcat(yylval.strval, "\t");}
<STRING>\\r        {strcat(yylval.strval, "\r");}
<STRING>\\'        {strcat(yylval.strval, "'");}
<STRING>\\.        {strcat(yylval.strval, yytext);}
<STRING>"'"        {BEGIN(INITIAL); return TOK_STRING;}

and                {return TOK_AND;}
array              {return TOK_ARRAY;}
begin              {return TOK_BEGIN;}
case               {return TOK_CASE;}
const              {return TOK_CONST;}
div                {return TOK_DIV;}
do                 {return TOK_DO;}
downto             {return TOK_DOWNTO;}
else               {return TOK_ELSE;}
end                {return TOK_END;}
file               {return TOK_FILE;}
for                {return TOK_FOR;}
function           {return TOK_FUNCTION;}
goto               {return TOK_GOTO;}
if                 {return TOK_IF;}
in                 {return TOK_IN;}
label              {return TOK_LABEL;}
mod                {return TOK_MOD;}
nil                {return TOK_NIL;}
not                {return TOK_NOT;}
of                 {return TOK_OF;}
or                 {return TOK_OR;}
packed             {return TOK_PACKED;}
procedure          {return TOK_PROCEDURE;}
program            {return TOK_PROGRAM;}
record             {return TOK_RECORD;}
repeat             {return TOK_REPEAT;}
set                {return TOK_SET;}
then               {return TOK_THEN;}
to                 {return TOK_TO;}
type               {return TOK_TYPE;}
until              {return TOK_UNTIL;}
var                {return TOK_VAR;}
while              {return TOK_WHILE;}
with               {return TOK_WITH;}

{identifier}         {return TOK_ID;}
{unsigned_integer}   {
    if (strlen(yytext) > MAX_INT_LEN) {
        yyerrno = ERR_INTEGER_TOO_LARGE;
        return TOK_ERROR;
    }
    yylval.intval = strtoul(yytext, NULL, 10);
    return TOK_INTEGER;
}
{real}               {
    yylval.realval = strtod(yytext, NULL);
    return TOK_REAL;
}

[+\-*/=<>\[\]\.,:;^()]   {return yytext[0];}
"<>"                {return TOK_NEQOP;}
"<="                {return TOK_LEOP;}
">="                {return TOK_GEOP;}
":="                {return TOK_ASSIGNOP;}
".."                {return TOK_DOTDOT;}

{whitespace}        
\n                  {yycolno_next = 1;}
.                   {yyerrno = ERR_ILLEGAL_INPUT; return TOK_ERROR;}

%%

int yywrap()
{
    return 1;
}

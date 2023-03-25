#include <stdio.h>
#include "lexer.h"

int main(int argc, char** argv)
{
    int token;
    while ((token = yylex()) != 0) {
        printf("Token: %d (%s) at line %d, column %d\n", token, yytext, yylineno, yycolno);
        if (token == TOK_STRING) {
            printf("String value: %s\n", yylval.strval);
        }
        if (token == TOK_INTEGER) {
            printf("Integer value: %d\n", yylval.intval);
        }
        if (token == TOK_REAL) {
            printf("Real value: %f\n", yylval.realval);
        }
        if (token == TOK_ERROR) {
            printf("!!!(%s)", YYERRMSG[yyerrno]);
        }
    }
    return 0;
}

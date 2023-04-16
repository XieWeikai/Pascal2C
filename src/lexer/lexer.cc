#include <cstdio>

#include "lexer.h"

int lexer() {
    int token;
    while ((token = yylex()) != 0) {
        printf("%s(%d) at line %d, column %d.", TokenToString(token), token,
               yylineno, yycolno);
        switch (token) {
            case TOK_ID:
                printf(" Value: %s\n", yytext);
                break;
            case TOK_INTEGER:
                printf(" Value: %d\n", yylval.intval);
                break;
            case TOK_REAL:
                printf(" Value: %f\n", yylval.realval);
                break;
            case TOK_STRING:
                printf(" Value: %s\n", yylval.strval);
                break;
            case TOK_ERROR:
                printf(" Value: %s\n", YYERRMSG[yyerrno]);
                break;
            default:
                printf("\n");
                break;
        }
    }
    return 0;
}

#include "lexer.h"
#include <stdio.h>

#define TOK_EOF 0

void yyreset(FILE* in) {
    yylineno = 1;
    yycolno = 1;
    yycolno_next = 1;
    yyerrno = 0;
    yyin = in;
}

char* TokenToString(int token) {
    static char *tokenNames[] = {
        [TOK_ID] = "TOK_ID",
        [TOK_INTEGER] = "TOK_INTEGER",
        [TOK_REAL] = "TOK_REAL",
        [TOK_STRING] = "TOK_STRING",
        [TOK_ERROR] = "TOK_ERROR",
        [TOK_AND] = "TOK_AND",
        [TOK_ARRAY] = "TOK_ARRAY",
        [TOK_BEGIN] = "TOK_BEGIN",
        [TOK_CASE] = "TOK_CASE",
        [TOK_CONST] = "TOK_CONST",
        [TOK_DIV] = "TOK_DIV",
        [TOK_DO] = "TOK_DO",
        [TOK_DOWNTO] = "TOK_DOWNTO",
        [TOK_ELSE] = "TOK_ELSE",
        [TOK_END] = "TOK_END",
        [TOK_FILE] = "TOK_FILE",
        [TOK_FOR] = "TOK_FOR",
        [TOK_FUNCTION] = "TOK_FUNCTION",
        [TOK_GOTO] = "TOK_GOTO",
        [TOK_IF] = "TOK_IF",
        [TOK_IN] = "TOK_IN",
        [TOK_LABEL] = "TOK_LABEL",
        [TOK_MOD] = "TOK_MOD",
        [TOK_NIL] = "TOK_NIL",
        [TOK_NOT] = "TOK_NOT",
        [TOK_OF] = "TOK_OF",
        [TOK_OR] = "TOK_OR",
        [TOK_PACKED] = "TOK_PACKED",
        [TOK_PROCEDURE] = "TOK_PROCEDURE",
        [TOK_PROGRAM] = "TOK_PROGRAM",
        [TOK_RECORD] = "TOK_RECORD",
        [TOK_REPEAT] = "TOK_REPEAT",
        [TOK_SET] = "TOK_SET",
        [TOK_THEN] = "TOK_THEN",
        [TOK_TO] = "TOK_TO",
        [TOK_TYPE] = "TOK_TYPE",
        [TOK_UNTIL] = "TOK_UNTIL",
        [TOK_VAR] = "TOK_VAR",
        [TOK_WHILE] = "TOK_WHILE",
        [TOK_WITH] = "TOK_WITH",
        [TOK_INTEGER_TYPE] = "TOK_INTEGER_TYPE",
        [TOK_REAL_TYPE] = "TOK_REAL_TYPE",
        [TOK_BOOLEAN_TYPE] = "TOK_BOOLEAN_TYPE",
        [TOK_CHAR_TYPE] = "TOK_CHAR_TYPE",
        [TOK_STRING_TYPE] = "TOK_STRING_TYPE",
        [TOK_TRUE] = "TOK_TRUE",
        [TOK_FALSE] = "TOK_FALSE",
        ['+'] = "TOK_+",
        ['-'] = "TOK_-",
        ['*'] = "TOK_*",
        ['/'] = "TOK_/",
        ['='] = "TOK_=",
        ['<'] = "TOK_<",
        ['>'] = "TOK_>",
        ['['] = "TOK_[",
        [']'] = "TOK_]",
        ['.'] = "TOK_.",
        [','] = "TOK_,",
        [':'] = "TOK_:",
        [';'] = "TOK_;",
        ['^'] = "TOK_^",
        ['('] = "TOK_(",
        [')'] = "TOK_)",
        [TOK_EOF] = "end of input",
        [TOK_NEQOP] = "TOK_NEQOP",
        [TOK_LEOP] = "TOK_LEOP",
        [TOK_GEOP] = "TOK_GEOP",
        [TOK_ASSIGNOP] = "TOK_ASSIGNOP",
        [TOK_DOTDOT] = "TOK_DOTDOT",
    };
    return tokenNames[token];
}

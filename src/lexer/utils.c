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
        [TOK_ID] = "'id'",
        [TOK_INTEGER] = "'integer'",
        [TOK_REAL] = "'real'",
        [TOK_STRING] = "'string'",
        [TOK_ERROR] = "'error'",
        [TOK_AND] = "'and'",
        [TOK_ARRAY] = "'array'",
        [TOK_BEGIN] = "'begin'",
        [TOK_CASE] = "'case'",
        [TOK_CONST] = "'const'",
        [TOK_DIV] = "'div'",
        [TOK_DO] = "'do'",
        [TOK_DOWNTO] = "'downto'",
        [TOK_ELSE] = "'else'",
        [TOK_END] = "'end'",
        [TOK_FILE] = "'file'",
        [TOK_FOR] = "'for'",
        [TOK_FUNCTION] = "'function'",
        [TOK_GOTO] = "'goto'",
        [TOK_IF] = "'if'",
        [TOK_IN] = "'in'",
        [TOK_LABEL] = "'label'",
        [TOK_MOD] = "'mod'",
        [TOK_NIL] = "'nil'",
        [TOK_NOT] = "'not'",
        [TOK_OF] = "'of'",
        [TOK_OR] = "'or'",
        [TOK_PACKED] = "'packed'",
        [TOK_PROCEDURE] = "'procedure'",
        [TOK_PROGRAM] = "'program'",
        [TOK_RECORD] = "'record'",
        [TOK_REPEAT] = "'repeat'",
        [TOK_SET] = "'set'",
        [TOK_THEN] = "'then'",
        [TOK_TO] = "'to'",
        [TOK_TYPE] = "'type'",
        [TOK_UNTIL] = "'until'",
        [TOK_VAR] = "'var'",
        [TOK_WHILE] = "'while'",
        [TOK_WITH] = "'with'",
        [TOK_INTEGER_TYPE] = "'integer_type'",
        [TOK_REAL_TYPE] = "'real_type'",
        [TOK_BOOLEAN_TYPE] = "'boolean_type'",
        [TOK_CHAR_TYPE] = "'char_type'",
        [TOK_STRING_TYPE] = "'string_type'",
        [TOK_TRUE] = "'true'",
        [TOK_FALSE] = "'false'",
        ['+'] = "'+'",
        ['-'] = "'-'",
        ['*'] = "'*'",
        ['/'] = "'/'",
        ['='] = "'='",
        ['<'] = "'<'",
        ['>'] = "'>'",
        ['['] = "'['",
        [']'] = "']'",
        ['.'] = "'.'",
        [','] = "','",
        [':'] = "':'",
        [';'] = "';'",
        ['^'] = "'^'",
        ['('] = "'('",
        [')'] = "')'",
        [TOK_NEQOP] = "'<>'",
        [TOK_LEOP] = "'<='",
        [TOK_GEOP] = "'>='",
        [TOK_ASSIGNOP] = "':='",
        [TOK_DOTDOT] = "'..'",
        [TOK_EOF] = "end of file",
    };
    return tokenNames[token];
}

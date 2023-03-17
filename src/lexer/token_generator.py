tok_list = [
    'ID', 'INTEGER', 'REAL', 'STRING', 'ERROR',
    # KEYWORDS
    'AND', 'ARRAY', 'BEGIN', 'CASE', 'CONST', 'DIV', 'DO', 'DOWNTO', 'ELSE', 'END', 'FILE', 'FOR', 'FUNCTION', 'GOTO', 'IF', 'IN', 'LABEL', 'MOD', 'NIL', 'NOT', 'OF', 'OR', 'PACKED', 'PROCEDURE', 'PROGRAM', 'RECORD', 'REPEAT', 'SET', 'THEN', 'TO', 'TYPE', 'UNTIL', 'VAR', 'WHILE', 'WITH',
    'INTEGER_TYPE', 'REAL_TYPE', 'BOOLEAN_TYPE', 'CHAR_TYPE', 'STRING_TYPE', 'TRUE', 'FALSE',
    # OPERATORS
    'NEQOP', 'LEOP', 'GEOP', 'ASSIGNOP', 'DOTDOT',
]

PREFIX = 'TOK_'
START = 257
FILE = 'lexer.h'
OTHER_CONTENT = '''#pragma once

#include <stdint.h>
#include <stdio.h>

#define MAX_STR_LEN 256
#define MAX_INT_LEN 32

union YYSTYPE {
    uint32_t intval;
    double realval;
    char strval[MAX_STR_LEN];
};

int yylex();
char* TokenToString(int token);
void yyreset(FILE *in);
extern int yylineno;
extern int yycolno;
extern int yyerrno;
extern char* YYERRMSG[];
extern char* yytext;
extern FILE *yyin;
extern union YYSTYPE yylval;
'''

with open(FILE, 'w') as f:
    f.write(OTHER_CONTENT)
    for i, tok in enumerate(tok_list, START):
        f.write(f'#define {PREFIX}{tok} {i}\n')

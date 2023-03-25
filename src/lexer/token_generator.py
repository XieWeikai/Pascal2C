tok_list = [
    'ID', 'INTEGER', 'REAL', 'STRING', 'ERROR',
    # KEYWORDS
    'AND', 'ARRAY', 'BEGIN', 'CASE', 'CONST', 'DIV', 'DO', 'DOWNTO', 'ELSE', 'END', 'FILE', 'FOR', 'FUNCTION', 'GOTO', 'IF', 'IN', 'LABEL', 'MOD', 'NIL', 'NOT', 'OF', 'OR', 'PACKED', 'PROCEDURE', 'PROGRAM', 'RECORD', 'REPEAT', 'SET', 'THEN', 'TO', 'TYPE', 'UNTIL', 'VAR', 'WHILE', 'WITH',
    # OPERATORS
    'NEQOP', 'LEOP', 'GEOP', 'ASSIGNOP', 'DOTDOT',
]

PREFIX = 'TOK_'
START = 257
FILE = 'lexer.h'
OTHER_CONTENT = '''#pragma once

#include <stdint.h>

#define MAX_STR_LEN 256
#define MAX_INT_LEN 32

union YYSTYPE {
    uint32_t intval;
    double realval;
    char strval[MAX_STR_LEN];
};

int yylex();
int yylineno;
int yycolno;
int yyerrno;
extern char* YYERRMSG[];
extern char* yytext;
extern union YYSTYPE yylval;
'''

with open(FILE, 'w') as f:
    f.write(OTHER_CONTENT)
    for i, tok in enumerate(tok_list, START):
        f.write(f'#define {PREFIX}{tok} {i}\n')

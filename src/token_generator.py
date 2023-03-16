tok_list = [
    'ID', 'INTEGER', 'REAL', 'ERROR'
    # KEYWORDS
    'AND', 'ARRAY', 'BEGIN', 'CASE', 'CONST', 'DIV', 'DO', 'DOWNTO', 'ELSE', 'END', 'FILE', 'FOR', 'FUNCTION', 'GOTO', 'IF', 'IN', 'LABEL', 'MOD', 'NIL', 'NOT', 'OF', 'OR', 'PACKED', 'PROCEDURE', 'PROGRAM', 'RECORD', 'REPEAT', 'SET', 'THEN', 'TO', 'TYPE', 'UNTIL', 'VAR', 'WHILE', 'WITH',
    # OPERATORS
    'NEQOP', 'LEOP', 'GEOP', 'ASSIGNOP', 'DOTDOT',
]

PREFIX = 'TOK_'
START = 257
FILE = 'token.h'
OTHER_CONTENT = '''#pragma once

#include <stdint.h>

union {
    uint32_t intval;
    double realval;
} yyval;

int yylineno;
int yycolno;
int yyerrno;
char **YYERRMSG;
'''

with open(FILE, 'w') as f:
    f.write(OTHER_CONTENT)
    for i, tok in enumerate(tok_list, START):
        f.write(f'#define {PREFIX}{tok} {i}\n')

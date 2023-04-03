#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "gtest/gtest.h"
extern "C" {
    #include "lexer.h"
}

using namespace std;

static void RunTest(const string &input, int *expected_tokens, YYSTYPE *expected_vals,
                    int *expected_lines, int *expected_columns) {
    FILE *file = fmemopen((void *)input.c_str(), input.size(), "r");
    yyreset(file);

    int i = 0;
    int token;
    while ((token = yylex()) != 0) {
        EXPECT_EQ(token, expected_tokens[i]);
        if (token == TOK_ID) {
            EXPECT_STREQ(yytext, expected_vals[i].strval);
        } else if (token == TOK_STRING) {
            EXPECT_STREQ(yylval.strval, expected_vals[i].strval);
        }
        EXPECT_EQ(yylineno, expected_lines[i]);
        EXPECT_EQ(yycolno, expected_columns[i]);
        i++;
    }
    fclose(file);
}

TEST(LexerSimpleTest, SimpleTest) {
    string input = R"(1 2 3 4
5 6 7 8
)";
    int expected_tokens[] = {
        TOK_INTEGER, TOK_INTEGER, TOK_INTEGER, TOK_INTEGER,
        TOK_INTEGER, TOK_INTEGER, TOK_INTEGER, TOK_INTEGER,
    };
    int expected_vals[] = {
        1, 2, 3, 4, 5, 6, 7, 8,
    };
    int expected_lines[] = {
        1, 1, 1, 1, 2, 2, 2, 2,
    };
    int expected_columns[] = {
        1, 3, 5, 7, 1, 3, 5, 7,
    };

    RunTest(input, expected_tokens, (YYSTYPE *)expected_vals,
            expected_lines, expected_columns);
}

TEST(LexerSimpleTest, HelloWorld) {
    string input = R"(program hello;
begin
    write('Hello, world!\n');
end.
)";
    int expected_tokens[] = {
        TOK_PROGRAM, TOK_ID, ';',
        TOK_BEGIN, TOK_ID, '(', TOK_STRING, ')',';',
        TOK_END, '.'
    };
    YYSTYPE expected_vals[11] = {{0}};
    strcpy(expected_vals[1].strval, "hello");
    strcpy(expected_vals[4].strval, "write");
    strcpy(expected_vals[6].strval, "Hello, world!\n");

    int expected_lines[] = {
        1, 1, 1,
        2, 3, 3, 3, 3, 3,
        4, 4,
    };
    int expected_columns[] = {
        1, 9, 14,
        1, 5, 10, 11, 28, 29,
        1, 4,
    };

    RunTest(input, expected_tokens, expected_vals,
            expected_lines, expected_columns);
}
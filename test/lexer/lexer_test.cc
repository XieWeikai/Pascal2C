#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "gtest/gtest.h"
extern "C" {
    #include "lexer.h"
}

using namespace std;

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

    FILE *file = fmemopen((void *)input.c_str(), input.size(), "r");
    SetInput(file);

    int i = 0;
    int token;
    while ((token = yylex()) != 0) {
        EXPECT_EQ(token, expected_tokens[i]);
        EXPECT_EQ(yylval.intval, expected_vals[i]);
        EXPECT_EQ(yylineno, expected_lines[i]);
        EXPECT_EQ(yycolno, expected_columns[i]);
        i++;
    }

    fclose(file);
}

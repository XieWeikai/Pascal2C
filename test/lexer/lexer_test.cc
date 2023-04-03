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

TEST(LexerCommentTest, SimpleComment) {
    string input = R"(begin{comment here}begin
)";
    int expected_tokens[] = {
            TOK_BEGIN, TOK_BEGIN,
    };
    YYSTYPE expected_vals[2] = {{0}};

    int expected_lines[] = {
             1, 1,
    };
    int expected_columns[] = {
             1, 20,
    };

    RunTest(input, expected_tokens, expected_vals,
            expected_lines, expected_columns);
}

TEST(LexerCommentTest, NestedComment) {
    string input = R"(begin{comment here{comment here}}begin
)";
    int expected_tokens[] = {
            TOK_BEGIN, TOK_BEGIN,
    };
    YYSTYPE expected_vals[2] = {{0}};

    int expected_lines[] = {
             1, 1,
    };
    int expected_columns[] = {
             1, 34,
    };

    RunTest(input, expected_tokens, expected_vals,
            expected_lines, expected_columns);
}

TEST(LexerCommentTest, ComplexComment) {
    string input = R"({{{{}}}}
begin {begin{end
}}             begin  {begin
}
begin
)";
    int expected_tokens[] = {
            TOK_BEGIN, TOK_BEGIN, TOK_BEGIN,
    };
    YYSTYPE expected_vals[2] = {{0}};

    int expected_lines[] = {
             2, 3, 5,
    };
    int expected_columns[] = {
             1, 16, 1,
    };

    RunTest(input, expected_tokens, expected_vals,
            expected_lines, expected_columns);
}

TEST(LexerIdentifierTest, VaildIdentifier) {
    string input = R"(begin
    var a, a1, _a1b2c3d4e5f6g7h8i9j0k1l2m3n4o5p6q_: integer;
    writeln('Valid identifiers');
end.
)";
    int expected_tokens[] = {
            TOK_BEGIN,
            TOK_VAR, TOK_ID, ',', TOK_ID, ',', TOK_ID, ':', TOK_INTEGER_TYPE, ';',
            TOK_ID, '(', TOK_STRING, ')', ';',
            TOK_END, '.',
    };
    YYSTYPE expected_vals[17] = {{0}};
    strcpy(expected_vals[2].strval, "a");
    strcpy(expected_vals[4].strval, "a1");
    strcpy(expected_vals[6].strval, "_a1b2c3d4e5f6g7h8i9j0k1l2m3n4o5p6q_");
    strcpy(expected_vals[10].strval, "writeln");
    strcpy(expected_vals[12].strval, "Valid identifiers");

    int expected_lines[] = {
            1,
            2, 2, 2, 2, 2, 2, 2, 2, 2,
            3, 3, 3, 3, 3,
            4, 4,
    };
    int expected_columns[] = {
            1,
            5, 9, 10, 12, 14, 16, 51, 53, 60,
            5, 12, 13, 32, 33,
            1, 4,
    };

    RunTest(input, expected_tokens, expected_vals,
            expected_lines, expected_columns);
}

TEST(LexerIdentifierTest, CaseInsensitiveTest) {
    string input = R"(var Identifier, identifier, IDENTIFIER, IdEnTiFiEr: integer;
)";
    int expected_tokens[] = {
            TOK_VAR, TOK_ID, ',', TOK_ID, ',', TOK_ID, ',', TOK_ID, ':', TOK_INTEGER_TYPE, ';',
    };

    YYSTYPE expected_vals[11] = {{0}};
    strcpy(expected_vals[1].strval, "identifier");
    strcpy(expected_vals[3].strval, "identifier");
    strcpy(expected_vals[5].strval, "identifier");
    strcpy(expected_vals[7].strval, "identifier");

    int expected_lines[] = {
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };

    int expected_columns[] = {
            1, 5, 15, 17, 27, 29, 39, 41, 51, 53, 60
    };

    RunTest(input, expected_tokens, expected_vals,
            expected_lines, expected_columns);
}

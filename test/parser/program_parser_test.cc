#include <iostream>
#include <cstdio>
#include <sstream>
#include <string>
#include <memory>
#include <vector>

#include "gtest/gtest.h"
#include "parser/parser.h"
#include "ast/ast.h"
#include "ast/program.h"

#define OUTPUT_RESULT(parse_function, input_strs)                                 \
    for (int i = 0; i < input_strs.size(); i++)                                   \
    {                                                                             \
        const string &input_str = input_strs[i];                                  \
        FILE *input = fmemopen((void *)input_str.c_str(), input_str.size(), "r"); \
        Parser par(input);                                                        \
        auto expr = par.parse_function();                                         \
        const vector<std::string> &err_messages = par.err_msg();                  \
        for (std::string err_message : err_messages)                              \
        {                                                                         \
            std::cout << err_message << std::endl;                                \
        }                                                                         \
        std::cout << expr->ToString(0) << std::endl                               \
                  << std::endl;                                                   \
    }

#define CHECK_RESULT(parse_function, input_strs, results)                         \
    for (int i = 0; i < input_strs.size(); i++)                                   \
    {                                                                             \
        const string &input_str = input_strs[i];                                  \
        FILE *input = fmemopen((void *)input_str.c_str(), input_str.size(), "r"); \
        Parser par(input);                                                        \
        auto expr = par.parse_function();                                         \
        EXPECT_EQ(expr->ToString(0), results[i]);                                 \
    }

namespace pascal2c::parser
{
    template <typename Tp>
    using vector = ::std::vector<Tp>;
    using string = ::std::string;

    TEST(ProgramParserTest, TestParseProgram)
    {
        const vector<string> input_strs = {
            "program f(a, b); var a, b : integer; begin  end.",
            "program test; var a:integer; begin end.",
            "program test var a: integer; begin end.",
            "program test const a = 1; begin end.",
            "program test procedure p;begin end; begin end.",
            "program test function f : integer ;begin end; begin end.",
            "program test begin end.",
            "program test; begin end"};
        const vector<string>
            results = {};

        OUTPUT_RESULT(ParseProgram, input_strs);
        // CHECK_RESULT(ParseProgram, input_strs, results);
    }

    TEST(ProgramParserTest, TestParseProgramHead)
    {
        const vector<string> input_strs = {
            "program f(a, b)",
            "",
            "f",
            "program",
            "program f",
            "(a,b)",
            "program f(a, b"};
        const vector<string> results = {};

        OUTPUT_RESULT(ParseProgramHead, input_strs);
        // CHECK_RESULT(ParseProgramHead, input_strs, results);
    }

    TEST(ProgramParserTest, TestParseProgramBody)
    {
        const vector<string> input_strs = {
            "const a = 1; b = 2; var c, d : integer; procedure p; begin end; function f(a, b : integer) : integer; begin end; begin end",
            "const a = 1 b = 2; begin end",
            "const a = 1 var a : integer; begin end",
            "const a = 1 procedure p; begin end;begin end",
            "const a = 1 function f : integer; begin end; begin end",
            "const a = 1 begin end",
            "var a : integer b : integer; begin end",
            "var a : integer procedure p; begin end;begin end",
            "var a : integer function f : integer; begin end; begin end",
            "var a : integer begin end",
            "procedure p; begin end function f : integer; begin end; begin end",
            "procedure p; begin end procedure p2;  begin end; begin end",
            "procedure p; begin end  begin end"};
        const vector<string> results = {};

        OUTPUT_RESULT(ParseProgramBody, input_strs);
        // CHECK_RESULT(ParseProgramBody, input_strs, results);
    }

    TEST(ProgramParserTest, TestParseConstDecl)
    {
        const vector<string> input_strs = {
            "a=1",
            "b=2.0",
            "c=-1",
            "d='a'",
            " ",
            "=1",
            "a -1",
            "a 1.1",
            "a 'a'",
            "a 1",
            "a "};

        const vector<string> results = {};

        OUTPUT_RESULT(ParseConstDeclaration, input_strs);
        // CHECK_RESULT(ParseConstDeclaration, input_strs, results);
    }

    TEST(ProgramParserTest, TestParseVarDecl)
    {
        const vector<string> input_strs = {
            "a, b, c : integer",
            "d, e : array [1..10] of integer",
            "a",
            "a,b,c integer",
            "a,b,c array [] of integer",
            "a,b,c real",
            "a,b,c char",
            "a,b,c boolean",
        };
        const vector<string> results = {};

        OUTPUT_RESULT(ParseVarDeclaration, input_strs);
        // CHECK_RESULT(ParseVarDeclaration, input_strs, results);
    }

    TEST(ProgramParserTest, TestParseSubProgramDecl)
    {
        const vector<string> input_strs = {
            "function f(a, b : integer) : integer; begin end",
            "procedure p(var c, d : real); begin end",
            "procedure p const a = 1;",
            "procedure p var a : integer;",
            "procedure p begin end"};
        const vector<string> results = {};

        OUTPUT_RESULT(ParseSubprogram, input_strs);
        // CHECK_RESULT(ParseSubprogram, input_strs, results);
    }

    TEST(ProgramParserTest, TestParseSubProgramHead)
    {
    }
    TEST(ProgramParserTest, TestParseSubProgramBody) {}
    TEST(ProgramParserTest, TestParseIdList) {}
    TEST(ProgramParserTest, TestParseType) {}
    TEST(ProgramParserTest, TestParsePeriod) {}
    TEST(ProgramParserTest, TestParseParameter) {}
}
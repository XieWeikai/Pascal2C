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

namespace pascal2c::parser
{
    template <typename Tp>
    using vector = ::std::vector<Tp>;
    using string = ::std::string;

    TEST(ProgramParserTest, TestParseProgram)
    {

        const vector<string> input_strs = {
            "program f(a, b); var a, b; begin a := 1; b := 2; end.",
            "program test; var a:integer; begin end."
            "program test begin end."
            "program test; begin end"};
        const vector<string>
            results = {};

        for (int i = 0; i < input_strs.size(); i++)
        {
            const string &input_str = input_strs[i];
            const string &result = results[i];
            FILE *input = fmemopen((void *)input_str.c_str(), input_str.size(), "r");
            Parser par(input);
            auto expr = par.ParseProgram();
            // EXPECT_EQ(expr->tostring(0), res[idx++]);
            std::cout << expr->ToString(0) << "\n"
                      << std::endl;
        }
    }
    TEST(ProgramParserTest, TestParseProgramHead)
    {
        const vector<string> input_strs = {
            "program f(a, b)",
            "program f",
            "f",
            "program (a,b)",
            "program f(a, b"};
        const vector<string> results = {};

        for (int i = 0; i < input_strs.size(); i++)
        {
            const string &input_str = input_strs[i];
            const string &result = results[i];
            FILE *input = fmemopen((void *)input_str.c_str(), input_str.size(), "r");
            Parser par(input);
            auto expr = par.ParseProgramHead();
            // EXPECT_EQ(expr->tostring(0), res[idx++]);
            std::cout << expr->ToString(0) << "\n"
                      << std::endl;
        }
    }
    TEST(ProgramParserTest, TestParseProgramBody)
    {
        const vector<string> input_strs = {
            "const a = 1; b = 2; var c, d : integer; procedure p; begin end; function f(a, b : integer) : integer; begin end; begin end"
            "const a = 1 b = 2; begin end"
            "var a b; begin end"
            "procedure p; begin end fuction f; begin end; begin end"
            "const a = 1; b = 2; var c, d : integer; procedure p; begin end;  end"
            "const a = 1; b = 2; var c, d : integer; procedure p; begin end;  begin "};
        const vector<string> results = {};

        for (int i = 0; i < input_strs.size(); i++)
        {
            const string &input_str = input_strs[i];
            const string &result = results[i];
            FILE *input = fmemopen((void *)input_str.c_str(), input_str.size(), "r");
            Parser par(input);
            auto expr = par.ParseProgramBody();
            // EXPECT_EQ(expr->tostring(0), res[idx++]);
            std::cout << expr->ToString(0) << "\n"
                      << std::endl;
        }
    }

    TEST(ProgramParserTest, TestParseConstDecl)
    {
        const vector<string> input_strs = {
            "a=1"
            "b=2.0"
            "c=-1"
            "d='a'"
            "=1",
            "a 1"};
        const vector<string> results = {};

        for (int i = 0; i < input_strs.size(); i++)
        {
            const string &input_str = input_strs[i];
            const string &result = results[i];
            FILE *input = fmemopen((void *)input_str.c_str(), input_str.size(), "r");
            Parser par(input);
            auto expr = par.ParseConstDeclaration();
            // EXPECT_EQ(expr->tostring(0), res[idx++]);
            std::cout << expr->ToString(0) << "\n"
                      << std::endl;
        }
    }

    TEST(ProgramParserTest, TestParseVarDecl)
    {
        const vector<string> input_strs = {
            "a, b, c : integer"
            "d, e : array [1..10] of integer"
            "a,b,c integer"};
        const vector<string> results = {};

        for (int i = 0; i < input_strs.size(); i++)
        {
            const string &input_str = input_strs[i];
            const string &result = results[i];
            FILE *input = fmemopen((void *)input_str.c_str(), input_str.size(), "r");
            Parser par(input);
            auto expr = par.ParseVarDeclaration();
            // EXPECT_EQ(expr->tostring(0), res[idx++]);
            std::cout << expr->ToString(0) << "\n"
                      << std::endl;
        }
    }

    TEST(ProgramParserTest, TestParseSubProgramDecl)
    {
        const vector<string> input_strs = {
            "function f(a, b : integer) : integer; begin end"
            "procedure p(var c, d : real); begin end"
            "procedure p begin end"};
        const vector<string> results = {};

        for (int i = 0; i < input_strs.size(); i++)
        {
            const string &input_str = input_strs[i];
            const string &result = results[i];
            FILE *input = fmemopen((void *)input_str.c_str(), input_str.size(), "r");
            Parser par(input);
            auto expr = par.ParseSubprogram();
            // EXPECT_EQ(expr->tostring(0), res[idx++]);
            std::cout << expr->ToString(0) << "\n"
                      << std::endl;
        }
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
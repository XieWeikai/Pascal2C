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

#define OUTPUT_RESULT(parse_function, input_strs)                                     \
    do                                                                                \
    {                                                                                 \
        vector<string> temp_results;                                                  \
        vector<vector<string>> temp_errs;                                             \
        for (int i = 0; i < input_strs.size(); i++)                                   \
        {                                                                             \
            const string &input_str = input_strs[i];                                  \
            FILE *input = fmemopen((void *)input_str.c_str(), input_str.size(), "r"); \
            Parser par(input);                                                        \
            auto expr = par.parse_function();                                         \
            const vector<std::string> &err_messages = par.err_msg();                  \
            temp_errs.push_back(err_messages);                                        \
            temp_results.push_back(expr->ToString(0));                                \
            fclose(input);                                                            \
        }                                                                             \
        for (int i = 0; i < temp_errs.size(); i++)                                    \
        {                                                                             \
            std::cout << "{";                                                         \
            for (std::string err_message : temp_errs[i])                              \
            {                                                                         \
                std::cout << "\"" << err_message << "\"," << std::endl;               \
            }                                                                         \
            std::cout << "}," << std::endl;                                           \
        }                                                                             \
        for (int i = 0; i < temp_results.size(); i++)                                 \
        {                                                                             \
            std::cout << temp_results[i] << "," << std::endl;                         \
        }                                                                             \
    } while (false)

#define CHECK_RESULT(parse_function, input_strs, results, errs)                   \
    for (int i = 0; i < input_strs.size(); i++)                                   \
    {                                                                             \
        const string &input_str = input_strs[i];                                  \
        FILE *input = fmemopen((void *)input_str.c_str(), input_str.size(), "r"); \
        Parser par(input);                                                        \
        auto expr = par.parse_function();                                         \
        EXPECT_EQ(par.err_msg(), errs[i]);                                        \
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
            "program test; begin end",
        };
        const vector<string>
            results = {
                "1:1 Program: \n"
                "1:1     ProgramHead: f\n"
                "1:11         IdList: a, b\n"
                "1:18     ProgramBody: \n"
                "1:22         VarDeclaration: \n"
                "1:29             Type: integer\n"
                "1:22             IdList: a, b\n"
                "        1:38 CompoundStatement :0",
                "1:1 Program: \n"
                "1:1     ProgramHead: test\n"
                "\n"
                "1:15     ProgramBody: \n"
                "1:19         VarDeclaration: \n"
                "1:21             Type: integer\n"
                "1:19             IdList: a\n"
                "        1:30 CompoundStatement :0",
                "1:1 Program: \n"
                "1:1     ProgramHead: test\n"
                "\n"
                "1:14     ProgramBody: \n"
                "1:18         VarDeclaration: \n"
                "1:21             Type: integer\n"
                "1:18             IdList: a\n"
                "        1:30 CompoundStatement :0",
                "1:1 Program: \n"
                "1:1     ProgramHead: test\n"
                "\n"
                "1:14     ProgramBody: \n"
                "1:20         ConstDeclaration: a\n"
                "            1:24 1\n"
                "        1:27 CompoundStatement :0",
                "1:1 Program: \n"
                "1:1     ProgramHead: test\n"
                "\n"
                "1:14     ProgramBody: \n"
                "1:14         Subprogram: \n"
                "1:14             SubprogramHead: procedure p\n"
                "\n"
                "1:26             SubprogramBody: \n"
                "                1:26 CompoundStatement :0\n"
                "        1:37 CompoundStatement :0",
                "1:1 Program: \n"
                "1:1     ProgramHead: test\n"
                "\n"
                "1:14     ProgramBody: \n"
                "1:14         Subprogram: \n"
                "1:14             SubprogramHead: function f integer\n"
                "\n"
                "1:36             SubprogramBody: \n"
                "                1:36 CompoundStatement :0\n"
                "        1:47 CompoundStatement :0",
                "1:1 Program: \n"
                "1:1     ProgramHead: test\n"
                "\n"
                "1:14     ProgramBody: \n"
                "        1:14 CompoundStatement :0",
                "1:1 Program: \n"
                "1:1     ProgramHead: test\n"
                "\n"
                "1:15     ProgramBody: \n"
                "        1:15 CompoundStatement :0",
            };
        const vector<vector<string>> errs = {
            {},
            {},
            {
                "1:14 syntax err:expected ';' before 'var'",
            },
            {
                "1:14 syntax err:expected ';' before 'const'",
            },
            {
                "1:14 syntax err:expected ';' before 'procedure'",
            },
            {
                "1:14 syntax err:expected ';' before 'function'",
            },
            {
                "1:14 syntax err:expected ';' before 'begin'",
            },
            {
                "1:21 syntax err:expected '.' before end of file",
            },
        };

        // OUTPUT_RESULT(ParseProgram, input_strs);
        CHECK_RESULT(ParseProgram, input_strs, results, errs);
    }

    TEST(ProgramParserTest, TestParseProgramHead)
    {
        const vector<string> input_strs = {
            "program f(a, b)",
            "program f",
            "",
            "f",
            "program",
            "(a,b)",
            "program f(a, b",
        };
        const vector<string> results = {
            "1:1 ProgramHead: f\n"
            "1:11     IdList: a, b",
            "1:1 ProgramHead: f\n"
            "",
            "1:1 ProgramHead: \n"
            "",
            "1:1 ProgramHead: f\n"
            "",
            "1:1 ProgramHead: \n"
            "",
            "1:1 ProgramHead: \n"
            "1:2     IdList: a, b",
            "1:1 ProgramHead: f\n"
            "1:11     IdList: a, b",
        };

        const vector<vector<string>> errs = {
            {},
            {},
            {
                "1:1 syntax err:expected 'program' before end of file",
                "1:1 syntax err:expected 'id' before end of file",
            },
            {
                "1:1 syntax err:expected 'program' before 'id'",
            },
            {
                "1:1 syntax err:expected 'id' before end of file",
            },
            {
                "1:1 syntax err:expected 'program' before '('",
                "1:1 syntax err:expected 'id' before '('",
            },
            {
                "1:14 syntax err:expected ')' before end of file",
            },
        };

        // OUTPUT_RESULT(ParseProgramHead, input_strs);
        CHECK_RESULT(ParseProgramHead, input_strs, results, errs);
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
            "procedure p; begin end  begin end",
        };
        const vector<string> results = {
            "1:1 ProgramBody: \n"
            "1:7     ConstDeclaration: a\n"
            "        1:11 1\n"
            "1:14     ConstDeclaration: b\n"
            "        1:18 2\n"
            "1:25     VarDeclaration: \n"
            "1:32         Type: integer\n"
            "1:25         IdList: c, d\n"
            "1:41     Subprogram: \n"
            "1:41         SubprogramHead: procedure p\n"
            "\n"
            "1:54         SubprogramBody: \n"
            "            1:54 CompoundStatement :0\n"
            "1:65     Subprogram: \n"
            "1:65         SubprogramHead: function f integer\n"
            "1:76             Parameter: integer\n"
            "1:76                 IdList: a, b\n"
            "1:103         SubprogramBody: \n"
            "            1:103 CompoundStatement :0\n"
            "    1:114 CompoundStatement :0",
            "1:1 ProgramBody: \n"
            "1:7     ConstDeclaration: a\n"
            "        1:11 1\n"
            "1:13     ConstDeclaration: b\n"
            "        1:17 2\n"
            "    1:20 CompoundStatement :0",
            "1:1 ProgramBody: \n"
            "1:7     ConstDeclaration: a\n"
            "        1:11 1\n"
            "1:17     VarDeclaration: \n"
            "1:21         Type: integer\n"
            "1:17         IdList: a\n"
            "    1:30 CompoundStatement :0",
            "1:1 ProgramBody: \n"
            "1:7     ConstDeclaration: a\n"
            "        1:11 1\n"
            "1:13     Subprogram: \n"
            "1:13         SubprogramHead: procedure p\n"
            "\n"
            "1:26         SubprogramBody: \n"
            "            1:26 CompoundStatement :0\n"
            "    1:36 CompoundStatement :0",
            "1:1 ProgramBody: \n"
            "1:7     ConstDeclaration: a\n"
            "        1:11 1\n"
            "1:13     Subprogram: \n"
            "1:13         SubprogramHead: function f integer\n"
            "\n"
            "1:35         SubprogramBody: \n"
            "            1:35 CompoundStatement :0\n"
            "    1:46 CompoundStatement :0",
            "1:1 ProgramBody: \n"
            "1:7     ConstDeclaration: a\n"
            "        1:11 1\n"
            "    1:13 CompoundStatement :0",
            "1:1 ProgramBody: \n"
            "1:5     VarDeclaration: \n"
            "1:9         Type: integer\n"
            "1:5         IdList: a\n"
            "1:17     VarDeclaration: \n"
            "1:21         Type: integer\n"
            "1:17         IdList: b\n"
            "    1:30 CompoundStatement :0",
            "1:1 ProgramBody: \n"
            "1:5     VarDeclaration: \n"
            "1:9         Type: integer\n"
            "1:5         IdList: a\n"
            "1:17     Subprogram: \n"
            "1:17         SubprogramHead: procedure p\n"
            "\n"
            "1:30         SubprogramBody: \n"
            "            1:30 CompoundStatement :0\n"
            "    1:40 CompoundStatement :0",
            "1:1 ProgramBody: \n"
            "1:5     VarDeclaration: \n"
            "1:9         Type: integer\n"
            "1:5         IdList: a\n"
            "1:17     Subprogram: \n"
            "1:17         SubprogramHead: function f integer\n"
            "\n"
            "1:39         SubprogramBody: \n"
            "            1:39 CompoundStatement :0\n"
            "    1:50 CompoundStatement :0",
            "1:1 ProgramBody: \n"
            "1:5     VarDeclaration: \n"
            "1:9         Type: integer\n"
            "1:5         IdList: a\n"
            "    1:17 CompoundStatement :0",
            "1:1 ProgramBody: \n"
            "1:1     Subprogram: \n"
            "1:1         SubprogramHead: procedure p\n"
            "\n"
            "1:14         SubprogramBody: \n"
            "            1:14 CompoundStatement :0\n"
            "1:24     Subprogram: \n"
            "1:24         SubprogramHead: function f integer\n"
            "\n"
            "1:46         SubprogramBody: \n"
            "            1:46 CompoundStatement :0\n"
            "    1:57 CompoundStatement :0",
            "1:1 ProgramBody: \n"
            "1:1     Subprogram: \n"
            "1:1         SubprogramHead: procedure p\n"
            "\n"
            "1:14         SubprogramBody: \n"
            "            1:14 CompoundStatement :0\n"
            "1:24     Subprogram: \n"
            "1:24         SubprogramHead: procedure p2\n"
            "\n"
            "1:39         SubprogramBody: \n"
            "            1:39 CompoundStatement :0\n"
            "    1:50 CompoundStatement :0",
            "1:1 ProgramBody: \n"
            "1:1     Subprogram: \n"
            "1:1         SubprogramHead: procedure p\n"
            "\n"
            "1:14         SubprogramBody: \n"
            "            1:14 CompoundStatement :0\n"
            "    1:25 CompoundStatement :0",
        };
        const vector<vector<string>> errs = {
            {},
            {
                "1:13 syntax err:expected ';' before 'id'",
            },
            {
                "1:13 syntax err:expected ';' before 'var'",
            },
            {
                "1:13 syntax err:expected ';' before 'procedure'",
            },
            {
                "1:13 syntax err:expected ';' before 'function'",
            },
            {
                "1:13 syntax err:expected ';' before 'begin'",
            },
            {
                "1:17 syntax err:expected ';' before 'id'",
            },
            {
                "1:17 syntax err:expected ';' before 'procedure'",
            },
            {
                "1:17 syntax err:expected ';' before 'function'",
            },
            {
                "1:17 syntax err:expected ';' before 'begin'",
            },
            {
                "1:24 syntax err:expected ';' before 'function'",
            },
            {
                "1:24 syntax err:expected ';' before 'procedure'",
            },
            {
                "1:25 syntax err:expected ';' before 'begin'",
            },
        };

        // OUTPUT_RESULT(ParseProgramBody, input_strs);
        CHECK_RESULT(ParseProgramBody, input_strs, results, errs);
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
            "a 1.1 ",
            "a 'a'",
            "a 1",
            "a ",
        };

        const vector<string> results = {
            "1:1 ConstDeclaration: a\n"
            "    1:3 1",
            "1:1 ConstDeclaration: b\n"
            "    1:3     2.0000",
            "1:1 ConstDeclaration: c\n"
            "    1:3 unary_op:'-'\n"
            "    expr :\n"
            "        1:4 1",
            "1:1 ConstDeclaration: d\n"
            "    1:3 'a'",
            "1:1 ConstDeclaration: \n"
            "    unknown value",
            "1:1 ConstDeclaration: \n"
            "    1:2 1",
            "1:1 ConstDeclaration: a\n"
            "    1:3 unary_op:'-'\n"
            "    expr :\n"
            "        1:4 1",
            "1:1 ConstDeclaration: a\n"
            "    1:3     1.1000",
            "1:1 ConstDeclaration: a\n"
            "    1:3 'a'",
            "1:1 ConstDeclaration: a\n"
            "    1:3 1",
            "1:1 ConstDeclaration: a\n"
            "    unknown value",
        };
        const vector<vector<string>> errs = {
            {},
            {},
            {},
            {},
            {
                "1:1 syntax err:expected 'id' before end of file",
                "1:1 syntax err:expected '=' before end of file",
                "1:1 syntax error: parse expression error: no expected token",
            },
            {
                "1:1 syntax err:expected 'id' before '='",
            },
            {
                "1:3 syntax err:expected '=' before '-'",
            },
            {
                "1:3 syntax err:expected '=' before 'real'",
            },
            {
                "1:3 syntax err:expected '=' before 'string'",
            },
            {
                "1:3 syntax err:expected '=' before 'integer'",
            },
            {
                "1:2 syntax err:expected '=' before end of file",
                "1:2 syntax error: parse expression error: no expected token",
            },
        };

        // OUTPUT_RESULT(ParseConstDeclaration, input_strs);
        CHECK_RESULT(ParseConstDeclaration, input_strs, results, errs);
    }

    TEST(ProgramParserTest, TestParseVarDecl)
    {
        const vector<string> input_strs = {
            "a, b, c : integer",
            "d, e : array [] of integer",
            "a",
            "a,b,c integer",
            "a,b,c array [] of integer",
            "a,b,c real",
            "a,b,c char",
            "a,b,c boolean",
        };
        const vector<string> results = {
            "1:1 VarDeclaration: \n"
            "1:11     Type: integer\n"
            "1:1     IdList: a, b, c",
            "1:1 VarDeclaration: \n"
            "1:8     Type: array [] of integer\n"
            "1:1     IdList: d, e",
            "1:1 VarDeclaration: \n"
            "1:1     Type: unknown\n"
            "1:1     IdList: a",
            "1:1 VarDeclaration: \n"
            "1:7     Type: integer\n"
            "1:1     IdList: a, b, c",
            "1:1 VarDeclaration: \n"
            "1:7     Type: array [] of integer\n"
            "1:1     IdList: a, b, c",
            "1:1 VarDeclaration: \n"
            "1:7     Type: real\n"
            "1:1     IdList: a, b, c",
            "1:1 VarDeclaration: \n"
            "1:7     Type: char\n"
            "1:1     IdList: a, b, c",
            "1:1 VarDeclaration: \n"
            "1:7     Type: boolean\n"
            "1:1     IdList: a, b, c",
        };
        const vector<vector<string>> errs = {
            {},
            {},
            {
                "1:1 syntax err:expected ':' before end of file",
                "1:1 syntax err:expected basic type(integer, real, bool, char) or array before end of file",
            },
            {
                "1:7 syntax err:expected ':' before 'integer_type'",
            },
            {
                "1:7 syntax err:expected ':' before 'array'",
            },
            {
                "1:7 syntax err:expected ':' before 'real_type'",
            },
            {
                "1:7 syntax err:expected ':' before 'char_type'",
            },
            {
                "1:7 syntax err:expected ':' before 'boolean_type'",
            },
        };

        // OUTPUT_RESULT(ParseVarDeclaration, input_strs);
        CHECK_RESULT(ParseVarDeclaration, input_strs, results, errs);
    }

    TEST(ProgramParserTest, TestParseSubProgramDecl)
    {
        const vector<string> input_strs = {
            "function f : integer; begin end",
            "procedure p; begin end",
            "procedure p const a = 1;begin end",
            "procedure p var a : integer;begin end",
            "procedure p begin end",
        };
        const vector<string> results = {
            "1:1 Subprogram: \n"
            "1:1     SubprogramHead: function f integer\n"
            "\n"
            "1:23     SubprogramBody: \n"
            "        1:23 CompoundStatement :0",
            "1:1 Subprogram: \n"
            "1:1     SubprogramHead: procedure p\n"
            "\n"
            "1:14     SubprogramBody: \n"
            "        1:14 CompoundStatement :0",
            "1:1 Subprogram: \n"
            "1:1     SubprogramHead: procedure p\n"
            "\n"
            "1:13     SubprogramBody: \n"
            "1:19         ConstDeclaration: a\n"
            "            1:23 1\n"
            "        1:25 CompoundStatement :0",
            "1:1 Subprogram: \n"
            "1:1     SubprogramHead: procedure p\n"
            "\n"
            "1:13     SubprogramBody: \n"
            "1:17         VarDeclaration: \n"
            "1:21             Type: integer\n"
            "1:17             IdList: a\n"
            "        1:29 CompoundStatement :0",
            "1:1 Subprogram: \n"
            "1:1     SubprogramHead: procedure p\n"
            "\n"
            "1:13     SubprogramBody: \n"
            "        1:13 CompoundStatement :0",
        };
        const vector<vector<string>> errs = {
            {},
            {},
            {
                "1:13 syntax err:expected ';' before 'const'",
            },
            {
                "1:13 syntax err:expected ';' before 'var'",
            },
            {
                "1:13 syntax err:expected ';' before 'begin'",
            },
        };

        // OUTPUT_RESULT(ParseSubprogram, input_strs);
        CHECK_RESULT(ParseSubprogram, input_strs, results, errs);
    }

    TEST(ProgramParserTest, TestParseSubProgramHead)
    {
        const vector<string> input_strs = {
            "procedure p(a : integer) ",
            "procedure p",
            "procedure",
            "procedure (a : integer)",
            "procedure p(a : integer; b : integer",
            "function f(a,b : integer) :integer",
            "function f : integer",
            "function",
            "function (a : integer) : integer",
            "function f(a : integer; b : integer",
            "function f(a : integer; b : integer : integer",
            "function f(a : integer, b : integer",
            "function f(a : integer) ",
            "function f(a : integer) integer",
            "function f(a : integer) real",
            "function f(a : integer) boolean",
            "function f(a : integer) char",
            "function f(a : integer) :",
        };
        const vector<string> results = {
            "1:1 SubprogramHead: procedure p\n"
            "1:13     Parameter: integer\n"
            "1:13         IdList: a",
            "1:1 SubprogramHead: procedure p\n"
            "",
            "1:1 SubprogramHead: procedure \n"
            "",
            "1:1 SubprogramHead: procedure \n"
            "1:12     Parameter: integer\n"
            "1:12         IdList: a",
            "1:1 SubprogramHead: procedure p\n"
            "1:13     Parameter: integer\n"
            "1:13         IdList: a\n"
            "1:26     Parameter: integer\n"
            "1:26         IdList: b",
            "1:1 SubprogramHead: function f integer\n"
            "1:12     Parameter: integer\n"
            "1:12         IdList: a, b",
            "1:1 SubprogramHead: function f integer\n"
            "",
            "1:1 SubprogramHead: procedure \n"
            "",
            "1:1 SubprogramHead: function  integer\n"
            "1:11     Parameter: integer\n"
            "1:11         IdList: a",
            "1:1 SubprogramHead: function f unknown\n"
            "1:12     Parameter: integer\n"
            "1:12         IdList: a\n"
            "1:25     Parameter: integer\n"
            "1:25         IdList: b",
            "1:1 SubprogramHead: function f integer\n"
            "1:12     Parameter: integer\n"
            "1:12         IdList: a\n"
            "1:25     Parameter: integer\n"
            "1:25         IdList: b",
            "1:1 SubprogramHead: function f integer\n"
            "1:12     Parameter: integer\n"
            "1:12         IdList: a",
            "1:1 SubprogramHead: function f unknown\n"
            "1:12     Parameter: integer\n"
            "1:12         IdList: a",
            "1:1 SubprogramHead: function f integer\n"
            "1:12     Parameter: integer\n"
            "1:12         IdList: a",
            "1:1 SubprogramHead: function f real\n"
            "1:12     Parameter: integer\n"
            "1:12         IdList: a",
            "1:1 SubprogramHead: function f boolean\n"
            "1:12     Parameter: integer\n"
            "1:12         IdList: a",
            "1:1 SubprogramHead: function f char\n"
            "1:12     Parameter: integer\n"
            "1:12         IdList: a",
            "1:1 SubprogramHead: function f unknown\n"
            "1:12     Parameter: integer\n"
            "1:12         IdList: a",
        };
        const vector<vector<string>> errs = {
            {},
            {},
            {
                "1:1 syntax err:expected 'id' before end of file",
            },
            {
                "1:11 syntax err:expected 'id' before '('",
            },
            {
                "1:30 syntax err:expected ')' before end of file",
            },
            {},
            {},
            {
                "1:1 syntax err:expected 'id' before end of file",
            },
            {
                "1:10 syntax err:expected 'id' before '('",
            },
            {
                "1:29 syntax err:expected ')' before end of file",
                "1:29 syntax err:expected ':' before end of file",
                "1:29 syntax err:expected basic type(integer, real, bool, char) before end of file",
            },
            {
                "1:37 syntax err:expected ')' before ':'",
            },
            {
                "1:23 syntax err:expected ')' before ','",
            },
            {
                "1:24 syntax err:expected ':' before end of file",
                "1:24 syntax err:expected basic type(integer, real, bool, char) before end of file",
            },
            {
                "1:25 syntax err:expected ':' before 'integer_type'",
            },
            {
                "1:25 syntax err:expected ':' before 'real_type'",
            },
            {
                "1:25 syntax err:expected ':' before 'boolean_type'",
            },
            {
                "1:25 syntax err:expected ':' before 'char_type'",
            },
            {
                "1:25 syntax err:expected basic type(integer, real, bool, char) before end of file",
            },
        };

        // OUTPUT_RESULT(ParseSubprogramHead, input_strs);
        CHECK_RESULT(ParseSubprogramHead, input_strs, results, errs);
    }

    TEST(ProgramParserTest, TestParseSubProgramBody)
    {
        const vector<string> input_strs = {
            "const a = 1; b = 2; var c, d : integer;  begin end",
            "const a = 1 b = 2; begin end",
            "const a = 1 var a : integer; begin end",
            "const a = 1 begin end",
            "var a : integer b : integer; begin end",
            "var a : integer begin end",
        };
        const vector<string> results = {
            "1:1 SubprogramBody: \n"
            "1:7     ConstDeclaration: a\n"
            "        1:11 1\n"
            "1:14     ConstDeclaration: b\n"
            "        1:18 2\n"
            "1:25     VarDeclaration: \n"
            "1:32         Type: integer\n"
            "1:25         IdList: c, d\n"
            "    1:42 CompoundStatement :0",
            "1:1 SubprogramBody: \n"
            "1:7     ConstDeclaration: a\n"
            "        1:11 1\n"
            "1:13     ConstDeclaration: b\n"
            "        1:17 2\n"
            "    1:20 CompoundStatement :0",
            "1:1 SubprogramBody: \n"
            "1:7     ConstDeclaration: a\n"
            "        1:11 1\n"
            "1:17     VarDeclaration: \n"
            "1:21         Type: integer\n"
            "1:17         IdList: a\n"
            "    1:30 CompoundStatement :0",
            "1:1 SubprogramBody: \n"
            "1:7     ConstDeclaration: a\n"
            "        1:11 1\n"
            "    1:13 CompoundStatement :0",
            "1:1 SubprogramBody: \n"
            "1:5     VarDeclaration: \n"
            "1:9         Type: integer\n"
            "1:5         IdList: a\n"
            "1:17     VarDeclaration: \n"
            "1:21         Type: integer\n"
            "1:17         IdList: b\n"
            "    1:30 CompoundStatement :0",
            "1:1 SubprogramBody: \n"
            "1:5     VarDeclaration: \n"
            "1:9         Type: integer\n"
            "1:5         IdList: a\n"
            "    1:17 CompoundStatement :0",
        };
        const vector<vector<string>> errs = {
            {},
            {
                "1:13 syntax err:expected ';' before 'id'",
            },
            {
                "1:13 syntax err:expected ';' before 'var'",
            },
            {
                "1:13 syntax err:expected ';' before 'begin'",
            },
            {
                "1:17 syntax err:expected ';' before 'id'",
            },
            {
                "1:17 syntax err:expected ';' before 'begin'",
            },
        };

        // OUTPUT_RESULT(ParseSubprogramBody, input_strs);
        CHECK_RESULT(ParseSubprogramBody, input_strs, results, errs);
    }

    TEST(ProgramParserTest, TestParseIdList)
    {
        const vector<string> input_strs = {
            "a, b, c",
            "a",
            "",
            ", b",
        };
        const vector<string> results = {
            "1:1 IdList: a, b, c",
            "1:1 IdList: a",
            "1:1 IdList: ",
            "1:1 IdList: b",
        };
        const vector<vector<string>> errs = {
            {},
            {},
            {
                "1:1 syntax err:expected 'id' before end of file",
            },
            {
                "1:1 syntax err:expected 'id' before ','",
            },
        };

        // OUTPUT_RESULT(ParseIdList, input_strs);
        CHECK_RESULT(ParseIdList, input_strs, results, errs);
    }

    TEST(ProgramParserTest, TestParseType)
    {
        const vector<string> input_strs = {
            "integer",
            "real",
            "boolean",
            "char",
            "array [] of integer",
            "array [1..10] of integer",
            "array [1..10, 20..30] of integer",
            "",
            "[] of integer",
            "array [1..10 20..30] of integer",
            "array [ of integer",
            "array [] integer",
            "array [] real",
            "array [] boolean",
            "array [] char",
            "array [] of",
        };
        const vector<string> results = {
            "1:1 Type: integer",
            "1:1 Type: real",
            "1:1 Type: boolean",
            "1:1 Type: char",
            "1:1 Type: array [] of integer",
            "1:1 Type: array [1..10] of integer",
            "1:1 Type: array [1..10, 20..30] of integer",
            "1:1 Type: unknown",
            "1:1 Type: array [] of integer",
            "1:1 Type: array [1..10] of integer",
            "1:1 Type: array [0..0] of unknown",
            "1:1 Type: array [] of integer",
            "1:1 Type: array [] of real",
            "1:1 Type: array [] of boolean",
            "1:1 Type: array [] of char",
            "1:1 Type: array [] of unknown",
        };
        const vector<vector<string>> errs = {
            {},
            {},
            {},
            {},
            {},
            {},
            {},
            {
                "1:1 syntax err:expected basic type(integer, real, bool, char) or array before end of file",
            },
            {
                "1:1 syntax err:expected basic type(integer, real, bool, char) or array before '['",
            },
            {
                "1:14 syntax err:expected ']' before 'integer'",
            },
            {
                "1:9 syntax err:expected 'integer' before 'of'",
                "1:12 syntax err:expected '..' before end of file",
                "1:12 syntax err:expected 'integer' before end of file",
                "1:12 syntax err:expected ']' before end of file",
                "1:12 syntax err:expected 'of' before end of file",
                "1:12 syntax err:expected basic type(integer, real, bool, char) before end of file",
            },
            {
                "1:10 syntax err:expected 'of' before 'integer_type'",
            },
            {
                "1:10 syntax err:expected 'of' before 'real_type'",
            },
            {
                "1:10 syntax err:expected 'of' before 'boolean_type'",
            },
            {
                "1:10 syntax err:expected 'of' before 'char_type'",
            },
            {
                "1:10 syntax err:expected basic type(integer, real, bool, char) before end of file",
            },
        };

        // OUTPUT_RESULT(ParseType, input_strs);
        CHECK_RESULT(ParseType, input_strs, results, errs);
    }

    TEST(ProgramParserTest, TestParsePeriod)
    {
        const vector<string> input_strs = {
            "1..10",
            "",
            "..10",
            "1 10",
            "1",
            "1..",
        };
        const vector<string> results = {
            "1..10",
            "0..0",
            "0..10",
            "1..10",
            "1..0",
            "1..0",
        };
        const vector<vector<string>> errs = {
            {},
            {
                "1:1 syntax err:expected 'integer' before end of file",
                "1:1 syntax err:expected '..' before end of file",
                "1:1 syntax err:expected 'integer' before end of file",
            },
            {
                "1:1 syntax err:expected 'integer' before '..'",
            },
            {
                "1:3 syntax err:expected '..' before 'integer'",
            },
            {
                "1:1 syntax err:expected '..' before end of file",
                "1:1 syntax err:expected 'integer' before end of file",
            },
            {
                "1:2 syntax err:expected 'integer' before end of file",
            },
        };

        for (int i = 0; i < input_strs.size(); i++)
        {
            const string &input_str = input_strs[i];
            FILE *input = fmemopen((void *)input_str.c_str(), input_str.size(), "r");
            Parser par(input);
            auto expr = par.ParsePeriod();
            const vector<std::string> &err_messages = par.err_msg();
            // for (std::string err_message : err_messages)
            //{
            //     std::cout << err_message << std::endl;
            // }
            // std::cout << expr.digits_1 << ".." << expr.digits_2
            //           << std::endl;

            EXPECT_EQ(par.err_msg(), errs[i]);
            EXPECT_EQ(std::to_string(expr.digits_1) + ".." + std::to_string(expr.digits_2), results[i]);
        }
    }

    TEST(ProgramParserTest, TestParseParameter)
    {
        const vector<string> input_strs = {
            "a, b : integer",
            "var a , b : integer",
            "a, b",
            "a, b integer",
            "a, b real",
            "a, b boolean",
            "a, b char",
            "a, b :"};
        const vector<string> results = {
            "1:1 Parameter: integer\n"
            "1:1     IdList: a, b",
            "1:1 Parameter: var integer\n"
            "1:5     IdList: a, b",
            "1:1 Parameter: unknown\n"
            "1:1     IdList: a, b",
            "1:1 Parameter: integer\n"
            "1:1     IdList: a, b",
            "1:1 Parameter: real\n"
            "1:1     IdList: a, b",
            "1:1 Parameter: boolean\n"
            "1:1     IdList: a, b",
            "1:1 Parameter: char\n"
            "1:1     IdList: a, b",
            "1:1 Parameter: unknown\n"
            "1:1     IdList: a, b",
        };
        const vector<vector<string>> errs = {
            {},
            {},
            {
                "1:4 syntax err:expected ':' before end of file",
                "1:4 syntax err:expected basic type(integer, real, bool, char) before end of file",
            },
            {
                "1:6 syntax err:expected ':' before 'integer_type'",
            },
            {
                "1:6 syntax err:expected ':' before 'real_type'",
            },
            {
                "1:6 syntax err:expected ':' before 'boolean_type'",
            },
            {
                "1:6 syntax err:expected ':' before 'char_type'",
            },
            {
                "1:6 syntax err:expected basic type(integer, real, bool, char) before end of file",
            },
        };

        // OUTPUT_RESULT(ParseParameter, input_strs);
        CHECK_RESULT(ParseParameter, input_strs, results, errs);
    }
}
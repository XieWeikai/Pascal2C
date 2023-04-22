//
// Created by 谢卫凯 on 2023/3/26.
//

#include <iostream>
#include <cstdio>
#include <sstream>
#include <string>
#include <memory>

#include "gtest/gtest.h"
#include "parser/parser.h"
#include "ast/ast.h"
#include "ast/expr.h"
#include "ast/statement.h"

namespace pascal2c::parser
{
    TEST(StatementParserTest, TestAssignStatement)
    {
        const char *input_str =
            "a := a + 100 ;\n"
            "abc := 1 + 2 + 3 ;\n"
            "count[1,2] := 1 + 2 * 3;\n"
            "res := add(i+1,b+3) * factor() ;\n";
        FILE *input = fmemopen((void *)input_str, strlen(input_str), "r");

        std::string res[] = {
            "AssignStatement :\n"
            "Variable:\n"
            "    variable:a\n"
            "Expr :\n"
            "    binary_op:'+'\n"
            "    lhs :\n"
            "        variable:a\n"
            "    rhs :\n"
            "        100",

            "AssignStatement :\n"
            "Variable:\n"
            "    variable:abc\n"
            "Expr :\n"
            "    binary_op:'+'\n"
            "    lhs :\n"
            "        binary_op:'+'\n"
            "        lhs :\n"
            "            1\n"
            "        rhs :\n"
            "            2\n"
            "    rhs :\n"
            "        3",

            "AssignStatement :\n"
            "Variable:\n"
            "    variable:count\n"
            "    index 1:\n"
            "        1\n"
            "    index 2:\n"
            "        2\n"
            "Expr :\n"
            "    binary_op:'+'\n"
            "    lhs :\n"
            "        1\n"
            "    rhs :\n"
            "        binary_op:'*'\n"
            "        lhs :\n"
            "            2\n"
            "        rhs :\n"
            "            3",

            "AssignStatement :\n"
            "Variable:\n"
            "    variable:res\n"
            "Expr :\n"
            "    binary_op:'*'\n"
            "    lhs :\n"
            "        function:add\n"
            "        expr 1:\n"
            "            binary_op:'+'\n"
            "            lhs :\n"
            "                variable:i\n"
            "            rhs :\n"
            "                1\n"
            "        expr 2:\n"
            "            binary_op:'+'\n"
            "            lhs :\n"
            "                variable:b\n"
            "            rhs :\n"
            "                3\n"
            "    rhs :\n"
            "        function:factor"};
        int idx = 0;

        pascal2c::parser::Parser par(input);
        while (par.token_ != 0)
        {
            auto statement = par.ParseStatement();
            if (par.token_ != 0)
                par.Match(';');
            EXPECT_EQ(statement->ToString(0), res[idx++]);
        }
    }

    TEST(StatementParserTest, TestIfStatement)
    {
        const char *input_str =
            "if a <= limit then\n"
            "   a := a + 100;\n"
            "if i <= 10 then\n"
            "   i := i + 1\n"
            "else\n"
            "   i := i - 1;\n";
        FILE *input = fmemopen((void *)input_str, strlen(input_str), "r");
        pascal2c::parser::Parser par(input);

        std::string res[] = {
            "IfStatement :\n"
            "if_part:\n"
            "    AssignStatement :\n"
            "    Variable:\n"
            "        variable:a\n"
            "    Expr :\n"
            "        binary_op:'+'\n"
            "        lhs :\n"
            "            variable:a\n"
            "        rhs :\n"
            "            100",

            "IfStatement :\n"
            "if_part:\n"
            "    AssignStatement :\n"
            "    Variable:\n"
            "        variable:i\n"
            "    Expr :\n"
            "        binary_op:'+'\n"
            "        lhs :\n"
            "            variable:i\n"
            "        rhs :\n"
            "            1\n"
            "else_part:\n"
            "    AssignStatement :\n"
            "    Variable:\n"
            "        variable:i\n"
            "    Expr :\n"
            "        binary_op:'-'\n"
            "        lhs :\n"
            "            variable:i\n"
            "        rhs :\n"
            "            1"};

        int idx = 0;
        while (par.token_ != 0)
        {
            auto statement = par.ParseStatement();
            if (par.token_ != 0)
                par.Match(';');
            EXPECT_EQ(statement->ToString(0), res[idx++]);
        }
    }

    TEST(StatementParserTest, TestForStatement)
    {
        const char *input_str =
            "for i := 1 to 10 do\n"
            "   count := count + 1;\n"
            "for i := lower(i) to upper(i) do\n"
            "begin\n"
            "   sub_program;\n"
            "   call(1,2,3);\n"
            "   a := a + 1"
            "end\n";
        FILE *input = fmemopen((void *)input_str, strlen(input_str), "r");
        pascal2c::parser::Parser par(input);

        std::string res[] = {
            "ForStatement:\n"
            "from:\n"
            "    1\n"
            "to:\n"
            "    10\n"
            "do:\n"
            "    AssignStatement :\n"
            "    Variable:\n"
            "        variable:count\n"
            "    Expr :\n"
            "        binary_op:'+'\n"
            "        lhs :\n"
            "            variable:count\n"
            "        rhs :\n"
            "            1",

            "ForStatement:\n"
            "from:\n"
            "    function:lower\n"
            "    expr 1:\n"
            "        variable:i\n"
            "to:\n"
            "    function:upper\n"
            "    expr 1:\n"
            "        variable:i\n"
            "do:\n"
            "    CompoundStatement :3\n"
            "    statement :1\n"
            "        CallStatement :\n"
            "        name:sub_program\n"
            "    statement :2\n"
            "        CallStatement :\n"
            "        name:call\n"
            "        expr 1:\n"
            "            1\n"
            "        expr 2:\n"
            "            2\n"
            "        expr 3:\n"
            "            3\n"
            "    statement :3\n"
            "        AssignStatement :\n"
            "        Variable:\n"
            "            variable:a\n"
            "        Expr :\n"
            "            binary_op:'+'\n"
            "            lhs :\n"
            "                variable:a\n"
            "            rhs :\n"
            "                1",
        };
        int idx = 0;
        while (par.token_ != 0)
        {
            auto statement = par.ParseStatement();
            if (par.token_ != 0)
                par.Match(';');

            EXPECT_EQ(statement->ToString(0), res[idx++]);
        }
    }

    TEST(StatementParserTest, TestCompoundStatement)
    {
        const char *input_str =
            "begin\n"
            "   if l < r then\n"
            "       begin\n"
            "           mid := (l + r) / 2;\n"
            "           sort(l,mid);\n"
            "           sort(mid+1,r)\n"
            "       end\n"
            "   else"
            "       write(a,b,c + d)"
            "end;\n";
        FILE *input = fmemopen((void *)input_str, strlen(input_str), "r");
        pascal2c::parser::Parser par(input);

        std::string res[] = {
            "CompoundStatement :1\n"
            "statement :1\n"
            "    IfStatement :\n"
            "    if_part:\n"
            "        CompoundStatement :3\n"
            "        statement :1\n"
            "            AssignStatement :\n"
            "            Variable:\n"
            "                variable:mid\n"
            "            Expr :\n"
            "                binary_op:'/'\n"
            "                lhs :\n"
            "                    binary_op:'+'\n"
            "                    lhs :\n"
            "                        variable:l\n"
            "                    rhs :\n"
            "                        variable:r\n"
            "                rhs :\n"
            "                    2\n"
            "        statement :2\n"
            "            CallStatement :\n"
            "            name:sort\n"
            "            expr 1:\n"
            "                variable:l\n"
            "            expr 2:\n"
            "                variable:mid\n"
            "        statement :3\n"
            "            CallStatement :\n"
            "            name:sort\n"
            "            expr 1:\n"
            "                binary_op:'+'\n"
            "                lhs :\n"
            "                    variable:mid\n"
            "                rhs :\n"
            "                    1\n"
            "            expr 2:\n"
            "                variable:r\n"
            "    else_part:\n"
            "        CallStatement :\n"
            "        name:write\n"
            "        expr 1:\n"
            "            variable:a\n"
            "        expr 2:\n"
            "            variable:b\n"
            "        expr 3:\n"
            "            binary_op:'+'\n"
            "            lhs :\n"
            "                variable:c\n"
            "            rhs :\n"
            "                variable:d"};

        int idx = 0;

        while (par.token_ != 0)
        {
            auto statement = par.ParseStatement();
            if (par.token_ != 0)
                par.Match(';');

            //            std::cout << statement->ToString(0) << std::endl;
            EXPECT_EQ(statement->ToString(0), res[idx++]);
        }
    }

}
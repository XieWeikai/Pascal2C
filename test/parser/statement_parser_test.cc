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

        std::string res =
                "AssignStatement :\n"
                "Variable:\n"
                "    variable:a\n"
                "Expr :\n"
                "    binary_op:'+'\n"
                "    lhs :\n"
                "        CallOrVar: a\n"
                "    rhs :\n"
                "        100\n"
                "\n"
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
                "        3\n"
                "\n"
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
                "            3\n"
                "\n"
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
                "                CallOrVar: i\n"
                "            rhs :\n"
                "                1\n"
                "        expr 2:\n"
                "            binary_op:'+'\n"
                "            lhs :\n"
                "                CallOrVar: b\n"
                "            rhs :\n"
                "                3\n"
                "    rhs :\n"
                "        function:factor\n\n";
        std::stringstream str_s;

        pascal2c::parser::Parser par(input);
        while (par.token_ != 0)
        {
            auto statement = par.ParseStatement();
            str_s << statement->ToString(0) << "\n" << std::endl;
            if (par.token_ != 0)
                par.Match(';');
        }
//        std::cout << str_s.str();
        EXPECT_EQ(str_s.str(),res);
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

        std::string res =
                "IfStatement :\n"
                "condition:\n"
                "    binary_op:'L'\n"
                "    lhs :\n"
                "        CallOrVar: a\n"
                "    rhs :\n"
                "        CallOrVar: limit\n"
                "if_part:\n"
                "    AssignStatement :\n"
                "    Variable:\n"
                "        variable:a\n"
                "    Expr :\n"
                "        binary_op:'+'\n"
                "        lhs :\n"
                "            CallOrVar: a\n"
                "        rhs :\n"
                "            100\n"
                "\n"
                "IfStatement :\n"
                "condition:\n"
                "    binary_op:'L'\n"
                "    lhs :\n"
                "        CallOrVar: i\n"
                "    rhs :\n"
                "        10\n"
                "if_part:\n"
                "    AssignStatement :\n"
                "    Variable:\n"
                "        variable:i\n"
                "    Expr :\n"
                "        binary_op:'+'\n"
                "        lhs :\n"
                "            CallOrVar: i\n"
                "        rhs :\n"
                "            1\n"
                "else_part:\n"
                "    AssignStatement :\n"
                "    Variable:\n"
                "        variable:i\n"
                "    Expr :\n"
                "        binary_op:'-'\n"
                "        lhs :\n"
                "            CallOrVar: i\n"
                "        rhs :\n"
                "            1\n\n";
        std::stringstream str_s;

        while (par.token_ != 0)
        {
            auto statement = par.ParseStatement();
            str_s << statement->ToString(0) << "\n" << std::endl;
            if (par.token_ != 0)
                par.Match(';');
        }
//        std::cout << str_s.str();
        EXPECT_EQ(str_s.str(),res);
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

        std::string res =
                "ForStatement:\n"
                "id: i\n"
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
                "            CallOrVar: count\n"
                "        rhs :\n"
                "            1\n"
                "\n"
                "ForStatement:\n"
                "id: i\n"
                "from:\n"
                "    function:lower\n"
                "    expr 1:\n"
                "        CallOrVar: i\n"
                "to:\n"
                "    function:upper\n"
                "    expr 1:\n"
                "        CallOrVar: i\n"
                "do:\n"
                "    CompoundStatement :3\n"
                "    statement 1:\n"
                "        CallStatement :\n"
                "        name:sub_program\n"
                "    statement 2:\n"
                "        CallStatement :\n"
                "        name:call\n"
                "        expr 1:\n"
                "            1\n"
                "        expr 2:\n"
                "            2\n"
                "        expr 3:\n"
                "            3\n"
                "    statement 3:\n"
                "        AssignStatement :\n"
                "        Variable:\n"
                "            variable:a\n"
                "        Expr :\n"
                "            binary_op:'+'\n"
                "            lhs :\n"
                "                CallOrVar: a\n"
                "            rhs :\n"
                "                1\n\n";
        std::stringstream str_s;

        while (par.token_ != 0)
        {
            auto statement = par.ParseStatement();
            str_s << statement->ToString(0) << "\n" << std::endl;
            if (par.token_ != 0)
                par.Match(';');
        }
//        std::cout << str_s.str();
        EXPECT_EQ(str_s.str(),res);
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

        std::string res =
            "CompoundStatement :1\n"
            "statement 1:\n"
            "    IfStatement :\n"
            "    condition:\n"
            "        binary_op:'<'\n"
            "        lhs :\n"
            "            CallOrVar: l\n"
            "        rhs :\n"
            "            CallOrVar: r\n"
            "    if_part:\n"
            "        CompoundStatement :3\n"
            "        statement 1:\n"
            "            AssignStatement :\n"
            "            Variable:\n"
            "                variable:mid\n"
            "            Expr :\n"
            "                binary_op:'/'\n"
            "                lhs :\n"
            "                    binary_op:'+'\n"
            "                    lhs :\n"
            "                        CallOrVar: l\n"
            "                    rhs :\n"
            "                        CallOrVar: r\n"
            "                rhs :\n"
            "                    2\n"
            "        statement 2:\n"
            "            CallStatement :\n"
            "            name:sort\n"
            "            expr 1:\n"
            "                CallOrVar: l\n"
            "            expr 2:\n"
            "                CallOrVar: mid\n"
            "        statement 3:\n"
            "            CallStatement :\n"
            "            name:sort\n"
            "            expr 1:\n"
            "                binary_op:'+'\n"
            "                lhs :\n"
            "                    CallOrVar: mid\n"
            "                rhs :\n"
            "                    1\n"
            "            expr 2:\n"
            "                CallOrVar: r\n"
            "    else_part:\n"
            "        CallStatement :\n"
            "        name:write\n"
            "        expr 1:\n"
            "            CallOrVar: a\n"
            "        expr 2:\n"
            "            CallOrVar: b\n"
            "        expr 3:\n"
            "            binary_op:'+'\n"
            "            lhs :\n"
            "                CallOrVar: c\n"
            "            rhs :\n"
            "                CallOrVar: d\n\n";
        std::stringstream str_s;
        while (par.token_ != 0)
        {
            auto statement = par.ParseStatement();
            str_s << statement->ToString(0) << "\n" << std::endl;
            if (par.token_ != 0)
                par.Match(';');
        }
//        std::cout << str_s.str();
        EXPECT_EQ(str_s.str(),res);
    }

}
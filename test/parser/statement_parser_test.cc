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
                "1:1 AssignStatement :\n"
                "Variable:\n"
                "    1:1 variable:a\n"
                "Expr :\n"
                "    1:6 binary_op:'+'\n"
                "    lhs :\n"
                "        1:6 CallOrVar: a\n"
                "    rhs :\n"
                "        1:10 100\n"
                "\n"
                "2:1 AssignStatement :\n"
                "Variable:\n"
                "    2:1 variable:abc\n"
                "Expr :\n"
                "    2:8 binary_op:'+'\n"
                "    lhs :\n"
                "        1:1 binary_op:'+'\n"
                "        lhs :\n"
                "            2:8 1\n"
                "        rhs :\n"
                "            2:12 2\n"
                "    rhs :\n"
                "        2:16 3\n"
                "\n"
                "3:1 AssignStatement :\n"
                "Variable:\n"
                "    3:1 variable:count\n"
                "    index 1:\n"
                "        3:7 1\n"
                "    index 2:\n"
                "        3:9 2\n"
                "Expr :\n"
                "    3:15 binary_op:'+'\n"
                "    lhs :\n"
                "        3:15 1\n"
                "    rhs :\n"
                "        3:19 binary_op:'*'\n"
                "        lhs :\n"
                "            3:19 2\n"
                "        rhs :\n"
                "            3:23 3\n"
                "\n"
                "4:1 AssignStatement :\n"
                "Variable:\n"
                "    4:1 variable:res\n"
                "Expr :\n"
                "    4:8 binary_op:'*'\n"
                "    lhs :\n"
                "        4:8 function:add\n"
                "        expr 1:\n"
                "            4:12 binary_op:'+'\n"
                "            lhs :\n"
                "                4:12 CallOrVar: i\n"
                "            rhs :\n"
                "                4:14 1\n"
                "        expr 2:\n"
                "            4:16 binary_op:'+'\n"
                "            lhs :\n"
                "                4:16 CallOrVar: b\n"
                "            rhs :\n"
                "                4:18 3\n"
                "    rhs :\n"
                "        4:23 function:factor\n\n";
        std::stringstream str_s;

        pascal2c::parser::Parser par(input);
        while (par.token_ != 0)
        {
            auto statement = par.ParseStatement();
            str_s << statement->ToString(0) << "\n" << std::endl;
            if (par.token_ != 0)
                par.Match(';');
        }
//        std::cout << input_str << "\n\n";
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
                "1:1 IfStatement :\n"
                "condition:\n"
                "    1:4 binary_op:'L'\n"
                "    lhs :\n"
                "        1:4 CallOrVar: a\n"
                "    rhs :\n"
                "        1:9 CallOrVar: limit\n"
                "if_part:\n"
                "    2:4 AssignStatement :\n"
                "    Variable:\n"
                "        2:4 variable:a\n"
                "    Expr :\n"
                "        2:9 binary_op:'+'\n"
                "        lhs :\n"
                "            2:9 CallOrVar: a\n"
                "        rhs :\n"
                "            2:13 100\n"
                "\n"
                "3:1 IfStatement :\n"
                "condition:\n"
                "    3:4 binary_op:'L'\n"
                "    lhs :\n"
                "        3:4 CallOrVar: i\n"
                "    rhs :\n"
                "        3:9 10\n"
                "if_part:\n"
                "    4:4 AssignStatement :\n"
                "    Variable:\n"
                "        4:4 variable:i\n"
                "    Expr :\n"
                "        4:9 binary_op:'+'\n"
                "        lhs :\n"
                "            4:9 CallOrVar: i\n"
                "        rhs :\n"
                "            4:13 1\n"
                "else_part:\n"
                "    6:4 AssignStatement :\n"
                "    Variable:\n"
                "        6:4 variable:i\n"
                "    Expr :\n"
                "        6:9 binary_op:'-'\n"
                "        lhs :\n"
                "            6:9 CallOrVar: i\n"
                "        rhs :\n"
                "            6:13 1\n\n";
        std::stringstream str_s;

        while (par.token_ != 0)
        {
            auto statement = par.ParseStatement();
            str_s << statement->ToString(0) << "\n" << std::endl;
            if (par.token_ != 0)
                par.Match(';');
        }

//        std::cout << input_str << "\n\n";
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
            "   a := a + 1\n"
            "end\n";
        FILE *input = fmemopen((void *)input_str, strlen(input_str), "r");
        pascal2c::parser::Parser par(input);

        std::string res =
                "1:1 ForStatement:\n"
                "id: i\n"
                "from:\n"
                "    1:10 1\n"
                "to:\n"
                "    1:15 10\n"
                "do:\n"
                "    2:4 AssignStatement :\n"
                "    Variable:\n"
                "        2:4 variable:count\n"
                "    Expr :\n"
                "        2:13 binary_op:'+'\n"
                "        lhs :\n"
                "            2:13 CallOrVar: count\n"
                "        rhs :\n"
                "            2:21 1\n"
                "\n"
                "3:1 ForStatement:\n"
                "id: i\n"
                "from:\n"
                "    3:10 function:lower\n"
                "    expr 1:\n"
                "        3:16 CallOrVar: i\n"
                "to:\n"
                "    3:22 function:upper\n"
                "    expr 1:\n"
                "        3:28 CallOrVar: i\n"
                "do:\n"
                "    4:1 CompoundStatement :3\n"
                "    statement 1:\n"
                "        5:4 CallStatement :\n"
                "        name:sub_program\n"
                "    statement 2:\n"
                "        6:4 CallStatement :\n"
                "        name:call\n"
                "        expr 1:\n"
                "            6:9 1\n"
                "        expr 2:\n"
                "            6:11 2\n"
                "        expr 3:\n"
                "            6:13 3\n"
                "    statement 3:\n"
                "        7:4 AssignStatement :\n"
                "        Variable:\n"
                "            7:4 variable:a\n"
                "        Expr :\n"
                "            7:9 binary_op:'+'\n"
                "            lhs :\n"
                "                7:9 CallOrVar: a\n"
                "            rhs :\n"
                "                7:13 1\n\n";
        std::stringstream str_s;

        while (par.token_ != 0)
        {
            auto statement = par.ParseStatement();
            str_s << statement->ToString(0) << "\n" << std::endl;
            if (par.token_ != 0)
                par.Match(';');
        }
//        std::cout << input_str << "\n\n";
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
            "   else\n"
            "       write(a,b,c + d)\n"
            "end;\n";
        FILE *input = fmemopen((void *)input_str, strlen(input_str), "r");
        pascal2c::parser::Parser par(input);

        std::string res =
            "1:1 CompoundStatement :1\n"
            "statement 1:\n"
            "    2:4 IfStatement :\n"
            "    condition:\n"
            "        2:7 binary_op:'<'\n"
            "        lhs :\n"
            "            2:7 CallOrVar: l\n"
            "        rhs :\n"
            "            2:11 CallOrVar: r\n"
            "    if_part:\n"
            "        3:8 CompoundStatement :3\n"
            "        statement 1:\n"
            "            4:12 AssignStatement :\n"
            "            Variable:\n"
            "                4:12 variable:mid\n"
            "            Expr :\n"
            "                4:19 binary_op:'/'\n"
            "                lhs :\n"
            "                    4:19 binary_op:'+'\n"
            "                    lhs :\n"
            "                        4:20 CallOrVar: l\n"
            "                    rhs :\n"
            "                        4:24 CallOrVar: r\n"
            "                rhs :\n"
            "                    4:29 2\n"
            "        statement 2:\n"
            "            5:12 CallStatement :\n"
            "            name:sort\n"
            "            expr 1:\n"
            "                5:17 CallOrVar: l\n"
            "            expr 2:\n"
            "                5:19 CallOrVar: mid\n"
            "        statement 3:\n"
            "            6:12 CallStatement :\n"
            "            name:sort\n"
            "            expr 1:\n"
            "                6:17 binary_op:'+'\n"
            "                lhs :\n"
            "                    6:17 CallOrVar: mid\n"
            "                rhs :\n"
            "                    6:21 1\n"
            "            expr 2:\n"
            "                6:23 CallOrVar: r\n"
            "    else_part:\n"
            "        9:8 CallStatement :\n"
            "        name:write\n"
            "        expr 1:\n"
            "            9:14 CallOrVar: a\n"
            "        expr 2:\n"
            "            9:16 CallOrVar: b\n"
            "        expr 3:\n"
            "            9:18 binary_op:'+'\n"
            "            lhs :\n"
            "                9:18 CallOrVar: c\n"
            "            rhs :\n"
            "                9:22 CallOrVar: d\n\n";
        std::stringstream str_s;
        while (par.token_ != 0)
        {
            auto statement = par.ParseStatement();
            str_s << statement->ToString(0) << "\n" << std::endl;
            if (par.token_ != 0)
                par.Match(';');
        }
//        std::cout << input_str << "\n\n";
//        std::cout << str_s.str();
        EXPECT_EQ(str_s.str(),res);
    }

}
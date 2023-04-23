//
// Created by 谢卫凯 on 2023/3/16.
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

    TEST(TokenTest, TestNextToken)
    {
        const char *input_str = "1 + 2 + 3 * 4 - 5 <= 6";
        FILE *input = fmemopen((void *)input_str, strlen(input_str), "r");
        Parser par(input);
        int res_tokens[] = {
            TOK_INTEGER,
            '+',
            TOK_INTEGER,
            '+',
            TOK_INTEGER,
            '*',
            TOK_INTEGER,
            '-',
            TOK_INTEGER,
            TOK_LEOP,
            TOK_INTEGER};
        int vals[] = {
            1,
            -1,
            2,
            -1,
            3,
            -1,
            4,
            -1,
            5,
            -1,
            6,
        };
        for (int i = 0; i < 11; i++)
        {
            EXPECT_EQ(par.token_, res_tokens[i]);
            if (vals[i] != -1)
                EXPECT_EQ(par.tok_value_.intval, vals[i]);
            std::cout << par.line_ << ":" << par.column_ << std::endl;
            par.NextToken();
        }
    }

    TEST(ExprParserTest, TestParsePrimary)
    {
        const char *input_str = "3  1.23  -3  not 4 +4  -4.1234 abcd add(3,4) count[i+1,b+2] say() b[]";
        FILE *input = fmemopen((void *)input_str, strlen(input_str), "r");
        Parser par(input);
        int idx = 0;
        std::string res[] = {
            "3",
            "1.2300",
            R"(unary_op:'-'
expr :
    3)",
            R"(unary_op:'n'
expr :
    4)",
            R"(unary_op:'+'
expr :
    4)",
            R"(unary_op:'-'
expr :
    4.1234)",
            "variable:abcd",
            R"(function:add
expr 1:
    3
expr 2:
    4)",
            R"(variable:count
index 1:
    binary_op:'+'
    lhs :
        variable:i
    rhs :
        1
index 2:
    binary_op:'+'
    lhs :
        variable:b
    rhs :
        2)",
            "function:say"};
        std::shared_ptr<ast::Expression> expr;
        while (par.token_ != 0)
        {
            try
            {
                expr = par.ParsePrimary();
            }
            catch (SyntaxErr &e)
            {
                EXPECT_EQ(std::string(e.what()), "1:69: parse expression error: no expected token");
                break;
            }
            EXPECT_EQ(expr->ToString(0), res[idx++]);
        }
    }

    TEST(ExprParserTest, TestParseExpr)
    {
        const char *input_str = "1 + 2 + 3 + 4 #"
                                "1 + 2 * 3  #"
                                "(1 + 2) * 3  #"
                                "-(1 + 2) * 3  #"
                                "(-(1 + 2) * 3 <= 5) and (3 > 4) or (4 < 3) #"
                                "(-(1 + 2) * 3 <= 5) or (3 > 4) and (4 < 3) #";
        FILE *input = fmemopen((void *)input_str, strlen(input_str), "r");
        Parser par(input);
        int idx = 0;
        std::string res[] = {
            "binary_op:'+'\n"
            "lhs :\n"
            "    binary_op:'+'\n"
            "    lhs :\n"
            "        binary_op:'+'\n"
            "        lhs :\n"
            "            1\n"
            "        rhs :\n"
            "            2\n"
            "    rhs :\n"
            "        3\n"
            "rhs :\n"
            "    4",

            "binary_op:'+'\n"
            "lhs :\n"
            "    1\n"
            "rhs :\n"
            "    binary_op:'*'\n"
            "    lhs :\n"
            "        2\n"
            "    rhs :\n"
            "        3",

            "binary_op:'*'\n"
            "lhs :\n"
            "    binary_op:'+'\n"
            "    lhs :\n"
            "        1\n"
            "    rhs :\n"
            "        2\n"
            "rhs :\n"
            "    3",

            "binary_op:'*'\n"
            "lhs :\n"
            "    unary_op:'-'\n"
            "    expr :\n"
            "        binary_op:'+'\n"
            "        lhs :\n"
            "            1\n"
            "        rhs :\n"
            "            2\n"
            "rhs :\n"
            "    3",

            "binary_op:'o'\n"
            "lhs :\n"
            "    binary_op:'a'\n"
            "    lhs :\n"
            "        binary_op:'L'\n"
            "        lhs :\n"
            "            binary_op:'*'\n"
            "            lhs :\n"
            "                unary_op:'-'\n"
            "                expr :\n"
            "                    binary_op:'+'\n"
            "                    lhs :\n"
            "                        1\n"
            "                    rhs :\n"
            "                        2\n"
            "            rhs :\n"
            "                3\n"
            "        rhs :\n"
            "            5\n"
            "    rhs :\n"
            "        binary_op:'>'\n"
            "        lhs :\n"
            "            3\n"
            "        rhs :\n"
            "            4\n"
            "rhs :\n"
            "    binary_op:'<'\n"
            "    lhs :\n"
            "        4\n"
            "    rhs :\n"
            "        3",

            "binary_op:'o'\n"
            "lhs :\n"
            "    binary_op:'L'\n"
            "    lhs :\n"
            "        binary_op:'*'\n"
            "        lhs :\n"
            "            unary_op:'-'\n"
            "            expr :\n"
            "                binary_op:'+'\n"
            "                lhs :\n"
            "                    1\n"
            "                rhs :\n"
            "                    2\n"
            "        rhs :\n"
            "            3\n"
            "    rhs :\n"
            "        5\n"
            "rhs :\n"
            "    binary_op:'a'\n"
            "    lhs :\n"
            "        binary_op:'>'\n"
            "        lhs :\n"
            "            3\n"
            "        rhs :\n"
            "            4\n"
            "    rhs :\n"
            "        binary_op:'<'\n"
            "        lhs :\n"
            "            4\n"
            "        rhs :\n"
            "            3"};
        while (par.token_ != 0)
        {
            auto expr = par.ParseExpr();
            //            std::cout << expr->ToString(0) << "\n" << std::endl;
            EXPECT_EQ(expr->ToString(0), res[idx++]);
            if (par.token_ == TOK_ERROR)
                par.NextToken();
        }
    }

    TEST(TestCode, Test)
    {
    }

}
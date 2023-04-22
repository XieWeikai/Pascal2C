#include <iostream>
#include <cstdio>
#include <sstream>
#include <string>
#include <memory>

#include "gtest/gtest.h"
#include "parser/parser.h"
#include "ast/ast.h"
#include "ast/program.h"

namespace pascal2c::parser
{
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
}
//
// Created by 谢卫凯 on 2023/3/16.
//
#include <iostream>
#include <cstdio>
#include <sstream>
#include<string>
#include <memory>
#include <cstdio>

#include "gtest/gtest.h"
#include "parser/parser.h"
#include "ast/ast.h"
#include "ast/expr.h"
#include "ast/statement.h"

namespace pascal2c::parser {

    TEST(TokenTest, TestNextToken) {
        const char *input_str = "1 + 2 + 3 * 4 - 5 <= 6";
        FILE *input = fmemopen((void *)input_str, strlen(input_str),"r");
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
                TOK_INTEGER
        };
        int vals[] = {
                1, -1, 2, -1, 3, -1, 4, -1, 5, -1, 6,
        };
        int col[] = {
                1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 22
        };
        for (int i = 0; i < 11; i++) {
            EXPECT_EQ(par.token_, res_tokens[i]);
            if (vals[i] != -1)
                EXPECT_EQ(par.tok_value_.intval, vals[i]);
            EXPECT_EQ(par.line_,1);
            EXPECT_EQ(par.column_, col[i]);
            par.NextToken();
        }
    }

    TEST(ExprParserTest,TestParsePrimary) {
        const char *input_str = "3  1.23  -3  not 4 +4  -4.1234 abcd add(3,4) count[i+1,b+2] say() b[]";
        FILE *input = fmemopen((void *)input_str, strlen(input_str),"r");
        Parser par(input);
        std::string res =
                "3\n"
                "1.2300\n"
                "unary_op:'-'\n"
                "expr :\n"
                "    3\n"
                "unary_op:'n'\n"
                "expr :\n"
                "    4\n"
                "unary_op:'+'\n"
                "expr :\n"
                "    4\n"
                "unary_op:'-'\n"
                "expr :\n"
                "    4.1234\n"
                "CallOrVar: abcd\n"
                "function:add\n"
                "expr 1:\n"
                "    3\n"
                "expr 2:\n"
                "    4\n"
                "variable:count\n"
                "index 1:\n"
                "    binary_op:'+'\n"
                "    lhs :\n"
                "        CallOrVar: i\n"
                "    rhs :\n"
                "        1\n"
                "index 2:\n"
                "    binary_op:'+'\n"
                "    lhs :\n"
                "        CallOrVar: b\n"
                "    rhs :\n"
                "        2\n"
                "function:say\n";

        std::stringstream str_s;
        std::shared_ptr<ast::Expression> expr;
        while(par.token_ != 0){
            try {
                expr = par.ParsePrimary();
            }catch (SyntaxErr &e){
                // std::cout << e.what() << std::endl;
                EXPECT_EQ(std::string(e.what()),"1:69: parse expression error: no expected token");
                break;
            }
            str_s << expr->ToString(0) << std::endl;
        }
//        std::cout << str_s.str() << std::endl;
        EXPECT_EQ(str_s.str(),res);
    }

    TEST(ExprParserTest,TestParseExpr) {
        const char *input_str = "1 + 2 + 3 + 4;\n"
                                "1 + 2 * 3  ;\n"
                                "(1 + 2) * 3 ;\n"
                                "-(1 + 2) * 3  ;\n"
                                "(-(1 + 2) * 3 <= 5) and (3 > 4) or (4 < 3) ;\n"
                                "(-(1 + 2) * 3 <= 5) or (3 > 4) and (4 < 3) ;\n"
                                "1 + 'a' + 'abc' + a + b \n"
                                "true or false and true ;\n"
                                "(true or false) and true ;\n"
                                "true or (false and true) ;\n";
        FILE *input = fmemopen((void *)input_str, strlen(input_str),"r");
        Parser par(input);

        std::string res =
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
                "    4\n"
                "\n"
                "binary_op:'+'\n"
                "lhs :\n"
                "    1\n"
                "rhs :\n"
                "    binary_op:'*'\n"
                "    lhs :\n"
                "        2\n"
                "    rhs :\n"
                "        3\n"
                "\n"
                "binary_op:'*'\n"
                "lhs :\n"
                "    binary_op:'+'\n"
                "    lhs :\n"
                "        1\n"
                "    rhs :\n"
                "        2\n"
                "rhs :\n"
                "    3\n"
                "\n"
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
                "    3\n"
                "\n"
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
                "        3\n"
                "\n"
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
                "            3\n"
                "\n"
                "binary_op:'+'\n"
                "lhs :\n"
                "    binary_op:'+'\n"
                "    lhs :\n"
                "        binary_op:'+'\n"
                "        lhs :\n"
                "            binary_op:'+'\n"
                "            lhs :\n"
                "                1\n"
                "            rhs :\n"
                "                'a'\n"
                "        rhs :\n"
                "            string: abc\n"
                "    rhs :\n"
                "        CallOrVar: a\n"
                "rhs :\n"
                "    CallOrVar: b\n"
                "\n"
                "binary_op:'o'\n"
                "lhs :\n"
                "    true\n"
                "rhs :\n"
                "    binary_op:'a'\n"
                "    lhs :\n"
                "        false\n"
                "    rhs :\n"
                "        true\n"
                "\n"
                "binary_op:'a'\n"
                "lhs :\n"
                "    binary_op:'o'\n"
                "    lhs :\n"
                "        true\n"
                "    rhs :\n"
                "        false\n"
                "rhs :\n"
                "    true\n"
                "\n"
                "binary_op:'o'\n"
                "lhs :\n"
                "    true\n"
                "rhs :\n"
                "    binary_op:'a'\n"
                "    lhs :\n"
                "        false\n"
                "    rhs :\n"
                "        true\n\n";
        std::stringstream str_s;
        while(par.token_ != 0){
            auto expr = par.ParseExpr();
            str_s << expr->ToString(0) << "\n" << std::endl;
            if(par.token_ == ';')
                par.NextToken();
        }
//        std::cout << str_s.str() << std::endl;
        EXPECT_EQ(str_s.str(),res);
    }

}

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
        fclose(input);
    }

    TEST(ExprParserTest,TestParsePrimary) {
        const char *input_str = "3  1.23  -3  not 4 +4  -4.1234 abcd add(3,4) count[i+1,b+2] say() b[]";
        FILE *input = fmemopen((void *)input_str, strlen(input_str),"r");
        Parser par(input);
        std::string res =
                "1:1 3\n"
                "1:4 1.2300\n"
                "1:10 unary_op:'-'\n"
                "expr :\n"
                "    1:11 3\n"
                "1:14 unary_op:'n'\n"
                "expr :\n"
                "    1:18 4\n"
                "1:20 unary_op:'+'\n"
                "expr :\n"
                "    1:21 4\n"
                "1:24 unary_op:'-'\n"
                "expr :\n"
                "    1:25     4.1234\n"
                "1:32 CallOrVar: abcd\n"
                "1:37 function:add\n"
                "expr 1:\n"
                "    1:41 3\n"
                "expr 2:\n"
                "    1:43 4\n"
                "1:46 variable:count\n"
                "index 1:\n"
                "    1:52 binary_op:'+'\n"
                "    lhs :\n"
                "        1:52 CallOrVar: i\n"
                "    rhs :\n"
                "        1:54 1\n"
                "index 2:\n"
                "    1:56 binary_op:'+'\n"
                "    lhs :\n"
                "        1:56 CallOrVar: b\n"
                "    rhs :\n"
                "        1:58 2\n"
                "1:61 function:say\n";

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

        fclose(input);
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
                "1:1 binary_op:'+'\n"
                "lhs :\n"
                "    1:1 binary_op:'+'\n"
                "    lhs :\n"
                "        1:1 binary_op:'+'\n"
                "        lhs :\n"
                "            1:1 1\n"
                "        rhs :\n"
                "            1:5 2\n"
                "    rhs :\n"
                "        1:9 3\n"
                "rhs :\n"
                "    1:13 4\n"
                "\n"
                "2:1 binary_op:'+'\n"
                "lhs :\n"
                "    2:1 1\n"
                "rhs :\n"
                "    2:5 binary_op:'*'\n"
                "    lhs :\n"
                "        2:5 2\n"
                "    rhs :\n"
                "        2:9 3\n"
                "\n"
                "3:1 binary_op:'*'\n"
                "lhs :\n"
                "    3:1 binary_op:'+'\n"
                "    lhs :\n"
                "        3:2 1\n"
                "    rhs :\n"
                "        3:6 2\n"
                "rhs :\n"
                "    3:11 3\n"
                "\n"
                "4:1 binary_op:'*'\n"
                "lhs :\n"
                "    4:1 unary_op:'-'\n"
                "    expr :\n"
                "        4:2 binary_op:'+'\n"
                "        lhs :\n"
                "            4:3 1\n"
                "        rhs :\n"
                "            4:7 2\n"
                "rhs :\n"
                "    4:12 3\n"
                "\n"
                "5:1 binary_op:'o'\n"
                "lhs :\n"
                "    5:1 binary_op:'a'\n"
                "    lhs :\n"
                "        5:1 binary_op:'L'\n"
                "        lhs :\n"
                "            5:2 binary_op:'*'\n"
                "            lhs :\n"
                "                5:2 unary_op:'-'\n"
                "                expr :\n"
                "                    5:3 binary_op:'+'\n"
                "                    lhs :\n"
                "                        5:4 1\n"
                "                    rhs :\n"
                "                        5:8 2\n"
                "            rhs :\n"
                "                5:13 3\n"
                "        rhs :\n"
                "            5:18 5\n"
                "    rhs :\n"
                "        5:25 binary_op:'>'\n"
                "        lhs :\n"
                "            5:26 3\n"
                "        rhs :\n"
                "            5:30 4\n"
                "rhs :\n"
                "    5:36 binary_op:'<'\n"
                "    lhs :\n"
                "        5:37 4\n"
                "    rhs :\n"
                "        5:41 3\n"
                "\n"
                "6:1 binary_op:'o'\n"
                "lhs :\n"
                "    6:1 binary_op:'L'\n"
                "    lhs :\n"
                "        6:2 binary_op:'*'\n"
                "        lhs :\n"
                "            6:2 unary_op:'-'\n"
                "            expr :\n"
                "                6:3 binary_op:'+'\n"
                "                lhs :\n"
                "                    6:4 1\n"
                "                rhs :\n"
                "                    6:8 2\n"
                "        rhs :\n"
                "            6:13 3\n"
                "    rhs :\n"
                "        6:18 5\n"
                "rhs :\n"
                "    6:24 binary_op:'a'\n"
                "    lhs :\n"
                "        6:24 binary_op:'>'\n"
                "        lhs :\n"
                "            6:25 3\n"
                "        rhs :\n"
                "            6:29 4\n"
                "    rhs :\n"
                "        6:36 binary_op:'<'\n"
                "        lhs :\n"
                "            6:37 4\n"
                "        rhs :\n"
                "            6:41 3\n"
                "\n"
                "7:1 binary_op:'+'\n"
                "lhs :\n"
                "    7:1 binary_op:'+'\n"
                "    lhs :\n"
                "        7:1 binary_op:'+'\n"
                "        lhs :\n"
                "            7:1 binary_op:'+'\n"
                "            lhs :\n"
                "                7:1 1\n"
                "            rhs :\n"
                "                7:5 'a'\n"
                "        rhs :\n"
                "            7:11 string: abc\n"
                "    rhs :\n"
                "        7:19 CallOrVar: a\n"
                "rhs :\n"
                "    7:23 CallOrVar: b\n"
                "\n"
                "8:1 binary_op:'o'\n"
                "lhs :\n"
                "    8:1 true\n"
                "rhs :\n"
                "    8:9 binary_op:'a'\n"
                "    lhs :\n"
                "        8:9 false\n"
                "    rhs :\n"
                "        8:19 true\n"
                "\n"
                "9:1 binary_op:'a'\n"
                "lhs :\n"
                "    9:1 binary_op:'o'\n"
                "    lhs :\n"
                "        9:2 true\n"
                "    rhs :\n"
                "        9:10 false\n"
                "rhs :\n"
                "    9:21 true\n"
                "\n"
                "10:1 binary_op:'o'\n"
                "lhs :\n"
                "    10:1 true\n"
                "rhs :\n"
                "    10:9 binary_op:'a'\n"
                "    lhs :\n"
                "        10:10 false\n"
                "    rhs :\n"
                "        10:20 true\n\n";
        std::stringstream str_s;
        while(par.token_ != 0){
            auto expr = par.ParseExpr();
            str_s << expr->ToString(0) << "\n" << std::endl;
            if(par.token_ == ';')
                par.NextToken();
        }

//        std::cout << str_s.str() << std::endl;
        EXPECT_EQ(str_s.str(),res);

        fclose(input);
    }

}

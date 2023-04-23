//
// Created by 谢卫凯 on 2023/4/23.
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

namespace pascal2c::parser{
    TEST(ErrorHandleTest, TestErrorHandle) {
        const char *input_str =
                "begin\n"
                "    a : = 1 + 2;\n"
                "    for := 1 to 10 do\n"
                "        subprogram;\n"
                "    if 3 <> 4 then\n"
                "        a := 3 + 4;\n"
                "\n"
                "    const a = 'b';\n"
                "    var c = 'd' \n"
                "\n"
                "    my_func (1,2+3;\n"
                "    c := 1 + 2 + *\n"
                "\n"
                "    d := (3 * 4 -( 5 - 3))\n"
                "\n"
                "    if (3 - (4) > c then\n"
                "        write('hello')\n"
                "\n"
                "    for c := a to b do begin\n"
                "        write(a,b,c);\n"
                "    end\n"
                "    c[3,4,7 := 4\n"
                "end.";
        FILE *input = fmemopen((void *) input_str, strlen(input_str), "r");
        Parser par(input);
        auto stat = par.ParseCompoundStatement();
        std::stringstream output;
        output << stat->ToString(0) << std::endl;
        output << "\nerr_msg:\n";
        for (const auto &e: par.err_msg_)
            output << e << std::endl;

        std::string res =
                "CompoundStatement :5\n"
                "statement 1:\n"
                "    CallStatement :\n"
                "    name:subprogram\n"
                "statement 2:\n"
                "    IfStatement :\n"
                "    condition:\n"
                "        binary_op:'N'\n"
                "        lhs :\n"
                "            3\n"
                "        rhs :\n"
                "            4\n"
                "    if_part:\n"
                "        AssignStatement :\n"
                "        Variable:\n"
                "            variable:a\n"
                "        Expr :\n"
                "            binary_op:'+'\n"
                "            lhs :\n"
                "                3\n"
                "            rhs :\n"
                "                4\n"
                "statement 3:\n"
                "    AssignStatement :\n"
                "    Variable:\n"
                "        variable:d\n"
                "    Expr :\n"
                "        binary_op:'-'\n"
                "        lhs :\n"
                "            binary_op:'*'\n"
                "            lhs :\n"
                "                3\n"
                "            rhs :\n"
                "                4\n"
                "        rhs :\n"
                "            binary_op:'-'\n"
                "            lhs :\n"
                "                5\n"
                "            rhs :\n"
                "                3\n"
                "statement 4:\n"
                "    CallStatement :\n"
                "    name:write\n"
                "    expr 1:\n"
                "        string: hello\n"
                "statement 5:\n"
                "    ForStatement:\n"
                "    id: c\n"
                "    from:\n"
                "        CallOrVar: a\n"
                "    to:\n"
                "        CallOrVar: b\n"
                "    do:\n"
                "        CompoundStatement :1\n"
                "        statement 1:\n"
                "            CallStatement :\n"
                "            name:write\n"
                "            expr 1:\n"
                "                CallOrVar: a\n"
                "            expr 2:\n"
                "                CallOrVar: b\n"
                "            expr 3:\n"
                "                CallOrVar: c\n"
                "\n"
                "err_msg:\n"
                "2:7 syntax error: lost ':=' when parsing assign statement\n"
                "3:9 syntax error: missing id in for statement\n"
                "8:5: syntax error: declaration is not part of statement\n"
                "8:13 syntax error: lost ':=' when parsing assign statement\n"
                "9:5: syntax error: declaration is not part of statement\n"
                "9:11 syntax error: lost ':=' when parsing assign statement\n"
                "11:19 syntax error: unclosed parentheses\n"
                "12:18: parse expression error: no expected token\n"
                "16:5 syntax error: missing ';' at the end of statement\n"
                "16:21 syntax err:expected TOK_) got TOK_THEN\n"
                "19:5 syntax error: missing ';' at the end of statement\n"
                "22:5 syntax error: missing ';' at the end of statement\n"
                "22:13 syntax error: unclosed brackets\n";
//        std::cout << output.str() << std::endl;
        EXPECT_EQ(output.str(), res);
    }
}

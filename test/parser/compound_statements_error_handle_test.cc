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

        std::string err_msgs[] = {
                "2:7 syntax error: lost ':=' when parsing assign statement",
                "3:9 syntax error: missing id in for statement",
                "8:5 syntax error: declaration is not part of statement",
                "8:13 syntax error: lost ':=' when parsing assign statement",
                "9:5 syntax error: declaration is not part of statement",
                "9:11 syntax error: lost ':=' when parsing assign statement",
                "11:19 syntax error: unclosed parentheses",
                "12:18 syntax error: parse expression error: no expected token",
                "16:5 syntax error: missing ';' at the end of statement",
                "16:21 syntax err:expected ')' before 'then'",
                "19:5 syntax error: missing ';' at the end of statement",
                "21:5 last statement should not end with ;",
                "22:5 syntax error: missing ';' at the end of statement",
                "22:13 syntax error: unclosed brackets"
        };
        int ind = 0;

        FILE *input = fmemopen((void *) input_str, strlen(input_str), "r");
        Parser par(input);
        auto stat = par.ParseCompoundStatement();

        for (const auto &e: par.err_msg_) {
            EXPECT_EQ(e, err_msgs[ind++]);
        }

        fclose(input);
    }
}

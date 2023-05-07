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

        std::cout << output.str() << std::endl;

        fclose(input);

        std::cout << "-----syntax errors------" << std::endl;
        for(const auto &e: par.syntax_errs()){
            std::cout << "line:" << e.line() << " col:" << e.col() << " " << e.err_msg() << std::endl;
        }

        std::cout << input_str;
    }
}

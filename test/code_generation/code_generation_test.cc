#include <string>

#include <gtest/gtest.h>

#include "ast_adapter_test.cc"
#include "code_generation/ast_adapter.h"
#include "code_generation/code_generator.h"
#include "code_generation/token_adapter.h"

TEST(CodeGenerationVanillaTest, ConvertASTToC) {
    using string = ::std::string;
    string source_code = R"(
    program Simple;
    var
        x, y: integer;
    begin
        x := 2 + 3;
        y := x - 1;
    end.
    )";
    string expected_ast = R"(
    Program: Simple
     Block
      VarDecl: x : integer
      VarDecl: y : integer
      Compound
       Assign
        Left:
         Var: x
        Right:
         BinOp: PLUS
          Num: 2
          Num: 3
       Assign
        Left:
         Var: y
        Right:
         BinOp: MINUS
          Var: x
          Num: 1
    )";
    string expected_c_code = "#include <stdio.h>\n"
                             "\n"
                             "int main() {\n"
                             "    int x;\n"
                             "    int y;\n"
                             "    x = (2 + 3);\n"
                             "    y = (x - 1);\n"
                             "    return 0;\n"
                             "}\n";
}

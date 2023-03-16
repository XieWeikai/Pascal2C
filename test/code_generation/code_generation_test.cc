#include <string>

#include <gtest/gtest.h>

#include "code_generation/ast_interface.h"
#include "code_generation/code_generator.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

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

    pascal2c::lexer::Lexer lexer(source_code);
    pascal2c::parser::Parser parser(&lexer);
    auto ast_program = parser.Program();

    pascal2c::code_generation::CodeGenerator code_generator(&parser);
    code_generator.Visit(ast_program);
    string c_code = code_generator.GetCCode();

    ASSERT_EQ(expected_c_code, c_code);
}

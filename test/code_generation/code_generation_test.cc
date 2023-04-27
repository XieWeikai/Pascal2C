#include "ast_adapter_test.cc"
#include "code_generation/ast_adapter.h"
#include "code_generation/ast_printer.h"
#include "code_generation/code_generator.h"
#include "code_generation/token_adapter.h"
#include "gmock/gmock.h"
#include <algorithm>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace pascal2c {
namespace code_generation {
using ::testing::Return;
using ::testing::ReturnRef;
using string = ::std::string;

std::shared_ptr<Program> SimpleProgramAST() {
    // vector<std::shared_ptr<ASTNode>> declarations;
    auto declarations = std::make_shared<vector<std::shared_ptr<ASTNode>>>();

    // VarDecl part
    const auto token_int = std::make_shared<Token>(TokenType::TYPE, "integer");
    const auto type_int = std::make_shared<Type>(std::move(token_int));

    const auto token_x = std::make_shared<Token>(TokenType::INTEGER, "x");
    const auto var_x = std::make_shared<Var>(std::move(token_x));
    const auto var_decl_x =
        std::make_shared<VarDeclaration>(std::move(var_x), std::move(type_int));

    const auto token_y = std::make_shared<Token>(TokenType::INTEGER, "y");
    const auto var_y = std::make_shared<Var>(std::move(token_y));
    const auto var_decl_y =
        std::make_shared<VarDeclaration>(std::move(var_y), std::move(type_int));

    declarations->push_back(std::move(var_decl_x));
    declarations->push_back(std::move(var_decl_y));

    // Compound statement part
    auto children = std::make_shared<vector<std::shared_ptr<ASTNode>>>();
    // :=
    const auto token_assign_op =
        std::make_shared<Token>(TokenType::ASSIGN, ":=");
    const auto assign_op = std::make_shared<Oper>(std::move(token_assign_op));
    // +
    const auto token_plus_op = std::make_shared<Token>(TokenType::PLUS, "+");
    const auto plus_op = std::make_shared<Oper>(std::move(token_plus_op));
    // -
    const auto token_minus_op = std::make_shared<Token>(TokenType::MINUS, "-");
    const auto minus_op = std::make_shared<Oper>(std::move(token_minus_op));

    // 2 + 3
    const auto token_2 = std::make_shared<Token>(TokenType::INTEGER, "2");
    const auto num_2 = std::make_shared<Num>(std::move(token_2));
    const auto token_3 = std::make_shared<Token>(TokenType::INTEGER, "3");
    const auto num_3 = std::make_shared<Num>(std::move(token_3));
    const auto bin_plus_op = std::make_shared<BinOp>(
        std::move(num_2), std::move(plus_op), std::move(num_3));

    // x - 1
    const auto token_1 = std::make_shared<Token>(TokenType::INTEGER, "1");
    const auto num_1 = std::make_shared<Num>(std::move(token_1));
    const auto bin_minus_op = std::make_shared<BinOp>(
        std::move(var_x), std::move(minus_op), std::move(num_1));

    const auto assign_x =
        std::make_shared<Assign>(std::move(var_x), std::move(bin_plus_op));
    const auto assign_y =
        std::make_shared<Assign>(std::move(var_y), std::move(bin_minus_op));

    // Construct compound
    children->push_back(std::move(assign_x));
    children->push_back(std::move(assign_y));
    const auto compound = std::make_shared<Compound>(std::move(*children));

    const auto declaration =
        std::make_shared<Declaration>(std::move(*declarations));

    auto block =
        std::make_shared<Block>(std::move(declaration), std::move(compound));
    auto program = std::make_shared<Program>("Simple", std::move(block));

    return program;
}

TEST(GeneratorTest, ASTPrinterTest) {
    string source_code = R"(program Simple;
var
    x, y: integer;
begin
    x := 2 + 3;
    y := x - 1;
end.
)";

    auto program = SimpleProgramAST();
    EXPECT_EQ(program->GetName(), "Simple");
    EXPECT_EQ(program->GetBlock()->GetDeclatation()->GetDeclarations().size(),
              2);
    ASTPrinter ast_printer;
    ast_printer->Traverse(program);
    string printed_ast = ast_printer->ToString();

    string expected_ast = R"(Program: Simple
 Block
  Declaration: 
   VarDecl: x: integer
   VarDecl: y: integer
  Compound
   Assign
    Left:
     Var: x
    Right:
     BinOp:
      Num: 2
      Oper: PLUS
      Num: 3
   Assign
    Left:
     Var: y
    Right:
     BinOp:
      Var: x
      Oper: MINUS
      Num: 1
)";

    EXPECT_EQ(printed_ast, expected_ast);
}

TEST(GeneratorTest, CodeGeneratorTest) {
    string source_code = R"(program Simple;
var
    x, y: integer;
begin
    x := 2 + 3;
    y := x - 1;
end.
)";

    auto program = SimpleProgramAST();
    auto code_generator = std::make_shared<CodeGenerator>(program);
    code_generator->Interpret();
    auto generated_ccode = code_generator->GetCCode();

    string expected_c_code = "#include <stdio.h>\n"
                             "#include <stdlib.h>\n"
                             "\n"
                             "// Simple\n"
                             "int main(int argc, char* argv[]) {\n"
                             "    int x;\n"
                             "    int y;\n"
                             "    x = (2 + 3);\n"
                             "    y = (x - 1);\n"
                             "    return 0;\n"
                             "}\n"
                             "// Simple\n";
    EXPECT_EQ(generated_ccode, expected_c_code);
}
} // namespace code_generation
} // namespace pascal2c
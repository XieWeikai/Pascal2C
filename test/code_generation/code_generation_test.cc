#include "ast_adapter_test.cc"
#include "code_generation/ast_adapter.h"
#include "code_generation/ast_printer.h"
#include "code_generation/code_generator.h"
#include "code_generation/token_adapter.h"
#include "gmock/gmock.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

namespace pascal2c {
namespace code_generation {
using ::testing::Return;
using ::testing::ReturnRef;

TEST(ASTPrinterTest, ConvertASTToC) {
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
    const auto mock_program = std::make_shared<MockProgram>();
    const auto mock_block = std::make_shared<MockBlock>();
    vector<std::shared_ptr<ASTNode>> declarations;

    // VarDecl part
    const auto token_int = std::make_shared<Token>(TokenType::TYPE, "integer");
    const auto type_int = std::make_shared<Type>(token_int);

    const auto token_x = std::make_shared<Token>(TokenType::INTEGER, "x");
    const auto var_x = std::make_shared<Var>(token_x);
    const auto var_decl_x = std::make_shared<VarDecl>(var_x, type_int);

    const auto token_y = std::make_shared<Token>(TokenType::INTEGER, "y");
    const auto var_y = std::make_shared<Var>(token_y);
    const auto var_decl_y = std::make_shared<VarDecl>(var_y, type_int);

    declarations.push_back(var_decl_x);
    declarations.push_back(var_decl_y);

    // Compound statement part
    vector<std::shared_ptr<ASTNode>> children;
    // :=
    const auto token_assign_op =
        std::make_shared<Token>(TokenType::ASSIGN, ":=");
    const auto assign_op = std::make_shared<Oper>(token_assign_op);
    // +
    const auto token_plus_op = std::make_shared<Token>(TokenType::PLUS, "+");
    const auto plus_op = std::make_shared<Oper>(token_plus_op);
    // -
    const auto token_minus_op = std::make_shared<Token>(TokenType::MINUS, "-");
    const auto minus_op = std::make_shared<Oper>(token_minus_op);

    // 2 + 3
    const auto token_2 = std::make_shared<Token>(TokenType::INTEGER, "2");
    const auto num_2 = std::make_shared<Num>(token_2);
    const auto token_3 = std::make_shared<Token>(TokenType::INTEGER, "3");
    const auto num_3 = std::make_shared<Num>(token_3);
    const auto bin_plus_op = std::make_shared<BinOp>(num_2, plus_op, num_3);

    // x - 1
    const auto token_1 = std::make_shared<Token>(TokenType::INTEGER, "1");
    const auto num_1 = std::make_shared<Num>(token_1);
    const auto bin_minus_op = std::make_shared<BinOp>(var_x, minus_op, num_1);

    const auto assign_x = std::make_shared<Assign>(var_x, bin_plus_op);
    const auto assign_y = std::make_shared<Assign>(var_y, bin_minus_op);

    // Construct compound
    children.push_back(assign_x);
    children.push_back(assign_y);
    const auto compound = std::make_shared<Compound>(children);

    EXPECT_CALL(*mock_program, GetName()).WillOnce(Return("Simple"));
    // EXPECT_CALL(*mock_program, GetBlock()).WillOnce(ReturnRef(mock_block));
    EXPECT_CALL(*mock_block, GetDeclarations())
        .WillOnce(ReturnRef(declarations));
    EXPECT_CALL(*mock_block, GetCompoundStatement())
        .WillOnce(ReturnRef(compound));
    auto block = std::make_shared<Block>(mock_block->GetDeclarations(),
                                         mock_block->GetCompoundStatement());
    auto program = std::make_shared<Program>(mock_program->GetName(), block);
    auto ast_printer = std::make_shared<ASTPrinter>(program);
    ast_printer->Visit();
    string printed_ast = ast_printer->ToString();

    string expected_ast = R"(Program: Simple
 Block
  VarDecl: x: integer
  VarDecl: y: integer
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

    EXPECT_EQ(printed_ast, expected_ast);

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
} // namespace code_generation
} // namespace pascal2c

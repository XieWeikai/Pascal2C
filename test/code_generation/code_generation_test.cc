#include "ast_adapter_test.cc"
#include "code_generation/ast_adapter.h"
#include "code_generation/code_generator.h"
#include "code_generation/token_adapter.h"
#include "symbol_table_mock.h"
#include "gmock/gmock.h"
#include <algorithm>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using namespace std;
using namespace pascal2c::code_generation;
using string = ::std::string;

std::shared_ptr<Program> SimpleProgramAST() {
    // vector<std::shared_ptr<ASTNode>> declarations;
    auto declarations =
        std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();

    // VarDecl part
    const auto type_int = std::make_shared<Type>(
        make_shared<Token>(TokenType::RESERVED, "integer"));
    const auto var_x =
        std::make_shared<Var>(make_shared<Token>(TokenType::IDENTIFIER, "x"));
    const auto var_decl_x =
        std::make_shared<VarDeclaration>(std::move(var_x), std::move(type_int));

    const auto var_y =
        std::make_shared<Var>(make_shared<Token>(TokenType::IDENTIFIER, "y"));
    const auto var_decl_y =
        std::make_shared<VarDeclaration>(std::move(var_y), std::move(type_int));

    declarations->push_back(std::move(var_decl_x));
    declarations->push_back(std::move(var_decl_y));

    // Compound statement part
    auto children = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();

    // +
    const auto plus_op =
        std::make_shared<Oper>(make_shared<Token>(TokenType::OPERATOR, "+"));
    // -
    const auto minus_op =
        std::make_shared<Oper>(make_shared<Token>(TokenType::OPERATOR, "-"));

    // 2 + 3
    const auto num_2 =
        std::make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "2"));
    const auto num_3 =
        std::make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "3"));
    const auto bin_plus_op = std::make_shared<BinaryOperation>(
        std::move(num_2), std::move(plus_op), std::move(num_3));

    // x - 1
    const auto num_1 =
        std::make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "1"));
    const auto bin_minus_op = std::make_shared<BinaryOperation>(
        std::move(var_x), std::move(minus_op), std::move(num_1));

    const auto assign_x =
        std::make_shared<Assignment>(std::move(var_x), std::move(bin_plus_op));
    const auto assign_y =
        std::make_shared<Assignment>(std::move(var_y), std::move(bin_minus_op));

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

    auto s_table = make_shared<SymbolTableMock>();
    auto code_generator = std::make_shared<CodeGenerator>(s_table);
    code_generator->Interpret(program);
    auto generated_ccode = code_generator->GetCCode();

    string expected_c_code = "#include <stdio.h>\n"
                             "#include <stdlib.h>\n"
                             "#include <stdbool.h>\n"
                             "\n"
                             "int x;\n"
                             "int y;\n"
                             "// Simple\n"
                             "int main(int argc, char* argv[]) {\n"
                             "    x = (2 + 3);\n"
                             "    y = (x - 1);\n"
                             "    return 0;\n"
                             "}\n";
    std::cout << generated_ccode << endl;
    EXPECT_EQ(generated_ccode, expected_c_code);
}
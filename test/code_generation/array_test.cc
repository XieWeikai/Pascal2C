#include "code_generation/ast_adapter.h"
#include "code_generation/code_generator.h"
#include "code_generation/token_adapter.h"
#include "symbol_table_mock.h"
#include "gtest/gtest.h"
#include <memory>
#include <utility>

using namespace pascal2c::code_generation;
using namespace std;

class ArrayTest : public ::testing::Test {
  protected:
    void SetUp() override {
        name_ = "arr";
        type_ = make_shared<Type>(
            make_shared<Token>(TokenType::RESERVED, "integer"));
        std::vector<pair<int, int>> bounds = {{1, 100}};
        array_type_ = make_shared<ArrayType>(type_, bounds);
        array_var_ =
            make_shared<Var>(make_shared<Token>(TokenType::IDENTIFIER, name_));
        array_ = make_shared<Array>(array_var_, bounds);
        array_declaration_ = make_shared<ArrayDeclaration>(array_, array_type_);
        // Expected C array declaration statement
        expected_array_declaration_statement_ = "int arr[100];\n";

        ASSERT_NE(array_var_, nullptr);
        ASSERT_NE(array_, nullptr);
        ASSERT_NE(type_, nullptr);
        ASSERT_NE(array_type_, nullptr);
        ASSERT_NE(array_declaration_, nullptr);
        ASSERT_EQ(name_, "arr");
    }
    void TearDown() override {}

    string expected_array_declaration_statement_;
    string name_;
    shared_ptr<ArrayDeclaration> array_declaration_;
    shared_ptr<Array> array_;
    shared_ptr<Var> array_var_;
    shared_ptr<ArrayType> array_type_;
    shared_ptr<Type> type_;
};

TEST_F(ArrayTest, ArrayCreation) {
    auto array_declaration_root = array_declaration_;
    auto s_table = make_shared<SymbolTableMock>();
    CodeGenerator code_generator(s_table);
    code_generator.Interpret(array_declaration_root);
    auto generated_ccode = code_generator.GetCCode();
    cout << generated_ccode << endl;
    ASSERT_EQ(generated_ccode, expected_array_declaration_statement_);
}
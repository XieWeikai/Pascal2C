#include "code_generation/ast_adapter.h"
#include "code_generation/code_generator.h"
#include "code_generation/token_adapter.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>

using namespace pascal2c::code_generation;
using namespace std;

class ConstDeclarationTest : public ::testing::Test {
  protected:
    void SetUp() override {
        const_type_ = make_shared<ConstType>(
            make_shared<Token>(TokenType::RESERVED, "character"));
        var_ =
            make_shared<Var>(make_shared<Token>(TokenType::IDENTIFIER, "K_A"));
        const_decl_ = make_shared<ConstDeclaration>(var_, const_type_);
    }
    void TearDown() override {}

    string expected_ccode_ = "const char K_A;\n";
    shared_ptr<ConstDeclaration> const_decl_;
    shared_ptr<ConstType> const_type_;
    shared_ptr<Var> var_;
};

TEST_F(ConstDeclarationTest, ConstDeclarationCreation) {
    EXPECT_EQ(const_type_->GetType(), "char");
    EXPECT_EQ(var_->GetName(), "K_A");
    CodeGenerator code_generator;
    code_generator.Interpret(const_decl_);
    auto ccode = code_generator.GetCCode();
    cout << ccode;
    ASSERT_EQ(ccode, expected_ccode_);
}
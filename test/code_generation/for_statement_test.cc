#include "code_generation/ast_adapter.h"
#include "code_generation/code_generator.h"
#include "code_generation/token_adapter.h"
#include "gtest/gtest.h"
#include <memory>
using namespace pascal2c::code_generation;
using namespace std;

class ForStatementTest : public ::testing::Test {
  protected:
    void SetUp() override {
        expected_ccode_ = R"(for (i = 1; i <= 10; i++) {
    a = (a + i);
}
)";

        auto a_ =
            make_shared<Var>(make_shared<Token>(TokenType::IDENTIFIER, "a"));
        auto for_var_ =
            make_shared<Var>(make_shared<Token>(TokenType::IDENTIFIER, "i"));
        auto plus_ =
            make_shared<Oper>(make_shared<Token>(TokenType::OPERATOR, "+"));
        auto start_ =
            make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "1"));
        auto end_ =
            make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "10"));

        auto bin_op_ = make_shared<BinOp>(a_, plus_, for_var_);
        auto assign_ = make_shared<Assign>(a_, bin_op_);
        auto body_ =
            make_shared<Compound>(std::vector<shared_ptr<ASTNode>>{assign_});

        for_statement_ =
            make_shared<ForStatement>(for_var_, start_, end_, body_);
    }

    string expected_ccode_;
    shared_ptr<ForStatement> for_statement_;
};

TEST_F(ForStatementTest, ForStatementCreation) {
    CodeGenerator cg;
    cg.Interpret(for_statement_);
    cout << cg.GetCCode();
    EXPECT_EQ(cg.GetCCode(), expected_ccode_);
}
#include "code_generation/ast_adapter.h"
#include "code_generation/code_generator.h"
#include "code_generation/token_adapter.h"
#include "symbol_table_mock.h"
#include "gtest/gtest.h"
#include <memory>
using namespace pascal2c::code_generation;
using namespace std;

class IfStatementTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // condition: a == b
        auto var_a =
            make_shared<Var>(make_shared<Token>(TokenType::IDENTIFIER, "a"));
        auto var_b =
            make_shared<Var>(make_shared<Token>(TokenType::IDENTIFIER, "b"));
        auto eq =
            make_shared<Oper>(make_shared<Token>(TokenType::OPERATOR, "="));
        auto a_eq_b = make_shared<BinaryOperation>(var_a, eq, var_b);
        auto condition = a_eq_b;
        auto then_branch = make_shared<Compound>();
        auto else_branch = make_shared<Compound>();

        // a = 1 + 2;
        auto num_1 =
            make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "1"));
        auto num_2 =
            make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "2"));
        auto plus =
            make_shared<Oper>(make_shared<Token>(TokenType::OPERATOR, "+"));
        auto then_binop = make_shared<BinaryOperation>(num_1, plus, num_2);
        auto then_assign = make_shared<Assignment>(var_a, then_binop);
        then_branch->AddChild(then_assign);

        // a = 2 * 3;
        auto num_3 =
            make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "3"));
        auto mul =
            make_shared<Oper>(make_shared<Token>(TokenType::OPERATOR, "*"));
        auto else_binop = make_shared<BinaryOperation>(num_2, mul, num_3);
        auto else_assign = make_shared<Assignment>(var_a, else_binop);
        else_branch->AddChild(else_assign);

        if_statement_ =
            make_shared<IfStatement>(condition, then_branch, else_branch);
    }

    string expected_ccode_ = R"(if ((a == b)) {
    a = (1 + 2);
} else {
    a = (2 * 3);
}
)";
    shared_ptr<IfStatement> if_statement_;
};

TEST_F(IfStatementTest, IfStatementCreation) {
    auto s_table = make_shared<SymbolTableMock>();
    CodeGenerator cg(s_table);
    cg.Interpret(if_statement_);
    auto ccode = cg.GetCCode();
    cout << ccode;
    EXPECT_EQ(ccode, expected_ccode_);
}
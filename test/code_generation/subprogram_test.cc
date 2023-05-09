#include "code_generation/ast_adapter.h"
#include "code_generation/code_generator.h"
#include "code_generation/token_adapter.h"
#include "symbol_table_mock.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>
#include <utility>
#include <vector>

using namespace pascal2c::code_generation;
using namespace std;
using ::testing::Return;

class SubprogramTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // Set name of subprogram
        subprogram_name_ = "test_subprogram";

        // Set args for subprogram
        var_by_reference = make_shared<Var>(
            make_shared<Token>(TokenType::IDENTIFIER, "by_reference"));
        auto type_by_reference = make_shared<Type>(
            make_shared<Token>(TokenType::RESERVED, "integer"));
        auto arg_by_reference =
            make_shared<Argument>(var_by_reference, type_by_reference, true);

        var_by_value = make_shared<Var>(
            make_shared<Token>(TokenType::IDENTIFIER, "by_value"));
        auto array_by_value = make_shared<Array>(var_by_value);
        auto type_by_value = make_shared<Type>(
            make_shared<Token>(TokenType::RESERVED, "integer"));
        auto array_bounds_by_value =
            std::vector<pair<int, int>>{make_pair(3, 5)};
        auto array_type_by_value =
            make_shared<ArrayType>(type_by_value, array_bounds_by_value);
        auto arg_by_value =
            make_shared<Argument>(array_by_value, array_type_by_value, false);
        std::vector<shared_ptr<Argument>> args = {arg_by_reference,
                                                  arg_by_value};

        // Construct body block for subprogram
        auto compound = make_shared<Compound>();
        // *by_reference = by_value * 1;
        auto bin_op = make_shared<BinaryOperation>(
            var_by_value,
            make_shared<Oper>(make_shared<Token>(TokenType::OPERATOR, "*")),
            make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "1")));
        auto assign_by_reference =
            make_shared<Assignment>(var_by_reference, bin_op);

        // by_value[3] = 5;
        auto array_access_by_value_3 = make_shared<ArrayAccess>(
            array_by_value, std::vector<shared_ptr<ASTNode>>{make_shared<Num>(
                                make_shared<Token>(TokenType::NUMBER, "3"))});
        auto assign_by_value = make_shared<Assignment>(
            array_access_by_value_3,
            make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "5")));
        compound->AddChild(assign_by_reference);
        compound->AddChild(assign_by_value);
        auto body = make_shared<Block>(compound);
        subprogram_ = make_shared<Subprogram>(subprogram_name_, args, body);
    }

    shared_ptr<Var> var_by_reference;
    shared_ptr<Var> var_by_value;
    string expected_ccode_ =
        R"(void test_subprogram(/* Is Reference */int *by_reference, int[3] by_value) {
    *by_reference = (by_value * 1);
    by_value[3] = 5;
}
)";
    string subprogram_name_;
    shared_ptr<Subprogram> subprogram_;
};

TEST_F(SubprogramTest, SubprogramDeclaration) {
    // Mock symbol table used for IsReference Lookup
    auto s_table = make_shared<SymbolTableMock>();
    EXPECT_CALL(*s_table, GetCurrentScope()).WillOnce(Return("global"));
    EXPECT_CALL(*s_table, SetCurrentScope(subprogram_name_));
    EXPECT_CALL(*s_table, SetCurrentScope("global"));
    CodeGenerator cg(s_table);
    EXPECT_CALL(*s_table, IsReference(var_by_reference->GetName()))
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*s_table, IsReference(var_by_value->GetName()))
        .WillRepeatedly(Return(false));
    cg.Interpret(subprogram_);
    auto code = cg.GetCCode();
    cout << code << endl;
    ASSERT_EQ(code, expected_ccode_);
}
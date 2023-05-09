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

class FunctionTest : public ::testing::Test {
  protected:
    void SetUp() override { // Set name of function
        function_name_ = "test_function";

        // Set args for function
        var_by_reference = make_shared<Var>(
            make_shared<Token>(TokenType::IDENTIFIER, "by_reference"), true);
        auto type_by_reference =
            make_shared<Type>(make_shared<Token>(TokenType::RESERVED, "char"));
        auto arg_by_reference =
            make_shared<Argument>(var_by_reference, type_by_reference, true);
        var_by_value = make_shared<Var>(
            make_shared<Token>(TokenType::IDENTIFIER, "by_value"));
        auto array_by_value = make_shared<Array>(var_by_value);
        auto type_by_value =
            make_shared<Type>(make_shared<Token>(TokenType::RESERVED, "int"));
        auto array_bounds_by_value =
            std::vector<pair<int, int>>{make_pair(3, 5), make_pair(7, 16)};
        auto array_type_by_value =
            make_shared<ArrayType>(type_by_value, array_bounds_by_value);
        auto arg_by_value =
            make_shared<Argument>(array_by_value, array_type_by_value, false);
        std::vector<shared_ptr<Argument>> args = {
            arg_by_value,
            arg_by_reference,
        };

        // Construct body block for function
        auto compound = make_shared<Compound>();
        // *by_reference = (by_value[5][9] + by_value[0][19]);
        // by_value[5][9]
        auto array_access_by_value_5_9 = make_shared<ArrayAccess>(
            array_by_value,
            std::vector<shared_ptr<ASTNode>>{
                make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "5")),
                make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "9"))});
        // by_value[0][19]
        auto array_access_by_value_0_19 = make_shared<ArrayAccess>(
            array_by_value,
            std::vector<shared_ptr<ASTNode>>{
                make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "0")),
                make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "19"))});
        // BinaryOperation
        auto bin_op = make_shared<BinaryOperation>(
            array_access_by_value_5_9,
            make_shared<Oper>(make_shared<Token>(TokenType::OPERATOR, "+")),
            array_access_by_value_0_19);
        // Assignment
        auto assign_by_reference =
            make_shared<Assignment>(var_by_reference, bin_op);

        // by_value[3][4] = 5;
        auto array_access_by_value_3_4 = make_shared<ArrayAccess>(
            array_by_value,
            std::vector<shared_ptr<ASTNode>>{
                make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "3")),
                make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "4"))});
        auto assign_by_value = make_shared<Assignment>(
            array_access_by_value_3_4,
            make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "5")));

        // test_function = by_value[3][4];
        auto var_return = make_shared<Var>(
            make_shared<Token>(TokenType::IDENTIFIER, function_name_), false,
            true);
        auto assign_return =
            make_shared<Assignment>(var_return, array_access_by_value_3_4);
        compound->AddChild(assign_by_reference);
        compound->AddChild(assign_by_value);
        compound->AddChild(assign_return);
        auto body = make_shared<Block>(compound);
        function_ = make_shared<Function>(
            function_name_,
            make_shared<Type>(make_shared<Token>(TokenType::RESERVED, "char")),
            args, body);
    }

    string expected_ccode_ =
        R"(char test_function(int[3][10] by_value, /* Is Reference */char *by_reference) {
    char ret_test_function;/* Auto Generated */
    *by_reference = (by_value[5][9] + by_value[0][19]);
    by_value[3][4] = 5;
    ret_test_function = by_value[3][4];
    return ret_test_function;/* Auto Generated */
}
)";

    shared_ptr<Var> var_by_reference;
    shared_ptr<Var> var_by_value;
    string function_name_;
    shared_ptr<Function> function_;
};

TEST_F(FunctionTest, FunctionDeclarationTest) {
    auto s_table = make_shared<SymbolTableMock>();
    EXPECT_CALL(*s_table, GetCurrentScope()).WillOnce(Return("global"));
    EXPECT_CALL(*s_table, SetCurrentScope(function_name_));
    EXPECT_CALL(*s_table, SetCurrentScope("global"));
    CodeGenerator cg(s_table);
    EXPECT_CALL(*s_table, IsReference(function_name_))
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*s_table, IsReference(var_by_reference->GetName()))
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*s_table, IsReference(var_by_value->GetName()))
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*s_table, IsReturnVar(function_name_))
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*s_table, IsReturnVar(var_by_reference->GetName()))
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*s_table, IsReturnVar(var_by_value->GetName()))
        .WillRepeatedly(Return(false));
    cg.Interpret(function_);
    auto code = cg.GetCCode();
    cout << code << endl;
    ASSERT_EQ(code, expected_ccode_);
}
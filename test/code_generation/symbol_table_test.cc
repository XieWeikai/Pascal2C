#include "code_generation/ast_adapter.h"
#include "code_generation/symbol_item.h"
#include "symbol_table_mock.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <cstddef>
#include <memory>

namespace pascal2c {
namespace code_generation {
using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

TEST(SymbolTableTest, MockSymbolTableTest) {
    auto x = std::make_shared<Var>("x");

    auto symbol_table_mock = std::make_shared<SymbolTableMock>();
    auto symbol_scope_mock = std::make_shared<SymbolScopeMock>();

    EXPECT_CALL(*symbol_table_mock, IsReference(x->GetValue()))
        .WillOnce(Return(false));
    EXPECT_CALL(*symbol_table_mock, Lookup("x"))
        .WillOnce(Return(std::make_shared<SymbolItem>("x", x)));
    EXPECT_CALL(*symbol_scope_mock, Lookup("x"))
        .WillOnce(Return(std::make_shared<SymbolItem>("x", x)));

    symbol_table_mock->AddVariable(x, false);
    symbol_scope_mock->Lookup(x->GetValue());
    auto item_x = symbol_table_mock->Lookup(x->GetValue());
    EXPECT_FALSE(symbol_table_mock->IsReference(x->GetValue()));
    EXPECT_FALSE(item_x->IsReference());
}
} // namespace code_generation
} // namespace pascal2c
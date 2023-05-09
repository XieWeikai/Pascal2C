#include "code_generation/ast_adapter.h"
#include "code_generation/symbol_item.h"
#include "symbol_table_mock.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <cstddef>
#include <memory>

namespace pascal2c {
namespace code_generation {
using ::testing::Return;

TEST(SymbolTableTest, MockSymbolTableTest) {
    auto x = std::make_shared<Var>("x");

    auto symbol_table_mock = std::make_shared<SymbolTableMock>();
    auto symbol_scope_mock = std::make_shared<SymbolScopeMock>();

    EXPECT_CALL(*symbol_table_mock, IsReference(x->GetName()))
        .WillOnce(Return(false));
    EXPECT_CALL(*symbol_table_mock, Lookup("x"))
        .WillOnce(Return(std::make_shared<SymbolItem>("x", false, false)));
    EXPECT_CALL(*symbol_scope_mock, Lookup("x"))
        .WillOnce(Return(std::make_shared<SymbolItem>("x", false, false)));

    symbol_scope_mock->Lookup(x->GetName());
    auto item_x = symbol_table_mock->Lookup(x->GetName());
    EXPECT_FALSE(symbol_table_mock->IsReference(x->GetName()));
    EXPECT_FALSE(item_x->IsReference());
}
} // namespace code_generation
} // namespace pascal2c
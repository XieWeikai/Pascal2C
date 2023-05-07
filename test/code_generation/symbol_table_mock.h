#pragma once
#include "code_generation/abstract_symbol_table_adapter.h"
#include "code_generation/ast_adapter.h"
#include "code_generation/symbol_item.h"
#include "code_generation/token_adapter.h"
#include "gmock/gmock.h"
#include <memory>

namespace pascal2c {
namespace code_generation {
using ::std::shared_ptr;
using ::std::string;

class SymbolScopeMock : public ISymbolScope {
  public:
    MOCK_METHOD(const shared_ptr<SymbolItem>, Lookup, (const string &name),
                (const, override));
    MOCK_METHOD(bool, IsReference, (const string &name), (const, override));
    MOCK_METHOD(bool, IsReturnVar, (const string &name), (const, override));
};

class SymbolTableMock : public ISymbolTable {
  public:
    MOCK_METHOD(bool, IsReference, (const string &name), (const, override));
    MOCK_METHOD(bool, IsReturnVar, (const string &name), (const, override));
    MOCK_METHOD(void, SetCurrentScope, (const string &scope_name), (override));
    MOCK_METHOD(const string, GetCurrentScope, (), (override));
    MOCK_METHOD(const shared_ptr<SymbolItem>, Lookup, (const string &name),
                (const, override));
};
} // namespace code_generation
} // namespace pascal2c
#pragma once
#include "code_generation/ast_adapter.h"
#include "symbol_item.h"
#include <memory>

namespace pascal2c {
namespace code_generation {
/**
 * @brief Abstract base class for SymbolScope for implementing a Mock Class for
 * testing before the real SymbolScope is implemented
 *
 */
class ISymbolScope {
  public:
    virtual ~ISymbolScope() = default;
    virtual const shared_ptr<SymbolItem> Lookup(const string &name) const = 0;
    virtual void AddVariable(const shared_ptr<ASTNode> &node,
                             bool is_reference) = 0;
};

/**
 * @brief Abstract base class for SymbolTable.
 *
 */
class ISymbolTable {
  public:
    virtual ~ISymbolTable() = default;
    virtual void AddVariable(const shared_ptr<ASTNode> &node,
                             bool is_reference) = 0;
    virtual bool IsReference(const string &name) const = 0;
    virtual void SetCurrentScope(const string &scope_name) = 0;
    virtual const shared_ptr<SymbolItem> Lookup(const string &name) const = 0;
};
} // namespace code_generation
} // namespace pascal2c
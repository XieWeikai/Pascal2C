#pragma once
#include "ast_adapter.h"
#include <string>
namespace pascal2c {
namespace code_generation {
using ::std::string;

class SymbolItem {
  public:
    SymbolItem(const string &name,
               const shared_ptr<ASTNode> &symbol_table_item_astnode,
               bool is_reference = false)
        : name_(name), node_(symbol_table_item_astnode),
          is_reference_(is_reference) {}
    bool IsReference() const { return is_reference_; }
    const shared_ptr<ASTNode> GetNode() const { return node_; }

  private:
    string name_;
    bool is_reference_;
    shared_ptr<ASTNode> node_;
};

} // namespace code_generation
} // namespace pascal2c
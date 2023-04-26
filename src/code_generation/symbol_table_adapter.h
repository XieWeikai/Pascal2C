#pragma once
#include "code_generation/ast_adapter.h"
#include "code_generation/token_adapter.h"
#include "semantic_analysis/semantic_analysis.h"
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>
namespace pascal2c {
namespace code_generation {
using ::std::shared_ptr;
using ::std::string;
using ::std::unordered_map;

class SymbolItem {
  public:
    SymbolItem(string name, bool is_reference = false)
        : name_(name), is_reference_(is_reference) {}
    bool IsReference() const { return is_reference_; }
    const shared_ptr<ASTNode> GetNode() const { return node_; }

  private:
    string name_;
    bool is_reference_;
    shared_ptr<ASTNode> node_;
};

class SymbolScope {
  public:
    SymbolScope() {}
    const shared_ptr<SymbolItem> &Lookup(const string &name) const;
    void AddVariable(const string &name, bool is_reference);

  private:
    unordered_map<string, shared_ptr<SymbolItem>> table_;
};

class SymbolTable {
  public:
    SymbolTable() {}
    void AddVariable(const string &name, bool is_reference);
    bool IsReference(const string &name) const;
    void SetCurrentScope(const string &scope_name);
    const shared_ptr<SymbolItem> &Query(const string &name) const;

  private:
    shared_ptr<SymbolScope> current_scope_;
    shared_ptr<analysiser::nameTable> name_table_;
};
} // namespace code_generation
} // namespace pascal2c
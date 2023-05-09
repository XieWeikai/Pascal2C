#ifndef PASCAL2C_SRC_CODE_GENERATION_SYMBOL_TABLE_ADAPTER_H_
#define PASCAL2C_SRC_CODE_GENERATION_SYMBOL_TABLE_ADAPTER_H_
#pragma once
#include "abstract_symbol_table_adapter.h"
#include "code_generation/ast_adapter.h"
#include "code_generation/token_adapter.h"
#include "semantic_analysis/semantic_analysis.h"
#include "symbol_item.h"
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

class SymbolScope : ISymbolScope {
  public:
    SymbolScope(
        const shared_ptr<symbol_table::SymbolTableBlock> &symbol_table_block)
        : symbol_table_block_(symbol_table_block) {}
    bool IsReference(const string &name) const override;
    bool IsReturnVar(const string &name) const override;
    const shared_ptr<SymbolItem> Lookup(const string &name) const override;

  private:
    unordered_map<string, shared_ptr<SymbolItem>> table_;
    shared_ptr<symbol_table::SymbolTableBlock> symbol_table_block_;
};

class SymbolTable : ISymbolTable {
  public:
    SymbolTable()
        : name_table_(
              std::shared_ptr<analysiser::nameTable>(analysiser::GetTable())) {}
    bool IsReference(const string &name) const override;
    bool IsReturnVar(const string &name) const override;
    void SetCurrentScope(const string &scope_name) override;
    const string GetCurrentScope() override { return current_scope_name_; }
    const shared_ptr<SymbolItem> Lookup(const string &name) const override;
    void AddLineNo(const string &name, int lineno) {
        name_lineno_map_.insert({name, lineno});
    }

  private:
    shared_ptr<SymbolScope> current_scope_;
    string current_scope_name_;
    shared_ptr<analysiser::nameTable> name_table_;
    unordered_map<string, int> name_lineno_map_;
};
} // namespace code_generation
} // namespace pascal2c
#endif
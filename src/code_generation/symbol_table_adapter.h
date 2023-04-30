#ifdef testing
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

class SymbolTableConverter {
  public:
    const symbol_table::ItemType
    StringTypeToSymbolTableType(const string &type) const;
    const shared_ptr<ASTNode> SymbolTableItemToASTNode(
        const shared_ptr<symbol_table::SymbolTableItem> &symbol_table_item)
        const;

  private:
    unordered_map<string, symbol_table::ItemType>
        string_type_to_symbol_table_type_map_;
};

class SymbolScope : ISymbolScope {
  public:
    SymbolScope(
        const shared_ptr<symbol_table::SymbolTableBlock> &symbol_table_block,
        shared_ptr<SymbolTableConverter> &symbol_table_converter)
        : symbol_table_block_(symbol_table_block),
          symbol_type_converter_(symbol_table_converter) {}
    const shared_ptr<SymbolItem> Lookup(const string &name) const override;
    void AddVariable(const string &name, bool is_reference) override;

  private:
    symbol_table::ItemType StringTypeToSymbolTable(const string &type) const;
    shared_ptr<SymbolTableConverter> symbol_type_converter_;
    unordered_map<string, shared_ptr<SymbolItem>> table_;
    shared_ptr<symbol_table::SymbolTableBlock> symbol_table_block_;
};

class SymbolTable : ISymbolTable {
  public:
    SymbolTable() {}
    void AddVariable(const string &name, bool is_reference) override;
    bool IsReference(const string &name) const override;
    void SetCurrentScope(const string &scope_name) override;
    const shared_ptr<SymbolItem> Lookup(const string &name) const override;

  private:
    shared_ptr<SymbolScope> current_scope_;
    shared_ptr<analysiser::nameTable> name_table_;
};
} // namespace code_generation
} // namespace pascal2c
#endif
#endif
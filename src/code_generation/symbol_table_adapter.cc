#include "symbol_table_adapter.h"
#include "code_generation/ast_adapter.h"
#include "semantic_analysis/symbol_table.h"
#include <cstddef>
#include <memory>
#include <stdexcept>

namespace pascal2c {
namespace code_generation {
using ::std::make_shared;
using ::std::shared_ptr;

// SymbolTypeConverter
const symbol_table::ItemType
SymbolTableConverter::StringTypeToSymbolTableType(const string &type) const {
    auto symbol_table_type = string_type_to_symbol_table_type_map_.find(type);
    if (symbol_table_type == string_type_to_symbol_table_type_map_.end()) {
        throw std::runtime_error("Unknown symbol type when converting to "
                                 "corresponding symbol_type::ItemType: " +
                                 type);
    }
    return symbol_table_type->second;
}

// SymbolScope
/**
 * @brief Convert symbol_table::SymbolTableItem to corresponding ASTNode
 *
 * @param symbol_table_item
 * @return const shared_ptr<ASTNode>
 */
const shared_ptr<ASTNode> SymbolTableConverter::SymbolTableItemToASTNode(
    const shared_ptr<symbol_table::SymbolTableItem> &symbol_table_item) const {
    throw std::runtime_error("Symbol_table::SymbolTableItem not implemented");
}

symbol_table::ItemType
SymbolScope::StringTypeToSymbolTable(const string &type) const {
    return symbol_type_converter_->StringTypeToSymbolTableType(type);
}

const shared_ptr<SymbolItem> SymbolScope::Lookup(const string &name) const {
    auto symbol_table_item =
        make_shared<symbol_table::SymbolTableItem>(NULL, name, false, false);

    if (!symbol_table_block_->Query(*symbol_table_item)) {
        throw std::runtime_error("Unknown symbol table item name " + name);
    }

    auto symbol_table_item_astnode =
        symbol_type_converter_->SymbolTableItemToASTNode(symbol_table_item);

    return make_shared<SymbolItem>(name, symbol_table_item_astnode);
}

// SymbolTable
void SymbolTable::SetCurrentScope(const string &scope_name) {
    auto symbol_table_block = make_shared<symbol_table::SymbolTableBlock>();
    if (!name_table_->Query(scope_name, symbol_table_block)) {
        throw std::runtime_error("Unknown scope name " + scope_name);
    }
    if (current_scope_) {
        current_scope_.reset();
    }
    current_scope_ = make_shared<SymbolScope>(symbol_table_block);
}

void SymbolTable::AddVariable(const shared_ptr<ASTNode> &node, bool is_reference) {

}

bool SymbolTable::IsReference(const string &name) const {
    
}

const std::shared_ptr<SymbolItem>
SymbolTable::Lookup(const string &name) const {
    return current_scope_->Lookup(name);
}

bool SymbolTable::IsReference(const string &name) const {
    const auto &item = current_scope_->Lookup(name);
    return item->IsReference();
}

} // namespace code_generation
} // namespace pascal2c

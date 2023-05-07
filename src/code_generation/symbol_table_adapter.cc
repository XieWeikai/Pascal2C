#include "symbol_table_adapter.h"
#include "code_generation/ast_adapter.h"
#include "semantic_analysis/errors.h"
#include "semantic_analysis/symbol_table.h"
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <vector>

namespace pascal2c {
namespace code_generation {
using ::std::make_shared;
using ::std::shared_ptr;

// SymbolScope
bool SymbolScope::IsReference(const string &name) const {
    auto item = this->Lookup(name);
    return item->IsReference();
}

bool SymbolScope::IsReturnVar(const string &name) const {
    auto item = this->Lookup(name);
    return item->IsReturnVar();
}

const shared_ptr<SymbolItem> SymbolScope::Lookup(const string &name) const {
    auto st_item = make_shared<symbol_table::SymbolTableItem>(
        symbol_table::MegaType(), name, false, false,
        std::vector<symbol_table::SymbolTablePara>());

    auto err = symbol_table_block_->Query(*st_item);
    if (err != saERRORS::NO_ERROR) {
        throw std::runtime_error(saERRORS::toString(err) +
                                 ": Unknown symbol table item name " + name);
    }

    return make_shared<SymbolItem>(name, !st_item->is_var(),
                                   (st_item->is_var() && st_item->is_func()));
}

// SymbolTable
void SymbolTable::SetCurrentScope(const string &scope_name) {
    auto st_block = make_shared<symbol_table::SymbolTableBlock>();
    if (!name_table_->Query(scope_name, st_block)) {
        throw std::runtime_error("Unknown scope name " + scope_name);
    }
    if (current_scope_) {
        current_scope_.reset();
    }
    current_scope_ = make_shared<SymbolScope>(st_block);
    current_scope_name_ = scope_name;
}

const std::shared_ptr<SymbolItem>
SymbolTable::Lookup(const string &name) const {
    return current_scope_->Lookup(name);
}

bool SymbolTable::IsReference(const string &name) const {
    return current_scope_->IsReference(name);
}

bool SymbolTable::IsReturnVar(const string &name) const {
    return current_scope_->IsReturnVar(name);
}

} // namespace code_generation
} // namespace pascal2c
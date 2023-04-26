#include "symbol_table_adapter.h"
#include "code_generation/ast_adapter.h"
#include "semantic_analysis/symbol_table.h"
#include <memory>

namespace pascal2c {
namespace code_generation {
using ::std::make_shared;
void SymbolTable::SetCurrentScope(const string &scope_name) {
    auto symbol_table_block = make_shared<symbol_table::SymbolTableBlock>();
    if (!)
}

void SymbolTable::AddVariable(const string &name, bool is_reference) {
    auto row = make_shared<Row>(name, is_reference);
    table_[name] = row;
}

const std::shared_ptr<ASTNode> SymbolTable::Query(const string &name) {}

bool SymbolTable::IsReference(const string &name) const {
    const auto &it = table_.find(name);
    if (it != table_.end()) {
        return it->second->IsReference();
    }
    return false;
}
} // namespace code_generation
} // namespace pascal2c
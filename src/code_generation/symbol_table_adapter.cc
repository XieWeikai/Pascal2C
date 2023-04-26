#include "symbol_table_adapter.h"
#include <memory>

namespace pascal2c {
namespace code_generation {
using ::std::make_shared;
void SymbolTable::AddVariable(const string &name, bool is_reference) {
    auto row = make_shared<Row>(name, is_reference);
    table_[name] = row;
}

bool SymbolTable::IsReference(const string &name) const {
    const auto &it = table_.find(name);
    if (it != table_.end()) {
        return it->second->IsReference();
    }
    return false;
}
} // namespace code_generation
} // namespace pascal2c
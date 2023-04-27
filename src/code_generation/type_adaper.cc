#include "type_adaper.h"
#include <cstdlib>
#include <stdexcept>

namespace pascal2c {
namespace code_generation {
const string TypeToolKit::SymbolToC(const string &pascal_symbol) const {
    auto it = PascalToCTypeMap.find(pascal_symbol);
    if (it != PascalToCTypeMap.end()) {
        return it->second;
    } else {
        return pascal_symbol;
    }
}
} // namespace code_generation
} // namespace pascal2c
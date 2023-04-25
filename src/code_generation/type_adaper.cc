#include "type_adaper.h"
#include <cstdlib>
#include <stdexcept>

namespace pascal2c {
namespace code_generation {
const string TypeToolKit::TypeToC(const string &pascal_type) const {
    auto it = PascalToCTypeMap.find(pascal_type);
    if (it != PascalToCTypeMap.end()) {
        return it->second;
    } else {
        throw std::runtime_error("Pascal type with no corresponding c type: " +
                                 pascal_type);
    }
}
} // namespace code_generation
} // namespace pascal2c
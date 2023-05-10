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


VarType TypeToolKit::MergeType(VarType a , VarType b , const string& op) const {
    if (a >= VarType::STRING || b >= VarType::STRING)
        return VarType::UNDEFINED;
    
    if (op == "=="|| op == "!=" ||
        op == ">" || op == ">=" ||
        op == "<" || op == "<=" || 
        op == "&&"|| op == "||" || op == "!")
        return VarType::BOOL;

    if (a <= VarType::INT && b <= VarType::INT && op == "/")
        return VarType::REAL;
    
    return std::max(a , b);
}

} // namespace code_generation
} // namespace pascal2c
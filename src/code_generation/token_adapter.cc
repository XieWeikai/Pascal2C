#include <stdexcept>

#include "token_adapter.h"
namespace pascal2c {
namespace code_generation {
const string TokenType::ToString() const {
    auto it = TokenTypeStringMap.find(value_);
    if (it != TokenTypeStringMap.end()) {
        return it->second;
    } else {
        throw std::runtime_error("Invalid token type");
    }
}
} // namespace lexer
} // namespace pascal2c

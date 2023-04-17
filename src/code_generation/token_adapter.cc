#include <stdexcept>

#include "token_adapter.h"
namespace pascal2c {
namespace code_generation {
const string TokenTypeToString(const TokenType &token_type) {
    switch (token_type) {

    default:
        throw std::runtime_error("Invalid token type");
    }
}
} // namespace lexer
} // namespace pascal2c

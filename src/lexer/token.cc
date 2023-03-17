#include <stdexcept>

#include "token.h"
namespace pascal2c {
namespace lexer {
const string TokenTypeToString(const TokenType &token_type) {
    switch (token_type) {

    default:
        throw std::runtime_error("Invalid token type");
    }
}
} // namespace lexer
} // namespace pascal2c

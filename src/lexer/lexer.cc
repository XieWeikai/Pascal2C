#include <cctype>
#include <stdexcept>
#include <string>

#include "lexer.h"
#include "token.h"

namespace pascal2c {
namespace lexer {
using string = ::std::string;
void Lexer::Error() const { throw std::runtime_error("Invalid character"); }

void Lexer::Advance() {
    pos_++;
    if (pos_ > source_code_.size() - 1) {
        current_char_ = kEol;
    } else {
        current_char_ = source_code_[pos_];
    }
}

void Lexer::SkipWhiteSpace() {
    while (current_char_ != kEol && std::isspace(current_char_)) {
        Advance();
    }
}

int Lexer::Integer() {
    string result;
    while (current_char_ != kEol && std::isdigit(current_char_)) {
        result += current_char_;
        Advance();
    }
    return std::stoi(result);
}

semantic::Token Lexer::GetNextToken() {
    while (current_char_ != kEol) {
        // Skip whitespace
        if (std::isspace(current_char_)) {
            SkipWhiteSpace();
            continue;
        }

        // Read integer token
        if (std::isdigit(current_char_)) {
            return semantic::Token(INTEGER, std::to_string(Integer()));
        }

        // Read +
        if (current_char_ == '+') {
            Advance();
            return Token(PLUS, "+");
        }

        // Implement other cases for the supported tokens

        // Parse error
        Error();
    }

    return semantic::Token(EOF_, "");
}

} // namespace lexer
} // namespace pascal2c

#ifndef PASCAL2C_SRC_LEXER_LEXER_H_
#define PASCAL2C_SRC_LEXER_LEXER_H_
#include <string>

#include "code_generation/ast_interface.h"
#include "token.h"

namespace pascal2c {
namespace lexer {
using string = ::std::string;
class Lexer {
  public:
    explicit Lexer(const string &source_code)
        : source_code_(source_code), pos_(0), current_char_(source_code_[0]) {}
    const lexer::Token GetNextToken();

  private:
    string source_code_;
    size_t pos_;
    char current_char_;
    const char kEol = '\0';

    void Error() const;
    void Advance();
    void SkipWhiteSpace();
    int Integer();
};
} // namespace lexer
} // namespace pascal2c
#endif // !PASCAL2C_SRC_LEXER_LEXER_H_

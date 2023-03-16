#ifndef PASCAL2C_SRC_LEXER_TOKEN_H_
#define PASCAL2C_SRC_LEXER_TOKEN_H_

#include <string>

namespace pascal2c {
namespace lexer {
using string = std::string;
enum TokenType { INTEGER, PLUS, MINUS, MUL, DIV, LPAREN, RPAREN, EOF_ };

class Type;
class Token {
  public:
    Token(TokenType type, const string &value) : type_(type), value_(value){};
    Token(const string &value) : value_(value){};
    Token(const Token &token) : value_(token.value_), type_(token.type_){};
    const string GetValue() const { return value_; }

  private:
    string value_;
    TokenType type_;
};

} // namespace lexer
} // namespace pascal2c
#endif // !PASCAL2C_SRC_LEXER_TOKEN_H_

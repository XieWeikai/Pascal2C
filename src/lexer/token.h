#ifndef PASCAL2C_SRC_LEXER_TOKEN_H_
#define PASCAL2C_SRC_LEXER_TOKEN_H_

#include <string>

namespace pascal2c {
namespace lexer {
using string = std::string;
enum TokenType {
    BEGIN,
    END,
    INTEGER,
    PLUS,
    MINUS,
    MUL,
    DIV,
    LPAREN,
    RPAREN,
    PROGRAM,
    BLOCK,
    IDENTIFIER,
    DECLARATION,
    TYPE,
    COMPOUND,
    ASSIGN,
    VAR,
    SEMICOLON, // ; End of line
    COLON,     // : Identifier separator
    COMMA,     // ,
    DOT,       // . End of Program
    EOF_
};

class Token {
  public:
    Token(TokenType type, const string &value) : type_(type), value_(value){};
    Token(const string &value) : value_(value){};
    Token(const Token &token) : value_(token.value_), type_(token.type_){};
    const string GetValue() const { return value_; }
    const TokenType getType() const { return type_; }

  private:
    string value_;
    TokenType type_;
};

} // namespace lexer
} // namespace pascal2c
#endif // !PASCAL2C_SRC_LEXER_TOKEN_H_

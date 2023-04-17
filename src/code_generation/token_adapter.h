#ifndef PASCAL2C_SRC_CODE_GENERATION_TOKEN_ADAPTER_H_
#define PASCAL2C_SRC_CODE_GENERATION_TOKEN_ADAPTER_H_
#include <string>
// #include "lexer/lexer.h"

namespace pascal2c {
namespace code_generation {
using string = ::std::string;
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

const string TokenTypeToString(const TokenType &token_type);
class BaseToken {
  public:
    ~BaseToken(){};
};
class Token : public BaseToken {
  public:
    Token(const TokenType type, const string value) : type_(type), value_(value) {}
    const TokenType GetType() const { return type_; }
    const string GetValue() const { return value_; }

  private:
    const TokenType type_;
    const string value_;
};
} // namespace code_generation
} // namespace pascal2c
#endif
#ifndef PASCAL2C_SRC_CODE_GENERATION_TOKEN_ADAPTER_H_
#define PASCAL2C_SRC_CODE_GENERATION_TOKEN_ADAPTER_H_
#include <string>
#include <unordered_map>

namespace pascal2c {
namespace code_generation {
using string = ::std::string;
template <typename Key, typename Tp>
using hashmap = ::std::unordered_map<Key, Tp>;

class TokenType {
  public:
    enum Value {
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

    TokenType(Value value) : value_(value) {}

    operator Value() const { return value_; }

    const string ToString() const;

  private:
    Value value_;

    const hashmap<int, string> TokenTypeStringMap = {
        {0, "BEGIN"},        {1, "END"},    {2, "INTEGER"},
        {3, "PLUS"},         {4, "MINUS"},  {5, "MUL"},
        {6, "DIV"},          {7, "LPAREN"}, {8, "RPAREN"},
        {9, "PROGRAM"},      {10, "BLOCK"}, {11, "IDENTIFIER"},
        {12, "DECLARATION"}, {13, "TYPE"},  {14, "COMPOUND"},
        {15, "ASSIGN"},      {16, "VAR"},   {17, "SEMICOLON"},
        {18, "COLON"},       {19, "COMMA"}, {20, "DOT"},
        {21, "EOF_"}};
};

const string TokenTypeToString(const TokenType &token_type);
class BaseToken {
  public:
    ~BaseToken(){};
};
class Token : public BaseToken {
  public:
    Token(const TokenType type, const string value = "")
        : type_(type), value_(value) {}
    const TokenType GetType() const { return type_; }
    const string GetValue() const { return value_; }

  private:
    const TokenType type_;
    const string value_;
};
} // namespace code_generation
} // namespace pascal2c
#endif
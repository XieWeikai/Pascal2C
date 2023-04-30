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
        OPERATOR,
        DELIMITER,
        RESERVED,
        IDENTIFIER,
        STRING,
        NUMBER,
        EOF_
    };

    TokenType(Value value) : value_(value) {}

    operator Value() const { return value_; }

    const string ToString() const;

  private:
    Value value_;

    const hashmap<int, string> TokenTypeStringMap = {
        {0, "OPERATOR"}, {1, "DELIMITER"}, {2, "RESERVED"}, {3, "IDENTIFIER"},
        {4, "STRING"},   {5, "NUMBER"},    {6, "EOF_"},
    };
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
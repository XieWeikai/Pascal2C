#ifndef PASCAL2C_SRC_LEXER_LEXER_H_
#define PASCAL2C_SRC_LEXER_LEXER_H_
#include <string>

namespace pascal2c {
namespace lexer {
using string = ::std::string;
class Lexer {
  public:
    Lexer(const string &source_code) : source_code_(source_code) {}

  private:
    string source_code_;
};
} // namespace lexer
} // namespace pascal2c
#endif // !PASCAL2C_SRC_LEXER_LEXER_H_

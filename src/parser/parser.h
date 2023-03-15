#ifndef PASCAL2C_SRC_PARSER_PARSER_H_
#define PASCAL2C_SRC_PARSER_PARSER_H_
#include "../lexer/lexer.h"
#include "../semantic/ast.h"

namespace pascal2c {
namespace parser {
class Parser {
  public:
    Parser(const lexer::Lexer *lexer) : lexer_(lexer){};
    const semantic::AST Run();

  private:
    const lexer::Lexer *lexer_;
};
} // namespace parser
} // namespace pascal2c
#endif // !PASCAL2C_SRC_PARSER_PARSER_H_

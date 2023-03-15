#ifndef PASCAL2C_SRC_PARSER_PARSER_H_
#define PASCAL2C_SRC_PARSER_PARSER_H_
#include <exception>
#include <memory>
#include <vector>

#include "../lexer/lexer.h"
#include "../semantic/ast.h"

namespace pascal2c {
namespace parser {
using string = ::std::string;
class Parser {
  public:
    explicit Parser(const lexer::Lexer *lexer) : lexer_(lexer) {
        current_token_ = lexer->GetNextToken();
    };

    void Error(const string &message) { throw std::runtime_error(message); }

    void Eat(const semantic::Type &token_type);

    std::shared_ptr<semantic::Program> Program();

    std::shared_ptr<semantic::Block> Block();

    std::vector<std::shared_ptr<semantic::VarDecl>> Declarations();

    std::shared_ptr<semantic::Type> TypeSpec();

    std::shared_ptr<semantic::Compound> CompoundStatement();

    std::shared_ptr<semantic::Assign> AssignStatement();

    std::shared_ptr<semantic::Var> variable();

  private:
    const lexer::Lexer *lexer_;
    semantic::Token current_token_;
};
} // namespace parser
} // namespace pascal2c
#endif // !PASCAL2C_SRC_PARSER_PARSER_H_

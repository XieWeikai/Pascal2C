#ifndef PASCAL2C_SRC_PARSER_PARSER_H_
#define PASCAL2C_SRC_PARSER_PARSER_H_
#include <exception>
#include <memory>
#include <vector>

#include "code_generation/ast_interface.h"
#include "lexer/lexer.h"

namespace pascal2c {
namespace parser {
using string = ::std::string;
class Parser {
  public:
    explicit Parser(const lexer::Lexer *lexer)
        : lexer_(const_cast<lexer::Lexer *>(lexer)),
          current_token_(lexer_->GetNextToken()){};

    void Error(const string &message) {
        throw std::runtime_error("Parser error: " + message);
    }

    void Eat(const lexer::TokenType &token_type);

    std::shared_ptr<semantic::Program> Program();

    std::shared_ptr<semantic::Block> Block();

    std::vector<std::shared_ptr<semantic::VarDecl>> Declarations();

    std::shared_ptr<semantic::Type> TypeSpec();

    std::shared_ptr<semantic::Compound> CompoundStatement();

    std::shared_ptr<semantic::Assign> AssignStatement();

    std::shared_ptr<semantic::ASTNode> Expression();

    std::shared_ptr<semantic::ASTNode> Factor();

    std::shared_ptr<semantic::ASTNode> Term();

    std::shared_ptr<semantic::Var> Variable();

  private:
    lexer::Lexer *lexer_;
    lexer::Token current_token_;
};
} // namespace parser
} // namespace pascal2c
#endif // !PASCAL2C_SRC_PARSER_PARSER_H_

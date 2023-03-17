#ifndef PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATION_H_
#define PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATION_H_
#include <memory>
#include <sstream>
#include <string>

#include "ast_interface.h"
#include "parser/parser.h"

namespace pascal2c {
namespace code_generation {
using string = ::std::string;
template <typename T> using vector = ::std::vector<T>;

class CodeGenerator {
  public:
    explicit CodeGenerator(std::shared_ptr<parser::Parser> parser)
        : parser_(parser) {}
    // TODO: Output of Visit save into ostream
    void Visit(const semantic::ASTRoot &ast);
    int Interpret();
    string GetCCode();

  private:
    int Visit(const std::shared_ptr<semantic::ASTNode> &node);
    int VisitBinOp(const std::shared_ptr<semantic::BinOp> &node);
    int VisitNum(const std::shared_ptr<semantic::Num> &node);
    void VisitProgram(const std::shared_ptr<semantic::Program> &node);
    void VisitBlock(const std::shared_ptr<semantic::Block> &node);
    int VisitVarDecl(const std::shared_ptr<semantic::VarDecl> &node);
    int VisitCompound(const std::shared_ptr<semantic::Compound> &node);
    int VisitAssign(const std::shared_ptr<semantic::Assign> &node);
    int VisitVar(const std::shared_ptr<semantic::Var> &node);
    int VisitNoOp(const std::shared_ptr<semantic::NoOp> &node);

    // Parser
    std::shared_ptr<parser::Parser> parser_;
    // AST root node
    semantic::ASTRoot ast_;
    // Global Scope symbols
    vector<semantic::ASTNode> global_scope_;
    // ostream
    std::stringstream ostream_;
    // Current indent level
    int indent_level_;
};
} // namespace code_generation
} // namespace pascal2c
#endif // !PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATION_H_

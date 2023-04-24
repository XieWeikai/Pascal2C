#ifndef PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATION_H_
#define PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATION_H_
#include <memory>
#include <sstream>
#include <string>

#include "ast_adapter.h"

namespace pascal2c {
namespace code_generation {
using string = ::std::string;
template <typename T> using vector = ::std::vector<T>;

class CodeGenerator {
  public:
    explicit CodeGenerator(std::shared_ptr<code_generation::ASTRoot> ast)
        : ast_(ast) {}
    // TODO: Output of Visit save into ostream
    void Visit(const code_generation::ASTRoot &ast);
    void Interpret();
    const string GetCCode() const;

  private:
    void Visit(const std::shared_ptr<code_generation::ASTNode> &node);
    void VisitBinOp(const std::shared_ptr<code_generation::BinOp> &node);
    void VisitNum(const std::shared_ptr<code_generation::Num> &node);
    void VisitProgram(const std::shared_ptr<code_generation::Program> &node);
    void VisitBlock(const std::shared_ptr<code_generation::Block> &node);
    void VisitVarDecl(const std::shared_ptr<code_generation::VarDecl> &node);
    void VisitCompound(const std::shared_ptr<code_generation::Compound> &node);
    void VisitAssign(const std::shared_ptr<code_generation::Assign> &node);
    void VisitVar(const std::shared_ptr<code_generation::Var> &node);
    void VisitNoOp(const std::shared_ptr<code_generation::NoOp> &node);

    // AST root node
    std::shared_ptr<code_generation::ASTRoot> ast_;
    // Global Scope symbols
    vector<code_generation::ASTNode> global_scope_;
    // ostream
    std::stringstream ostream_;
    // Current indent level
    int indent_level_;
};
} // namespace code_generation
} // namespace pascal2c
#endif // !PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATION_H_

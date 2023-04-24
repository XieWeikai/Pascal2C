#ifndef PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATOR_H_
#define PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATOR_H_
#include <memory>
#include <sstream>
#include <string>

#include "ast_adapter.h"
#include "visitor.h"

namespace pascal2c {
namespace code_generation {
using string = ::std::string;
template <typename T> using vector = ::std::vector<T>;

class CodeGenerator : Visitor {
  public:
    explicit CodeGenerator(std::shared_ptr<code_generation::ASTRoot> ast)
        : ast_(ast) {}
    // TODO: Output of Visit save into ostream
    void Interpret();
    const string GetCCode() const;

  private:
    void Visit(const std::shared_ptr<code_generation::ASTNode> &node) override;
    void
    VisitBinOp(const std::shared_ptr<code_generation::BinOp> &node) override;
    void VisitNum(const std::shared_ptr<code_generation::Num> &node) override;
    void VisitType(const std::shared_ptr<code_generation::Type> &node) override;
    void VisitProgram(
        const std::shared_ptr<code_generation::Program> &node) override;
    void
    VisitBlock(const std::shared_ptr<code_generation::Block> &node) override;
    void VisitVarDecl(
        const std::shared_ptr<code_generation::VarDecl> &node) override;
    void VisitCompound(
        const std::shared_ptr<code_generation::Compound> &node) override;
    void
    VisitAssign(const std::shared_ptr<code_generation::Assign> &node) override;
    void VisitVar(const std::shared_ptr<code_generation::Var> &node) override;
    void VisitNoOp(const std::shared_ptr<code_generation::NoOp> &node) override;

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

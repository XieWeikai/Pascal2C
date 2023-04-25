#ifndef PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATOR_H_
#define PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATOR_H_
#include <memory>
#include <sstream>
#include <string>

#include "ast_adapter.h"
#include "code_generation/type_adaper.h"
#include "visitor.h"

namespace pascal2c {
namespace code_generation {
using string = ::std::string;
template <typename T> using vector = ::std::vector<T>;

class CodeGenerator : Visitor {
  public:
    explicit CodeGenerator(std::shared_ptr<code_generation::ASTRoot> ast)
        : ast_(ast), type_tool_kit_() {}
    void Interpret();
    const string GetCCode() const;

  private:
    virtual void Visit(const std::shared_ptr<ASTNode> &node,
                       bool indent = false) override;
    virtual void VisitProgram(const std::shared_ptr<Program> &node) override;
    virtual void VisitBlock(const std::shared_ptr<Block> &node) override;
    virtual void
    VisitDeclaration(const std::shared_ptr<Declaration> &node) override;
    virtual void VisitVarDecl(const std::shared_ptr<VarDecl> &node) override;
    virtual void VisitCompound(const std::shared_ptr<Compound> &node) override;
    virtual void VisitBinOp(const std::shared_ptr<BinOp> &node) override;
    virtual void VisitOper(const std::shared_ptr<Oper> &node) override;
    virtual void VisitNum(const std::shared_ptr<Num> &node) override;
    virtual void VisitType(const std::shared_ptr<Type> &node) override;
    virtual void VisitAssign(const std::shared_ptr<Assign> &node) override;
    virtual void VisitVar(const std::shared_ptr<Var> &node) override;
    virtual void VisitNoOp(const std::shared_ptr<NoOp> &node) override;

    const string Indent() const;
    const string TypeToC(const string &pascal_type) const;
    const string eol_ = ";\n";

    // AST root node
    std::shared_ptr<ASTRoot> ast_;
    // Global Scope symbols
    vector<ASTNode> global_scope_;
    // ostream
    std::stringstream ostream_;
    // Current indent level
    int indent_level_;

    // TypeToolKit for type conversion
    const TypeToolKit type_tool_kit_;
};

} // namespace code_generation
} // namespace pascal2c
#endif // !PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATION_H_

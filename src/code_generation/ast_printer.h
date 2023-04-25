#ifndef PASCAL2C_SRC_CODE_GENERATION_AST_PRINTER_H_
#define PASCAL2C_SRC_CODE_GENERATION_AST_PRINTER_H_
#include <memory>
#include <sstream>
#include <string>

#include "ast_adapter.h"

namespace pascal2c {
namespace code_generation {
using ::std::string;
template <typename T> using vector = ::std::vector<T>;
using ::std::shared_ptr;
using ::std::stringstream;

class ASTPrinter {
  public:
    explicit ASTPrinter(const shared_ptr<code_generation::ASTRoot> ast)
        : ast_(ast){};
    // TODO: Output of Visit save into ostream
    void Visit();
    string ToString() const;

  private:
    void Visit(const shared_ptr<code_generation::ASTNode> &node);
    void VisitProgram(const shared_ptr<code_generation::Program> &node);
    void VisitBlock(const shared_ptr<code_generation::Block> &node);
    void VisitDeclaration(const shared_ptr<Declaration> &node);
    void VisitVarDecl(const shared_ptr<code_generation::VarDecl> &node);
    void VisitCompound(const shared_ptr<code_generation::Compound> &node);
    void VisitBinOp(const shared_ptr<code_generation::BinOp> &node);
    void VisitNum(const shared_ptr<code_generation::Num> &node);
    void VisitAssign(const shared_ptr<code_generation::Assign> &node);
    void VisitVar(const shared_ptr<code_generation::Var> &node);
    void VisitType(const shared_ptr<code_generation::Type> &node);
    void VisitNoOp(const shared_ptr<code_generation::NoOp> &node);

    // ast
    const shared_ptr<code_generation::ASTRoot> ast_;
    // ostream
    stringstream ostream_;
    // Current indent level
    int indent_level_;
};
} // namespace code_generation
} // namespace pascal2c
#endif // !PASCAL2C_SRC_CODE_GENERATION_AST_PRINTER_H_

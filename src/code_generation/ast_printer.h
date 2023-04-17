#ifndef PASCAL2C_SRC_CODE_GENERATION_AST_PRINTER_H_
#define PASCAL2C_SRC_CODE_GENERATION_AST_PRINTER_H_
#include <memory>
#include <sstream>
#include <string>

#include "ast_adapter.h"
#include "parser/parser.h"

namespace pascal2c {
namespace code_generation {
using string = ::std::string;
template <typename T> using vector = ::std::vector<T>;

class ASTPrinter {
  public:
    explicit ASTPrinter(){};
    // TODO: Output of Visit save into ostream
    void Visit(const std::shared_ptr<semantic::ASTNode> &node);

  private:
    void VisitBinOp(const std::shared_ptr<semantic::BinOp> &node);
    void VisitNum(const std::shared_ptr<semantic::Num> &node);
    void VisitProgram(const std::shared_ptr<semantic::Program> &node);
    void VisitBlock(const std::shared_ptr<semantic::Block> &node);
    void VisitVarDecl(const std::shared_ptr<semantic::VarDecl> &node);
    void VisitCompound(const std::shared_ptr<semantic::Compound> &node);
    void VisitAssign(const std::shared_ptr<semantic::Assign> &node);
    void VisitVar(const std::shared_ptr<semantic::Var> &node);
    void VisitType(const std::shared_ptr<semantic::Type> &node);
    void VisitNoOp(const std::shared_ptr<semantic::NoOp> &node);

    // ostream
    std::stringstream ostream_;
    // Current indent level
    int indent_level_;
};
} // namespace code_generation
} // namespace pascal2c
#endif // !PASCAL2C_SRC_CODE_GENERATION_AST_PRINTER_H_

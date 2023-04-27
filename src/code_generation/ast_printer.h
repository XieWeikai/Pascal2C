#ifndef PASCAL2C_SRC_CODE_GENERATION_AST_PRINTER_H_
#define PASCAL2C_SRC_CODE_GENERATION_AST_PRINTER_H_
#pragma once
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

#include "ast_adapter.h"

namespace pascal2c {
namespace code_generation {
using ::std::string;
template <typename T> using vector = ::std::vector<T>;
using ::std::shared_ptr;
using ::std::stringstream;

class ASTPrinter : Visitor {
  public:
    ASTPrinter(){};

    void Traverse(const shared_ptr<ASTNode> &node);
    string ToString() const;

  private:
    virtual void Visit(const shared_ptr<ASTNode> &node,
                       bool indent = false) = 0;
    virtual void VisitArgument(const shared_ptr<Argument> &node) = 0;
    virtual void VisitProgram(const shared_ptr<Program> &node) = 0;
    virtual void VisitSubprogram(const shared_ptr<Subprogram> &node) = 0;
    virtual void VisitFunction(const shared_ptr<Function> &node) = 0;
    virtual void VisitBlock(const shared_ptr<Block> &node) = 0;
    virtual void VisitDeclaration(const shared_ptr<Declaration> &node) = 0;
    virtual void VisitConst(const shared_ptr<Var> &node) = 0;
    virtual void
    VisitConstDeclaration(const shared_ptr<ConstDeclaration> &node) = 0;
    virtual void VisitArrayType(const shared_ptr<ArrayType> &node) = 0;
    virtual void VisitArray(const shared_ptr<Array> &node) = 0;
    virtual void
    VisitArrayDeclaration(const shared_ptr<ArrayDeclaration> &node) = 0;
    virtual void VisitVarDecl(const shared_ptr<VarDeclaration> &node) = 0;
    virtual void VisitCompound(const shared_ptr<Compound> &node) = 0;
    virtual void VisitBinOp(const shared_ptr<BinOp> &node) = 0;
    virtual void VisitOper(const shared_ptr<Oper> &node) = 0;
    virtual void VisitNum(const shared_ptr<Num> &node) = 0;
    virtual void VisitType(const shared_ptr<Type> &node) = 0;
    virtual void VisitAssign(const shared_ptr<Assign> &node) = 0;
    virtual void VisitVar(const shared_ptr<Var> &node) = 0;
    virtual void VisitNoOp(const shared_ptr<NoOp> &node) = 0;
    virtual void VisitStatement(const shared_ptr<Statement> &node) = 0;
    virtual void VisitIfStatement(const shared_ptr<IfStatement> &node) = 0;
    virtual void VisitForStatement(const shared_ptr<ForStatement> &node) = 0;

    void ErrNotImplemented() const {
        throw std::runtime_error("Not implemented");
    }

    // ostream
    stringstream ostream_;
    // Current indent level
    int indent_level_;
};
} // namespace code_generation
} // namespace pascal2c
#endif // !PASCAL2C_SRC_CODE_GENERATION_AST_PRINTER_H_

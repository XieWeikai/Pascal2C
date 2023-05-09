#ifndef PASCAL2C_SRC_CODE_GENERATION_VISITOR_H_
#define PASCAL2C_SRC_CODE_GENERATION_VISITOR_H_
#pragma once
#include "ast_adapter.h"
#include <memory>
namespace pascal2c {
namespace code_generation {
using ::std::shared_ptr;
class Visitor {
  public:
    virtual void Visit(const shared_ptr<ASTNode> &node,
                       bool indent = false) = 0;
    virtual void VisitExitStatement(const shared_ptr<ExitStatement> &node) = 0;
    virtual void VisitArgument(const shared_ptr<Argument> &node) = 0;
    virtual void VisitProgram(const shared_ptr<Program> &node) = 0;
    virtual void VisitSubprogram(const shared_ptr<Subprogram> &node) = 0;
    virtual void VisitFunction(const shared_ptr<Function> &node) = 0;
    virtual void VisitBlock(const shared_ptr<Block> &node) = 0;
    virtual void VisitDeclaration(const shared_ptr<Declaration> &node) = 0;
    virtual void
    VisitConstDeclaration(const shared_ptr<ConstDeclaration> &node) = 0;
    virtual void VisitArrayType(const shared_ptr<ArrayType> &node) = 0;
    virtual void VisitArray(const shared_ptr<Array> &node) = 0;
    virtual void
    VisitArrayDeclaration(const shared_ptr<ArrayDeclaration> &node) = 0;
    virtual void VisitArrayAccess(const shared_ptr<ArrayAccess> &node) = 0;
    virtual void VisitVarDecl(const shared_ptr<VarDeclaration> &node) = 0;
    virtual void VisitCompound(const shared_ptr<Compound> &node) = 0;
    virtual void VisitBinOp(const shared_ptr<BinaryOperation> &node) = 0;
    virtual void
    VisitUnaryOperation(const shared_ptr<UnaryOperation> &node) = 0;
    virtual void VisitOper(const shared_ptr<Oper> &node) = 0;
    virtual void VisitNum(const shared_ptr<Num> &node) = 0;
    virtual void VisitString(const shared_ptr<String> &node) = 0;
    virtual void VisitReal(const shared_ptr<Real> &node) = 0;
    virtual void VisitChar(const shared_ptr<Char> &node) = 0;
    virtual void VisitType(const shared_ptr<Type> &node) = 0;
    virtual void VisitConstType(const shared_ptr<ConstType> &node) = 0;
    virtual void VisitAssign(const shared_ptr<Assignment> &node) = 0;
    virtual void VisitVar(const shared_ptr<Var> &node) = 0;
    virtual void VisitNoOp(const shared_ptr<NoOp> &node) = 0;
    virtual void VisitStatement(const shared_ptr<Statement> &node) = 0;
    virtual void VisitIfStatement(const shared_ptr<IfStatement> &node) = 0;
    virtual void VisitForStatement(const shared_ptr<ForStatement> &node) = 0;
    virtual void
    VisitWhileStatement(const shared_ptr<WhileStatement> &node) = 0;
    virtual void VisitFunctionCall(const shared_ptr<FunctionCall> &node) = 0;
};
} // namespace code_generation
} // namespace pascal2c
#endif
#ifndef PASCAL2C_SRC_CODE_GENERATION_VISITOR_H_
#define PASCAL2C_SRC_CODE_GENERATION_VISITOR_H_
#pragma once
#include "code_generation/ast_adapter.h"
#include <memory>
namespace pascal2c {
namespace code_generation {
class Visitor {
  public:
    virtual void Visit(const std::shared_ptr<ASTNode> &node) = 0;
    virtual void VisitProgram(const std::shared_ptr<Program> &node) = 0;
    virtual void VisitBlock(const std::shared_ptr<Block> &node) = 0;
    virtual void VisitDeclaration(const std::shared_ptr<Declaration> &node) = 0;
    virtual void VisitVarDecl(const std::shared_ptr<VarDecl> &node) = 0;
    virtual void VisitCompound(const std::shared_ptr<Compound> &node) = 0;
    virtual void VisitBinOp(const std::shared_ptr<BinOp> &node) = 0;
    virtual void VisitOper(const std::shared_ptr<Oper> &node) = 0;
    virtual void VisitNum(const std::shared_ptr<Num> &node) = 0;
    virtual void VisitType(const std::shared_ptr<Type> &node) = 0;
    virtual void VisitAssign(const std::shared_ptr<Assign> &node) = 0;
    virtual void VisitVar(const std::shared_ptr<Var> &node) = 0;
    virtual void VisitNoOp(const std::shared_ptr<NoOp> &node) = 0;
};
} // namespace code_generation
} // namespace pascal2c
#endif
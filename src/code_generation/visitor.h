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
    virtual void
    VisitBinOp(const std::shared_ptr<code_generation::BinOp> &node) = 0;
    virtual void
    VisitNum(const std::shared_ptr<code_generation::Num> &node) = 0;
    virtual void
    VisitType(const std::shared_ptr<code_generation::Type> &node) = 0;
    virtual void
    VisitProgram(const std::shared_ptr<code_generation::Program> &node) = 0;
    virtual void
    VisitBlock(const std::shared_ptr<code_generation::Block> &node) = 0;
    virtual void
    VisitVarDecl(const std::shared_ptr<code_generation::VarDecl> &node) = 0;
    virtual void
    VisitCompound(const std::shared_ptr<code_generation::Compound> &node) = 0;
    virtual void
    VisitAssign(const std::shared_ptr<code_generation::Assign> &node) = 0;
    virtual void
    VisitVar(const std::shared_ptr<code_generation::Var> &node) = 0;
    virtual void
    VisitNoOp(const std::shared_ptr<code_generation::NoOp> &node) = 0;
};
} // namespace code_generation
} // namespace pascal2c
#endif
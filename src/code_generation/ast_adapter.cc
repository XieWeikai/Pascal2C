#include "ast_adapter.h"
#include "code_generation/token_adapter.h"
#include <memory>

namespace pascal2c {
namespace code_generation {
using ::std::dynamic_pointer_cast;

ASTNode::~ASTNode() {}
VarDecl::~VarDecl() {
    var_node_.reset();
    type_node_.reset();
}

Var::~Var() { token_.reset(); }
Oper::~Oper() { oper_.reset(); }
Num::~Num() { token_.reset(); }

// Program
void Program::Accept(Visitor &visitor) {
    visitor.VisitProgram(dynamic_pointer_cast<Program>(shared_from_this()));
}

// Block
void Block::Accept(Visitor &visitor) {
    visitor.VisitBlock(dynamic_pointer_cast<Block>(shared_from_this()));
}
Block::Block(const std::shared_ptr<Declaration> &declarations,
             const std::shared_ptr<Compound> &compound_statement)
    : declarations_(declarations), compound_statement_(compound_statement){};

// Declaration
void Declaration::Accept(Visitor &visitor) {
    visitor.VisitDeclaration(
        dynamic_pointer_cast<Declaration>(shared_from_this()));
}

// VarDecl
void VarDecl::Accept(Visitor &visitor) {
    visitor.VisitVarDecl(dynamic_pointer_cast<VarDecl>(shared_from_this()));
}

// Compound
Compound::Compound(const std::vector<std::shared_ptr<ASTNode>> &children)
    : children_(
          const_cast<std::vector<std::shared_ptr<ASTNode>> &>(children)){};
void Compound::Accept(Visitor &visitor) {
    visitor.VisitCompound(dynamic_pointer_cast<Compound>(shared_from_this()));
}

/**
 * add new node to children
 */
void Compound::AddChild(std::shared_ptr<ASTNode> node) {
    children_.push_back(node);
}

// BinOp
void BinOp::Accept(Visitor &visitor) {
    visitor.VisitBinOp(dynamic_pointer_cast<BinOp>(shared_from_this()));
}

// Oper
void Oper::Accept(Visitor &visitor) {
    visitor.VisitOper(dynamic_pointer_cast<Oper>(shared_from_this()));
}

// Num
void Num::Accept(Visitor &visitor) {
    visitor.VisitNum(dynamic_pointer_cast<Num>(shared_from_this()));
}

// Type
// Type::~Type() { token_.reset(); }
void Type::Accept(Visitor &visitor) {
    visitor.VisitType(dynamic_pointer_cast<Type>(shared_from_this()));
}

// Assign
void Assign::Accept(Visitor &visitor) {
    visitor.VisitAssign(dynamic_pointer_cast<Assign>(shared_from_this()));
}

// Var
void Var::Accept(Visitor &visitor) {
    visitor.VisitVar(dynamic_pointer_cast<Var>(shared_from_this()));
}

// NoOp
void NoOp::Accept(Visitor &visitor) {
    visitor.VisitNoOp(dynamic_pointer_cast<NoOp>(shared_from_this()));
}

} // namespace code_generation
} // namespace pascal2c

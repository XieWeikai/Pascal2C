#include "ast_adapter.h"
#include "code_generation/token_adapter.h"
#include <memory>

namespace pascal2c {
namespace code_generation {
using ::std::dynamic_pointer_cast;

ASTNode::~ASTNode() {}
VarDeclaration::~VarDeclaration() {
    var_node_.reset();
    type_node_.reset();
}

Var::~Var() {}
Oper::~Oper() {}
Num::~Num() {}

void Argument::Accept(Visitor &visitor) {
    visitor.VisitArgument(dynamic_pointer_cast<Argument>(shared_from_this()));
}

// Program
void Program::Accept(Visitor &visitor) {
    visitor.VisitProgram(dynamic_pointer_cast<Program>(shared_from_this()));
}

// Subprogram
void Subprogram::Accept(Visitor &visitor) {
    visitor.VisitSubprogram(
        dynamic_pointer_cast<Subprogram>(shared_from_this()));
}

// Function
void Function::Accept(Visitor &visitor) {
    visitor.VisitFunction(dynamic_pointer_cast<Function>(shared_from_this()));
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

// VarDeclaration
void VarDeclaration::Accept(Visitor &visitor) {
    visitor.VisitVarDecl(
        dynamic_pointer_cast<VarDeclaration>(shared_from_this()));
}

// ConstDeclaration
void ConstDeclaration::Accept(Visitor &visitor) {
    visitor.VisitConstDeclaration(
        dynamic_pointer_cast<ConstDeclaration>(shared_from_this()));
}

// Array
void Array::Accept(Visitor &visitor) {
    visitor.VisitArray(dynamic_pointer_cast<Array>(shared_from_this()));
}

// ArrayDeclaration
void ArrayDeclaration::Accept(Visitor &visitor) {
    visitor.VisitArrayDeclaration(
        dynamic_pointer_cast<ArrayDeclaration>(shared_from_this()));
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
Type::~Type() {}
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

void Statement::Accept(Visitor &visitor) {
    visitor.VisitStatement(dynamic_pointer_cast<Statement>(shared_from_this()));
}

void IfStatement::Accept(Visitor &visitor) {
    visitor.VisitIfStatement(
        dynamic_pointer_cast<IfStatement>(shared_from_this()));
}

void ForStatement::Accept(Visitor &visitor) {
    visitor.VisitForStatement(
        dynamic_pointer_cast<ForStatement>(shared_from_this()));
}

} // namespace code_generation
} // namespace pascal2c

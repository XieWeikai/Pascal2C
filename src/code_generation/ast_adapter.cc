#include "ast_adapter.h"
#include "token_adapter.h"
#include <memory>

namespace pascal2c {
namespace code_generation {
using ::std::dynamic_pointer_cast;

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

// ArrayType
void ArrayType::Accept(Visitor &visitor) {
    visitor.VisitArrayType(dynamic_pointer_cast<ArrayType>(shared_from_this()));
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

// ArrayAccess
void ArrayAccess::Accept(Visitor &visitor) {
    visitor.VisitArrayAccess(
        dynamic_pointer_cast<ArrayAccess>(shared_from_this()));
}

// Compound
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
void BinaryOperation::Accept(Visitor &visitor) {
    visitor.VisitBinOp(
        dynamic_pointer_cast<BinaryOperation>(shared_from_this()));
}

// Oper
void Oper::Accept(Visitor &visitor) {
    visitor.VisitOper(dynamic_pointer_cast<Oper>(shared_from_this()));
}

// Num
void Num::Accept(Visitor &visitor) {
    visitor.VisitNum(dynamic_pointer_cast<Num>(shared_from_this()));
}

// String
void String::Accept(Visitor &visitor) {
    visitor.VisitString(dynamic_pointer_cast<String>(shared_from_this()));
}

// Real
void Real::Accept(Visitor &visitor) {
    visitor.VisitReal(dynamic_pointer_cast<Real>(shared_from_this()));
}

// Char
void Char::Accept(Visitor &visitor) {
    visitor.VisitChar(dynamic_pointer_cast<Char>(shared_from_this()));
}

// Type
void Type::Accept(Visitor &visitor) {
    visitor.VisitType(dynamic_pointer_cast<Type>(shared_from_this()));
}

// ConstType
void ConstType::Accept(Visitor &visitor) {
    visitor.VisitConstType(dynamic_pointer_cast<ConstType>(shared_from_this()));
}

// Assign
void Assignment::Accept(Visitor &visitor) {
    visitor.VisitAssign(dynamic_pointer_cast<Assignment>(shared_from_this()));
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
void FunctionCall::Accept(Visitor &visitor) {
    visitor.VisitFunctionCall(
        dynamic_pointer_cast<FunctionCall>(shared_from_this()));
}

} // namespace code_generation
} // namespace pascal2c
#include <memory>
#include <stdexcept>
#include <string>

#include "code_generation/ast_interface.h"
#include "code_generator.h"
#include "lexer/token.h"

namespace pascal2c {
namespace code_generation {

int CodeGenerator::Interpret() { return Visit(parser_->Expression()); }

// TODO: Re-implement this method. Now it just visits BinOp and Num
// And lead its output into ostream_
int CodeGenerator::Visit(const std::shared_ptr<semantic::ASTRoot> &node) {
    if (std::dynamic_pointer_cast<semantic::BinOp>(node)) {
        return VisitBinOp(std::dynamic_pointer_cast<semantic::BinOp>(node));
    } else if (std::dynamic_pointer_cast<semantic::Num>(node)) {
        return VisitNum(std::dynamic_pointer_cast<semantic::Num>(node));
    }

    throw std::runtime_error("Invalid node type");
}

void CodeGenerator::VisitProgram(
    const std::shared_ptr<semantic::Program> &node) {
    ostream_ << "Program: " << node->getName();
    indent_level_++;
    Visit(node->GetBlock());
    indent_level_--;
}

void CodeGenerator::VisitBlock(const std::shared_ptr<semantic::Block> &node) {
    ostream_ << string(indent_level_, ' ') << " Block";
    indent_level_++;
    for (auto decl : node->GetDeclarations()) {
        Visit(decl);
    }
    Visit(node->GetCompoundStatement());
    indent_level_--;
}

void CodeGenerator::VisitVarDecl(
    const std::shared_ptr<semantic::VarDecl> &node) {
    ostream_ << string(indent_level_, ' ')
             << "VarDecl: " << node->GetVarNode()->GetValue() << ": "
             << node->GetTypeNode()->GetType();
}

int CodeGenerator::VisitBinOp(const std::shared_ptr<semantic::BinOp> &node) {
    if (node->getOper()->getType() == lexer::PLUS) {
        return Visit(node->getLeft());
    } else if (node->getOper()->getType() == lexer::MINUS) {
        return Visit(node->getLeft()) - Visit(node->getRight());
    } else if (node->getOper()->getType() == lexer::MUL) {
        return Visit(node->getLeft()) * Visit(node->getRight());
    } else if (node->getOper()->getType() == lexer::DIV) {
        return Visit(node->getLeft()) / Visit(node->getRight());
    }

    throw std::runtime_error("Invalid operator");
}

int CodeGenerator::VisitNum(const std::shared_ptr<semantic::Num> &node) {
    return node->getValue();
}
} // namespace code_generation
} // namespace pascal2c

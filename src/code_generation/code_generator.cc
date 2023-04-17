#include <memory>
#include <stdexcept>
#include <string>

#include "code_generation/ast_adapter.h"
#include "code_generator.h"
#include "token_adapter.h"

namespace pascal2c {
namespace code_generation {

// int CodeGenerator::Interpret() { return Visit(parser_->Expression()); }

// TODO: Re-implement this method. Now it just visits BinOp and Num
// And lead its output into ostream_
int CodeGenerator::Visit(const std::shared_ptr<code_generation::ASTRoot> &node) {
    if (std::dynamic_pointer_cast<code_generation::BinOp>(node)) {
        return VisitBinOp(std::dynamic_pointer_cast<code_generation::BinOp>(node));
    } else if (std::dynamic_pointer_cast<code_generation::Num>(node)) {
        return VisitNum(std::dynamic_pointer_cast<code_generation::Num>(node));
    }

    throw std::runtime_error("Invalid node type");
}

void CodeGenerator::VisitProgram(
    const std::shared_ptr<code_generation::Program> &node) {
    ostream_ << "Program: " << node->GetName();
    indent_level_++;
    Visit(node->GetBlock());
    indent_level_--;
}

void CodeGenerator::VisitBlock(const std::shared_ptr<code_generation::Block> &node) {
    ostream_ << string(indent_level_, ' ') << " Block";
    indent_level_++;
    for (auto decl : node->GetDeclarations()) {
        Visit(decl);
    }
    Visit(node->GetCompoundStatement());
    indent_level_--;
}

void CodeGenerator::VisitVarDecl(
    const std::shared_ptr<code_generation::VarDecl> &node) {
    ostream_ << string(indent_level_, ' ')
             << "VarDecl: " << node->GetVarNode()->GetValue() << ": "
             << node->GetTypeNode()->GetType();
}

int CodeGenerator::VisitBinOp(const std::shared_ptr<code_generation::BinOp> &node) {
    if (node->GetOper()->GetType() == PLUS) {
        return Visit(node->GetLeft());
    } else if (node->GetOper()->GetType() == MINUS) {
        return Visit(node->GetLeft()) - Visit(node->GetRight());
    } else if (node->GetOper()->GetType() == MUL) {
        return Visit(node->GetLeft()) * Visit(node->GetRight());
    } else if (node->GetOper()->GetType() == DIV) {
        return Visit(node->GetLeft()) / Visit(node->GetRight());
    }

    throw std::runtime_error("Invalid operator");
}

int CodeGenerator::VisitNum(const std::shared_ptr<code_generation::Num> &node) {
    return node->GetValue();
}
} // namespace code_generation
} // namespace pascal2c

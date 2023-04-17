#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>

#include "ast_printer.h"
#include "code_generation/ast_adapter.h"
#include "lexer/token.h"

namespace pascal2c {
namespace code_generation {
void ASTPrinter::Visit(const std::shared_ptr<semantic::ASTRoot> &node) {
    if (std::dynamic_pointer_cast<semantic::Program>(node)) {
        VisitProgram(std::dynamic_pointer_cast<semantic::Program>(node));
    } else if (std::dynamic_pointer_cast<semantic::Block>(node)) {
        VisitBlock(std::dynamic_pointer_cast<semantic::Block>(node));
    } else if (std::dynamic_pointer_cast<semantic::VarDecl>(node)) {
        VisitVarDecl(std::dynamic_pointer_cast<semantic::VarDecl>(node));
    } else if (std::dynamic_pointer_cast<semantic::Type>(node)) {
        VisitType(std::dynamic_pointer_cast<semantic::Type>(node));
    } else if (std::dynamic_pointer_cast<semantic::Compound>(node)) {
        VisitCompound(std::dynamic_pointer_cast<semantic::Compound>(node));
    } else if (std::dynamic_pointer_cast<semantic::Assign>(node)) {
        VisitAssign(std::dynamic_pointer_cast<semantic::Assign>(node));
    } else if (std::dynamic_pointer_cast<semantic::Var>(node)) {
        VisitVar(std::dynamic_pointer_cast<semantic::Var>(node));
    } else if (std::dynamic_pointer_cast<semantic::NoOp>(node)) {
        VisitNoOp(std::dynamic_pointer_cast<semantic::NoOp>(node));
    } else if (std::dynamic_pointer_cast<semantic::BinOp>(node)) {
        VisitBinOp(std::dynamic_pointer_cast<semantic::BinOp>(node));
    } else if (std::dynamic_pointer_cast<semantic::Num>(node)) {
        VisitNum(std::dynamic_pointer_cast<semantic::Num>(node));
    }

    throw std::runtime_error("Invalid node type");
}

void ASTPrinter::VisitProgram(const std::shared_ptr<semantic::Program> &node) {
    ostream_ << "Program: " << node->getName() << std::endl;
    indent_level_++;
    Visit(node->GetBlock());
    indent_level_--;
}

void ASTPrinter::VisitBlock(const std::shared_ptr<semantic::Block> &node) {
    ostream_ << string(indent_level_, ' ') << " Block" << std::endl;
    indent_level_++;
    for (auto decl : node->GetDeclarations()) {
        Visit(decl);
    }
    Visit(node->GetCompoundStatement());
    indent_level_--;
}

void ASTPrinter::VisitVarDecl(const std::shared_ptr<semantic::VarDecl> &node) {
    ostream_ << string(indent_level_, ' ')
             << "VarDecl: " << node->GetVarNode()->GetValue() << ": "
             << node->GetTypeNode()->GetType() << std::endl;
}

void ASTPrinter::VisitCompound(
    const std::shared_ptr<semantic::Compound> &node) {
    ostream_ << string(indent_level_, ' ') << "Compound" << std::endl;
    indent_level_++;
    for (const auto &child : node->GetChildren()) {
        Visit(child);
    }
    indent_level_--;
}

void ASTPrinter::VisitAssign(const std::shared_ptr<semantic::Assign> &node) {
    ostream_ << string(indent_level_, ' ') << "Assign" << std::endl;
    indent_level_++;
    ostream_ << string(indent_level_, ' ') << "Left:" << std::endl;
    Visit(node->GetLeft());
    ostream_ << string(indent_level_, ' ') << "Right:" << std::endl;
    Visit(node->GetRight());
    indent_level_--;
}

void ASTPrinter::VisitVar(const std::shared_ptr<semantic::Var> &node) {
    ostream_ << string(indent_level_, ' ') << "Var:" << node->GetValue()
             << std::endl;
}

void ASTPrinter::VisitType(const std::shared_ptr<semantic::Type> &node) {
    ostream_ << string(indent_level_, ' ') << "Type:" << node->GetType()
             << std::endl;
}

void ASTPrinter::VisitNoOp(const std::shared_ptr<semantic::NoOp> &node) {
    ostream_ << string(indent_level_, ' ') << "NoOp" << std::endl;
}

void ASTPrinter::VisitBinOp(const std::shared_ptr<semantic::BinOp> &node) {
    ostream_ << string(indent_level_, ' ')
             << "BinOp:" << node->getOper()->getType() << std::endl;
    indent_level_++;
    Visit(node->getLeft());
    Visit(node->getRight());
    indent_level_--;
}

void ASTPrinter::VisitNum(const std::shared_ptr<semantic::Num> &node) {
    ostream_ << string(indent_level_, ' ') << "Num:" << node->getValue()
             << std::endl;
}
} // namespace code_generation
} // namespace pascal2c

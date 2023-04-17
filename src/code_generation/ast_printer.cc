#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>

#include "ast_printer.h"
#include "ast_adapter.h"
#include "token_adapter.h"

namespace pascal2c {
namespace code_generation {
void ASTPrinter::Visit(const std::shared_ptr<code_generation::ASTRoot> &node) {
    if (std::dynamic_pointer_cast<code_generation::Program>(node)) {
        VisitProgram(std::dynamic_pointer_cast<code_generation::Program>(node));
    } else if (std::dynamic_pointer_cast<code_generation::Block>(node)) {
        VisitBlock(std::dynamic_pointer_cast<code_generation::Block>(node));
    } else if (std::dynamic_pointer_cast<code_generation::VarDecl>(node)) {
        VisitVarDecl(std::dynamic_pointer_cast<code_generation::VarDecl>(node));
    } else if (std::dynamic_pointer_cast<code_generation::Type>(node)) {
        VisitType(std::dynamic_pointer_cast<code_generation::Type>(node));
    } else if (std::dynamic_pointer_cast<code_generation::Compound>(node)) {
        VisitCompound(std::dynamic_pointer_cast<code_generation::Compound>(node));
    } else if (std::dynamic_pointer_cast<code_generation::Assign>(node)) {
        VisitAssign(std::dynamic_pointer_cast<code_generation::Assign>(node));
    } else if (std::dynamic_pointer_cast<code_generation::Var>(node)) {
        VisitVar(std::dynamic_pointer_cast<code_generation::Var>(node));
    } else if (std::dynamic_pointer_cast<code_generation::NoOp>(node)) {
        VisitNoOp(std::dynamic_pointer_cast<code_generation::NoOp>(node));
    } else if (std::dynamic_pointer_cast<code_generation::BinOp>(node)) {
        VisitBinOp(std::dynamic_pointer_cast<code_generation::BinOp>(node));
    } else if (std::dynamic_pointer_cast<code_generation::Num>(node)) {
        VisitNum(std::dynamic_pointer_cast<code_generation::Num>(node));
    }

    throw std::runtime_error("Invalid node type");
}

void ASTPrinter::VisitProgram(const std::shared_ptr<code_generation::Program> &node) {
    ostream_ << "Program: " << node->GetName() << std::endl;
    indent_level_++;
    Visit(node->GetBlock());
    indent_level_--;
}

void ASTPrinter::VisitBlock(const std::shared_ptr<code_generation::Block> &node) {
    ostream_ << string(indent_level_, ' ') << " Block" << std::endl;
    indent_level_++;
    for (auto decl : node->GetDeclarations()) {
        Visit(decl);
    }
    Visit(node->GetCompoundStatement());
    indent_level_--;
}

void ASTPrinter::VisitVarDecl(const std::shared_ptr<code_generation::VarDecl> &node) {
    ostream_ << string(indent_level_, ' ')
             << "VarDecl: " << node->GetVarNode()->GetValue() << ": "
             << node->GetTypeNode()->GetType() << std::endl;
}

void ASTPrinter::VisitCompound(
    const std::shared_ptr<code_generation::Compound> &node) {
    ostream_ << string(indent_level_, ' ') << "Compound" << std::endl;
    indent_level_++;
    for (const auto &child : node->GetChildren()) {
        Visit(child);
    }
    indent_level_--;
}

void ASTPrinter::VisitAssign(const std::shared_ptr<code_generation::Assign> &node) {
    ostream_ << string(indent_level_, ' ') << "Assign" << std::endl;
    indent_level_++;
    ostream_ << string(indent_level_, ' ') << "Left:" << std::endl;
    Visit(node->GetLeft());
    ostream_ << string(indent_level_, ' ') << "Right:" << std::endl;
    Visit(node->GetRight());
    indent_level_--;
}

void ASTPrinter::VisitVar(const std::shared_ptr<code_generation::Var> &node) {
    ostream_ << string(indent_level_, ' ') << "Var:" << node->GetValue()
             << std::endl;
}

void ASTPrinter::VisitType(const std::shared_ptr<code_generation::Type> &node) {
    ostream_ << string(indent_level_, ' ') << "Type:" << node->GetType()
             << std::endl;
}

void ASTPrinter::VisitNoOp(const std::shared_ptr<code_generation::NoOp> &node) {
    ostream_ << string(indent_level_, ' ') << "NoOp" << std::endl;
}

void ASTPrinter::VisitBinOp(const std::shared_ptr<code_generation::BinOp> &node) {
    ostream_ << string(indent_level_, ' ')
             << "BinOp:" << node->GetOper()->GetType() << std::endl;
    indent_level_++;
    Visit(node->GetLeft());
    Visit(node->GetRight());
    indent_level_--;
}

void ASTPrinter::VisitNum(const std::shared_ptr<code_generation::Num> &node) {
    ostream_ << string(indent_level_, ' ') << "Num:" << node->GetValue()
             << std::endl;
}
} // namespace code_generation
} // namespace pascal2c

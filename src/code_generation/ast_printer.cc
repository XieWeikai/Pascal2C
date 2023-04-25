#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>

#include "ast_adapter.h"
#include "ast_printer.h"
#include "token_adapter.h"

namespace pascal2c {
namespace code_generation {
using ::std::dynamic_pointer_cast;
using ::std::endl;
using ::std::runtime_error;
using ::std::shared_ptr;

void ASTPrinter::Visit() {
    auto program = dynamic_pointer_cast<Program>(ast_);
    Visit(program);
}
void ASTPrinter::Visit(const shared_ptr<ASTNode> &node) {
    if (dynamic_pointer_cast<Program>(node)) {
        VisitProgram(dynamic_pointer_cast<Program>(node));
    } else if (dynamic_pointer_cast<Block>(node)) {
        VisitBlock(dynamic_pointer_cast<Block>(node));
    } else if (dynamic_pointer_cast<VarDecl>(node)) {
        VisitVarDecl(dynamic_pointer_cast<VarDecl>(node));
    } else if (dynamic_pointer_cast<Type>(node)) {
        VisitType(dynamic_pointer_cast<Type>(node));
    } else if (dynamic_pointer_cast<Compound>(node)) {
        VisitCompound(dynamic_pointer_cast<Compound>(node));
    } else if (dynamic_pointer_cast<Assign>(node)) {
        VisitAssign(dynamic_pointer_cast<Assign>(node));
    } else if (dynamic_pointer_cast<Var>(node)) {
        VisitVar(dynamic_pointer_cast<Var>(node));
    } else if (dynamic_pointer_cast<NoOp>(node)) {
        VisitNoOp(dynamic_pointer_cast<NoOp>(node));
    } else if (dynamic_pointer_cast<BinOp>(node)) {
        VisitBinOp(dynamic_pointer_cast<BinOp>(node));
    } else if (dynamic_pointer_cast<Num>(node)) {
        VisitNum(dynamic_pointer_cast<Num>(node));
    } else {
        throw runtime_error("Invalid node type");
    }
}

void ASTPrinter::VisitProgram(const shared_ptr<Program> &node) {
    ostream_ << "Program: " << node->GetName() << endl;
    indent_level_++;
    Visit(node->GetBlock());
    indent_level_--;
}

void ASTPrinter::VisitBlock(const shared_ptr<Block> &node) {
    ostream_ << string(indent_level_, ' ') << "Block" << endl;
    indent_level_++;
    for (auto decl : node->GetDeclarations()) {
        Visit(decl);
    }
    Visit(node->GetCompoundStatement());
    indent_level_--;
}

void ASTPrinter::VisitDeclaration(const shared_ptr<Declaration> &node) {
    ostream_ << string(indent_level_, ' ') << "Declaration: " << endl;
    indent_level_++;
    for (const auto &it : node->GetDeclarations()) {
        auto var_decl = dynamic_pointer_cast<VarDecl>(it);
        if (var_decl == nullptr) {
            throw runtime_error(
                "Failed to cast Declaration ASTNode into VarDecl");
        }
        VisitVarDecl(var_decl);
    }
    indent_level_--;
}

void ASTPrinter::VisitVarDecl(const shared_ptr<VarDecl> &node) {
    ostream_ << string(indent_level_, ' ')
             << "VarDecl: " << node->GetVarNode()->GetValue() << ": "
             << node->GetTypeNode()->GetType() << endl;
}

void ASTPrinter::VisitCompound(const shared_ptr<Compound> &node) {
    ostream_ << string(indent_level_, ' ') << "Compound" << endl;
    indent_level_++;
    for (const auto &child : node->GetChildren()) {
        Visit(child);
    }
    indent_level_--;
}

void ASTPrinter::VisitAssign(const shared_ptr<Assign> &node) {
    ostream_ << string(indent_level_, ' ') << "Assign" << endl;
    indent_level_++;

    ostream_ << string(indent_level_, ' ') << "Left:" << endl;
    indent_level_++;
    Visit(node->GetLeft());
    indent_level_--;

    ostream_ << string(indent_level_, ' ') << "Right:" << endl;
    indent_level_++;
    Visit(node->GetRight());
    indent_level_--;

    indent_level_--;
}

void ASTPrinter::VisitVar(const shared_ptr<Var> &node) {
    ostream_ << string(indent_level_, ' ') << "Var: " << node->GetValue()
             << endl;
}

void ASTPrinter::VisitType(const shared_ptr<Type> &node) {
    ostream_ << string(indent_level_, ' ') << "Type: " << node->GetType()
             << endl;
}

void ASTPrinter::VisitNoOp(const shared_ptr<NoOp> &node) {
    ostream_ << string(indent_level_, ' ') << "NoOp" << endl;
}

void ASTPrinter::VisitBinOp(const shared_ptr<BinOp> &node) {
    ostream_ << string(indent_level_, ' ')
             << "BinOp: " << node->GetOper()->GetType().ToString() << endl;
    indent_level_++;
    Visit(node->GetLeft());
    Visit(node->GetRight());
    indent_level_--;
}

void ASTPrinter::VisitNum(const shared_ptr<Num> &node) {
    ostream_ << string(indent_level_, ' ') << "Num: " << node->GetValue()
             << endl;
}
string ASTPrinter::ToString() const { return ostream_.str(); }
} // namespace code_generation
} // namespace pascal2c

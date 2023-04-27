#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <sys/errno.h>

#include "ast_adapter.h"
#include "ast_printer.h"
#include "token_adapter.h"

namespace pascal2c {
namespace code_generation {
using ::std::dynamic_pointer_cast;
using ::std::endl;
using ::std::runtime_error;
using ::std::shared_ptr;

void ASTPrinter::Traverse(const shared_ptr<ASTNode> &node) { Visit(node); }

void ASTPrinter::Visit(const shared_ptr<ASTNode> &node, bool indent) {
    node->Accept(*this);
}

void ASTPrinter::VisitArgument(const shared_ptr<Argument> &node) {
    ErrNotImplemented();
}

void ASTPrinter::VisitProgram(const shared_ptr<Program> &node) {
    ostream_ << "Program: " << node->GetName() << endl;
    indent_level_++;
    Visit(node->GetBlock());
    indent_level_--;
}

void ASTPrinter::VisitSubprogram(const shared_ptr<Subprogram> &node) {
    ErrNotImplemented();
}

void ASTPrinter::VisitFunction(const shared_ptr<Function> &node) {
    ErrNotImplemented();
}

void ASTPrinter::VisitBlock(const shared_ptr<Block> &node) {
    ostream_ << string(indent_level_, ' ') << "Block" << endl;
    indent_level_++;
    Visit(node->GetDeclatation());
    Visit(node->GetCompoundStatement());
    indent_level_--;
}

void ASTPrinter::VisitDeclaration(const shared_ptr<Declaration> &node) {
    ostream_ << string(indent_level_, ' ') << "Declaration: " << endl;
    indent_level_++;
    for (const auto &it : node->GetDeclarations()) {
        auto var_decl = dynamic_pointer_cast<VarDeclaration>(it);
        if (var_decl == nullptr) {
            throw runtime_error(
                "Failed to cast Declaration ASTNode into VarDecl");
        }
        VisitVarDecl(var_decl);
    }
    indent_level_--;
}

void ASTPrinter::VisitConstDeclaration(
    const shared_ptr<ConstDeclaration> &node) {
    ErrNotImplemented();
}
void ASTPrinter::VisitArrayType(const shared_ptr<ArrayType> &node) {
    ErrNotImplemented();
}
void ASTPrinter::VisitArray(const shared_ptr<Array> &node) {
    ErrNotImplemented();
}
void ASTPrinter::VisitArrayDeclaration(
    const shared_ptr<ArrayDeclaration> &node) {
    ErrNotImplemented();
}

void ASTPrinter::VisitVarDecl(const shared_ptr<VarDeclaration> &node) {
    ostream_ << string(indent_level_, ' ')
             << "VarDecl: " << node->GetVarNode()->GetName() << ": "
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
    ostream_ << string(indent_level_, ' ') << "Var: " << node->GetName()
             << endl;
}

void ASTPrinter::VisitType(const shared_ptr<Type> &node) {
    ostream_ << string(indent_level_, ' ') << "Type: " << node->GetType()
             << endl;
}

void ASTPrinter::VisitConstType(const shared_ptr<ConstType> &node) {
    ErrNotImplemented();
}

void ASTPrinter::VisitNoOp(const shared_ptr<NoOp> &node) {
    ostream_ << string(indent_level_, ' ') << "NoOp" << endl;
}

void ASTPrinter::VisitBinOp(const shared_ptr<BinOp> &node) {
    ostream_ << string(indent_level_, ' ') << "BinOp:" << endl;
    indent_level_++;
    Visit(node->GetLeft());
    Visit(node->GetOper());
    Visit(node->GetRight());
    indent_level_--;
}

void ASTPrinter::VisitOper(const shared_ptr<Oper> &node) {
    ostream_ << string(indent_level_, ' ') << "Oper: " << node->GetOper()
             << endl;
}

void ASTPrinter::VisitNum(const shared_ptr<Num> &node) {
    ostream_ << string(indent_level_, ' ') << "Num: " << node->GetValue()
             << endl;
}

void ASTPrinter::VisitStatement(const shared_ptr<Statement> &node) {
    ErrNotImplemented();
}
void ASTPrinter::VisitIfStatement(const shared_ptr<IfStatement> &node) {
    ErrNotImplemented();
}
void ASTPrinter::VisitForStatement(const shared_ptr<ForStatement> &node) {
    ErrNotImplemented();
}

string ASTPrinter::ToString() const { return ostream_.str(); }
} // namespace code_generation
} // namespace pascal2c

#include <memory>
#include <stdexcept>
#include <string>

#include "code_generation/ast_adapter.h"
#include "code_generator.h"
#include "token_adapter.h"

namespace pascal2c {
namespace code_generation {
using ::std::dynamic_pointer_cast;
using ::std::endl;
using ::std::runtime_error;
using ::std::shared_ptr;

void CodeGenerator::Interpret() {
    auto program = std::dynamic_pointer_cast<Program>(ast_);
    Visit(ast_);
}

const string CodeGenerator::GetCCode() const { return ostream_.str(); }

void CodeGenerator::Visit(
    const std::shared_ptr<code_generation::ASTNode> &node) {
    node->Accept(*this);
}

void CodeGenerator::VisitProgram(
    const std::shared_ptr<code_generation::Program> &node) {
    ostream_ << "Program: " << node->GetName() << std::endl;
    indent_level_++;
    Visit(node->GetBlock());
    indent_level_--;
}

void CodeGenerator::VisitBlock(
    const std::shared_ptr<code_generation::Block> &node) {
    ostream_ << string(indent_level_, ' ') << "Block" << std::endl;
    indent_level_++;
    Visit(node->GetDeclatation());
    Visit(node->GetCompoundStatement());
    indent_level_--;
}

void CodeGenerator::VisitDeclaration(const shared_ptr<Declaration> &node) {
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

    void CodeGenerator::VisitVarDecl(
        const std::shared_ptr<code_generation::VarDecl> &node) {
    ostream_ << string(indent_level_, ' ')
             << "VarDecl: " << node->GetVarNode()->GetValue() << ": "
             << node->GetTypeNode()->GetType() << std::endl;
}

void CodeGenerator::VisitCompound(
    const std::shared_ptr<code_generation::Compound> &node) {
    ostream_ << string(indent_level_, ' ') << "Compound" << std::endl;
    indent_level_++;
    for (const auto &child : node->GetChildren()) {
        Visit(child);
    }
    indent_level_--;
}

void CodeGenerator::VisitAssign(
    const std::shared_ptr<code_generation::Assign> &node) {
    ostream_ << string(indent_level_, ' ') << "Assign" << std::endl;
    indent_level_++;

    ostream_ << string(indent_level_, ' ') << "Left:" << std::endl;
    indent_level_++;
    Visit(node->GetLeft());
    indent_level_--;

    ostream_ << string(indent_level_, ' ') << "Right:" << std::endl;
    indent_level_++;
    Visit(node->GetRight());
    indent_level_--;

    indent_level_--;
}

void CodeGenerator::VisitVar(
    const std::shared_ptr<code_generation::Var> &node) {
    ostream_ << string(indent_level_, ' ') << "Var: " << node->GetValue()
             << std::endl;
}

void CodeGenerator::VisitType(
    const std::shared_ptr<code_generation::Type> &node) {
    ostream_ << string(indent_level_, ' ') << "Type: " << node->GetType()
             << std::endl;
}

void CodeGenerator::VisitNoOp(
    const std::shared_ptr<code_generation::NoOp> &node) {
    ostream_ << string(indent_level_, ' ') << "NoOp" << std::endl;
}

void CodeGenerator::VisitBinOp(
    const std::shared_ptr<code_generation::BinOp> &node) {
    indent_level_++;
    Visit(node->GetLeft());
    Visit(node->GetOper());
    Visit(node->GetRight());
    indent_level_--;
}

void CodeGenerator::VisitOper(const shared_ptr<Oper> &node) {
    ostream_ << node->GetType().ToString();
}

void CodeGenerator::VisitNum(
    const std::shared_ptr<code_generation::Num> &node) {
    ostream_ << string(indent_level_, ' ') << "Num: " << node->GetValue()
             << std::endl;
}

} // namespace code_generation
} // namespace pascal2c

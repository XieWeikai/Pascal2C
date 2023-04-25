#include <memory>
#include <stdexcept>
#include <string>

#include "code_generation/ast_adapter.h"
#include "code_generation/visitor.h"
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

void CodeGenerator::Visit(const std::shared_ptr<code_generation::ASTNode> &node,
                          bool indent) {
    if (indent)
        ostream_ << Indent();
    node->Accept(*this);
}

void CodeGenerator::VisitProgram(
    const std::shared_ptr<code_generation::Program> &node) {
    ostream_ << "#include <stdio.h>" << endl
             << "#include <stdlib.h>" << endl
             << endl;
    ostream_ << "// " << node->GetName() << endl;
    ostream_ << "int main(int argc, char* argv[]) {" << endl;

    IncIndent();
    Visit(node->GetBlock());
    ostream_ << Indent() << "return 0;" << endl;
    DecIndent();
    ostream_ << "}" << endl << "// " << node->GetName() << endl;
}

void CodeGenerator::VisitBlock(
    const std::shared_ptr<code_generation::Block> &node) {
    Visit(node->GetDeclatation());
    Visit(node->GetCompoundStatement());
}

void CodeGenerator::VisitDeclaration(const shared_ptr<Declaration> &node) {
    for (const auto &it : node->GetDeclarations()) {
        auto var_decl = dynamic_pointer_cast<VarDecl>(it);
        if (var_decl == nullptr) {
            throw runtime_error(
                "Failed to cast Declaration ASTNode into VarDecl");
        }
        Visit(var_decl, true);
    }
}

void CodeGenerator::VisitVarDecl(
    const std::shared_ptr<code_generation::VarDecl> &node) {
    Visit(node->GetTypeNode());
    ostream_ << " ";
    Visit(node->GetVarNode());
    ostream_ << eol_;
}

void CodeGenerator::VisitCompound(
    const std::shared_ptr<code_generation::Compound> &node) {
    for (const auto &child : node->GetChildren()) {
        Visit(child);
    }
}

void CodeGenerator::VisitAssign(
    const std::shared_ptr<code_generation::Assign> &node) {
    Visit(node->GetLeft(), true);
    ostream_ << " = ";
    Visit(node->GetRight());
    ostream_ << eol_;
}

void CodeGenerator::VisitVar(
    const std::shared_ptr<code_generation::Var> &node) {
    ostream_ << node->GetValue();
}

void CodeGenerator::VisitType(
    const std::shared_ptr<code_generation::Type> &node) {
    ostream_ << TypeToC(node->GetType());
}

void CodeGenerator::VisitNoOp(
    const std::shared_ptr<code_generation::NoOp> &node) {
    ostream_ << ";" << endl;
}

void CodeGenerator::VisitBinOp(
    const std::shared_ptr<code_generation::BinOp> &node) {
    IncIndent();
    ostream_ << '(';
    Visit(node->GetLeft());
    ostream_ << ' ';
    Visit(node->GetOper());
    ostream_ << ' ';
    Visit(node->GetRight());
    ostream_ << ')';
    DecIndent();
}

void CodeGenerator::VisitOper(const shared_ptr<Oper> &node) {
    ostream_ << TypeToC(node->GetType().ToString());
}

void CodeGenerator::VisitNum(
    const std::shared_ptr<code_generation::Num> &node) {
    ostream_ << node->GetValue();
}

const string CodeGenerator::Indent() const {
    return string(indent_level_ * 4, ' ');
}

void CodeGenerator::IncIndent() { indent_level_++; }

void CodeGenerator::DecIndent() { indent_level_--; }

const string CodeGenerator::TypeToC(const string &pascal_type) const {
    return type_tool_kit_.TypeToC(pascal_type);
}
} // namespace code_generation
} // namespace pascal2c

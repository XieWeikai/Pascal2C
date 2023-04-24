#include <memory>
#include <stdexcept>
#include <string>

#include "code_generation/ast_adapter.h"
#include "code_generator.h"
#include "token_adapter.h"

namespace pascal2c {
namespace code_generation {

void CodeGenerator::Interpret() { Visit(ast_); }

const string CodeGenerator::GetCCode() const { return ostream_.str(); }

// TODO: Re-implement this method. Now it just visits BinOp and Num
// And lead its output into ostream_
void CodeGenerator::Visit(
    const std::shared_ptr<code_generation::ASTRoot> &node) {
    if (std::dynamic_pointer_cast<code_generation::BinOp>(node)) {
        VisitBinOp(std::dynamic_pointer_cast<code_generation::BinOp>(node));
    } else if (std::dynamic_pointer_cast<code_generation::Num>(node)) {
        VisitNum(std::dynamic_pointer_cast<code_generation::Num>(node));
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

void CodeGenerator::VisitBinOp(
    const std::shared_ptr<code_generation::BinOp> &node) {
    Visit(node->GetLeft());
    ostream_ << node->GetOper()->GetType().ToString();
    Visit(node->GetRight());
}

void CodeGenerator::VisitNum(
    const std::shared_ptr<code_generation::Num> &node) {
    node->GetValue();
}
} // namespace code_generation
} // namespace pascal2c

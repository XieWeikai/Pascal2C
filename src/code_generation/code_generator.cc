#include <memory>
#include <ostream>
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

void CodeGenerator::Visit(const shared_ptr<code_generation::ASTNode> &node,
                          bool indent) {
    if (indent)
        ostream_ << Indent();
    node->Accept(*this);
}

void CodeGenerator::VisitArgument(const shared_ptr<Argument> &node) {
    if (node->IsReference()) {
        ostream_ << "/* Is Reference */";
    }
    Visit(node->GetType());
    if (node->IsReference()) {
        ostream_ << "*";
    }
    ostream_ << " ";
    Visit(node->GetVariable());
}

void CodeGenerator::VisitProgram(
    const shared_ptr<code_generation::Program> &node) {
    ostream_ << "#include <stdio.h>" << endl
             << "#include <stdlib.h>" << endl
             << endl;
    ostream_ << "// " << node->GetName() << endl;
    ostream_ << "int main(int argc, char* argv[]) {" << endl;

    IncIndent();
    Visit(node->GetBlock());
    ostream_ << Indent() << "return 0;" << endl;
    DecIndent();
    ostream_ << "}\n";
}

void CodeGenerator::VisitSubprogram(const shared_ptr<Subprogram> &node) {
    ostream_ << Indent() << "void " << node->GetName() << "(";
    for (auto i = 0; i < node->GetArgs().size(); i++) {
        const auto &arg = node->GetArgs().at(i);
        Visit(arg);
        if (i < node->GetArgs().size() - 1) {
            ostream_ << ",";
        }
    }
    ostream_ << ") {\n";
    IncIndent();
    Visit(node->GetBlock());
    DecIndent();
    ostream_ << Indent() << "}\n";
}

void CodeGenerator::VisitFunction(const shared_ptr<Function> &node) {
    ostream_ << node->GetReturnType() << ' ' << node->GetName() << '(';
    for (int i = 0; i < node->GetArgs().size(); i++) {
        const auto &arg = node->GetArgs().at(i);
        Visit(arg);
        if (i < node->GetArgs().size() - 1) {
            ostream_ << ',';
        }
    }
    ostream_ << ") {\n";
    IncIndent();
    Visit(node->GetBlock());
    DecIndent();
    ostream_ << Indent() << "}\n";
}

void CodeGenerator::VisitBlock(const shared_ptr<code_generation::Block> &node) {
    Visit(node->GetDeclatation());
    Visit(node->GetCompoundStatement());
}

void CodeGenerator::VisitDeclaration(const shared_ptr<Declaration> &node) {
    for (const auto &it : node->GetDeclarations()) {
        auto var_decl = dynamic_pointer_cast<VarDeclaration>(it);
        if (var_decl == nullptr) {
            throw runtime_error(
                "Failed to cast Declaration ASTNode into VarDecl");
        }
        Visit(var_decl, true);
    }
}

void CodeGenerator::VisitConstDeclaration(
    const shared_ptr<ConstDeclaration> &node) {
    Visit(node->GetTypeNode());
    ostream_ << ' ';
    Visit(node->GetConstNode());
    ostream_ << eol_;
}

void CodeGenerator::VisitVarDecl(
    const shared_ptr<code_generation::VarDeclaration> &node) {
    Visit(node->GetTypeNode());
    ostream_ << ' ';
    Visit(node->GetVarNode());
    ostream_ << eol_;
}

void CodeGenerator::VisitArrayType(const shared_ptr<ArrayType> &node) {
    Visit(node->GetType());
}

void CodeGenerator::VisitArray(const shared_ptr<Array> &node) {
    Visit(node->GetVarNode());
}

void CodeGenerator::VisitArrayDeclaration(
    const shared_ptr<ArrayDeclaration> &node) {
    Visit(node->GetTypeNode());
    ostream_ << ' ';
    Visit(node->GetArrayNode());
    auto bounds = node->GetTypeNode()->GetBounds();
    for (auto &b : bounds) {
        ostream_ << '[' << b.second - b.first + 1 << ']';
    }
    ostream_ << eol_;
}

void CodeGenerator::VisitCompound(
    const shared_ptr<code_generation::Compound> &node) {
    for (const auto &child : node->GetChildren()) {
        Visit(child);
    }
}

void CodeGenerator::VisitAssign(
    const shared_ptr<code_generation::Assign> &node) {
    Visit(node->GetLeft(), true);
    ostream_ << " = ";
    Visit(node->GetRight());
    ostream_ << eol_;
}

void CodeGenerator::VisitVar(const shared_ptr<code_generation::Var> &node) {
    ostream_ << node->GetValue();
}

void CodeGenerator::VisitType(const shared_ptr<code_generation::Type> &node) {
    ostream_ << TypeToC(node->GetType());
}

void CodeGenerator::VisitNoOp(const shared_ptr<code_generation::NoOp> &node) {
    ostream_ << ";" << endl;
}

void CodeGenerator::VisitBinOp(const shared_ptr<code_generation::BinOp> &node) {
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

void CodeGenerator::VisitNum(const shared_ptr<code_generation::Num> &node) {
    ostream_ << node->GetValue();
}

void CodeGenerator::VisitStatement(const shared_ptr<Statement> &node) {
    Visit(node->GetLeftHand(), true);
    ostream_ << " = ";
    Visit(node->GetRightHand());
}

void CodeGenerator::VisitIfStatement(const shared_ptr<IfStatement> &node) {
    ostream_ << Indent() << "if (";
    Visit(node->GetCondition());
    ostream_ << ") {\n";
    IncIndent();
    Visit(node->GetThenBranch());
    DecIndent();
    ostream_ << Indent() << "}";
    if (node->GetElseBranch()) {
        IncIndent();
        ostream_ << " else {\n";
        Visit(node->GetElseBranch());
        DecIndent();
        ostream_ << Indent() << "}";
    }
    ostream_ << "\n";
}

void CodeGenerator::VisitForStatement(const shared_ptr<ForStatement> &node) {
    ostream_ << Indent() << "for (";
    Visit(node->GetVariable());
    ostream_ << " = ";
    Visit(node->GetStart());
    ostream_ << "; ";
    Visit(node->GetVariable());
    ostream_ << " <= ";
    Visit(node->GetEnd());
    ostream_ << "; ";
    Visit(node->GetVariable());
    ostream_ << "++) {\n";
    IncIndent();
    Visit(node->GetBody());
    DecIndent();
    ostream_ << Indent() << "}\n";
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

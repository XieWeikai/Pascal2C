#include <memory>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "code_generation/ast_adapter.h"
#include "code_generation/type_adaper.h"
#include "code_generation/visitor.h"
#include "code_generator.h"
#include "token_adapter.h"

namespace pascal2c {
namespace code_generation {
using ::std::dynamic_pointer_cast;
using ::std::endl;
using ::std::shared_ptr;

void CodeGenerator::Interpret(const shared_ptr<ASTNode> &node) { Visit(node); }

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

    // Generate array bounds code if type is ArrayType
    auto array_type = dynamic_pointer_cast<ArrayType>(node->GetType());
    if (array_type) {
        // Array bounds
        auto bounds = array_type->GetBounds();
        auto dims = bounds.size();
        for (auto i = 0; i < dims; i++) {
            auto b = bounds.at(i);
            ostream_ << '[' << (b.second - b.first + 1) << ']';
        }
    }

    ostream_ << (node->IsReference() ? " *" : " ");
    // When visiting variable, will lookup variable in Symbol table, and add '*'
    // if variable is passed-by-reference or is a pointer
    Visit(node->GetVariable());
}

void CodeGenerator::VisitProgram(
    const shared_ptr<code_generation::Program> &node) {
    ostream_ << "#include <stdio.h>" << endl
             << "#include <stdlib.h>" << endl
             << endl;

    auto program_block = node->GetBlock();
    Visit(program_block->GetDeclaration());

    ostream_ << "// " << node->GetName() << endl;
    ostream_ << "int main(int argc, char* argv[]) {" << endl;

    IncIndent();
    Visit(program_block->GetCompoundStatement());
    ostream_ << Indent() << "return 0;" << endl;
    DecIndent();
    ostream_ << "}\n";
}

void CodeGenerator::VisitSubprogram(const shared_ptr<Subprogram> &node) {
    // Store parent scope name, return to this scope after visiting
    // subprogram.
    // string parent_scope_name = GetCurrentScope();
    // SetCurrentScope(node->GetName());

    ostream_ << Indent() << "void " << node->GetName() << "(";
    for (auto i = 0; i < node->GetArgs().size(); i++) {
        const auto &arg = node->GetArgs().at(i);
        Visit(arg);
        if (i < node->GetArgs().size() - 1) {
            ostream_ << ", ";
        }
    }
    ostream_ << ") {\n";
    IncIndent();
    Visit(node->GetBlock());
    DecIndent();
    ostream_ << Indent() << "}\n";
    // Return to parent symbol scope
    // SetCurrentScope(parent_scope_name);
}

void CodeGenerator::VisitFunction(const shared_ptr<Function> &node) {
    // Get parent symbol scope name for returning after visiting function
    // string parent_scope_name = GetCurrentScope();
    // SetCurrentScope(node->GetName());

    ostream_ << node->GetReturnType() << ' ' << node->GetName() << '(';
    for (int i = 0; i < node->GetArgs().size(); i++) {
        const auto &arg = node->GetArgs().at(i);
        Visit(arg);
        if (i < node->GetArgs().size() - 1) {
            ostream_ << ", ";
        }
    }
    ostream_ << ") {\n";
    IncIndent();
    // Declare return variable
    ostream_ << Indent() << node->GetReturnType() << ' ';
    Visit(node->GetReturnVar());
    ostream_ << ";/* Auto Generated */\n";
    Visit(node->GetBlock());
    // Return statement
    ostream_ << Indent() << "return ";
    Visit(node->GetReturnVar());
    ostream_ << ";/* Auto Generated */\n";
    DecIndent();
    ostream_ << Indent() << "}\n";
    // Return to parent scope
    // SetCurrentScope(parent_scope_name);
}

void CodeGenerator::VisitBlock(const shared_ptr<code_generation::Block> &node) {
    Visit(node->GetDeclaration());
    Visit(node->GetCompoundStatement());
}

void CodeGenerator::VisitDeclaration(const shared_ptr<Declaration> &node) {
    for (const auto &it : node->GetDeclarations()) {
        // auto var_decl = dynamic_pointer_cast<VarDeclaration>(it);
        // if (var_decl == nullptr) {
        //     throw runtime_error(
        //         "Failed to cast Declaration ASTNode into VarDecl");
        // }
        Visit(it, true);
    }
}

void CodeGenerator::VisitConstDeclaration(
    const shared_ptr<ConstDeclaration> &node) {
    Visit(node->GetTypeNode());
    ostream_ << ' ';
    Visit(node->GetLeftNode());
    if (node->GetRightNode()) {
        ostream_ << " = ";
        Visit(node->GetRightNode());
    }
    ostream_ << eol_;
}

void CodeGenerator::VisitVarDecl(
    const shared_ptr<code_generation::VarDeclaration> &node) {
    Visit(node->GetTypeNode());
    ostream_ << ' ';
    Visit(node->GetLeftNode());
    if (node->GetRightNode()) {
        ostream_ << " = ";
        Visit(node->GetRightNode());
    }
    ostream_ << eol_;
}

void CodeGenerator::VisitArrayType(const shared_ptr<ArrayType> &node) {
    ostream_ << SymbolToC(node->GetType());
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

void CodeGenerator::VisitArrayAccess(const shared_ptr<ArrayAccess> &node) {
    Visit(node->GetArray());
    auto bounds = node->GetBounds();
    auto indices = node->GetIndices();
    for (int i = 0; i < node->GetIndices().size(); i++) {
        auto b = bounds.at(i);
        auto index = indices.at(i);
        ostream_ << '[';
        Visit(index);
        ostream_ << " - ";
        ostream_ << b.first;
        ostream_ << ']';
    }
}

void CodeGenerator::VisitCompound(
    const shared_ptr<code_generation::Compound> &node) {
    for (const auto &child : node->GetChildren()) {
        Visit(child);
    }
}

void CodeGenerator::VisitAssign(
    const shared_ptr<code_generation::Assignment> &node) {
    Visit(node->GetLeft(), true);
    ostream_ << " = ";
    Visit(node->GetRight());
    ostream_ << eol_;
}

void CodeGenerator::VisitVar(const shared_ptr<code_generation::Var> &node) {
    if (IsReferenceArg(node)) {
        ostream_ << "*";
    }
    if (IsReturnVar(node)) {
        ostream_ << "ret_";
    }
    ostream_ << node->GetName();
}

void CodeGenerator::VisitType(const shared_ptr<code_generation::Type> &node) {
    ostream_ << SymbolToC(node->GetType());
}

void CodeGenerator::VisitConstType(const shared_ptr<ConstType> &node) {
    ostream_ << "const " << SymbolToC(node->GetType());
}

void CodeGenerator::VisitNoOp(const shared_ptr<code_generation::NoOp> &node) {
    ostream_ << ";" << endl;
}

void CodeGenerator::VisitUnaryOperation(
    const shared_ptr<UnaryOperation> &node) {
    Visit(node->GetOper());
    Visit(node->GetVarNode());
}

void CodeGenerator::VisitBinOp(
    const shared_ptr<code_generation::BinaryOperation> &node) {
    ostream_ << '(';
    Visit(node->GetLeft());
    ostream_ << ' ';
    Visit(node->GetOper());
    ostream_ << ' ';
    Visit(node->GetRight());
    ostream_ << ')';
}

void CodeGenerator::VisitOper(const shared_ptr<Oper> &node) {
    ostream_ << SymbolToC(node->GetOper());
}

void CodeGenerator::VisitNum(const shared_ptr<Num> &node) {
    ostream_ << node->GetValue();
}

void CodeGenerator::VisitString(const shared_ptr<String> &node) {
    ostream_ << "\"" << node->GetValue() << "\"";
}

void CodeGenerator::VisitReal(const shared_ptr<Real> &node) {
    ostream_ << node->GetValue();
}

void CodeGenerator::VisitChar(const shared_ptr<Char> &node) {
    ostream_ << node->GetValue();
}

void CodeGenerator::VisitStatement(const shared_ptr<Statement> &node) {
    Visit(node->GetNode(), true);
    ostream_ << ";\n";
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

// Print "%d%c%s%d..."
void CodeGenerator::PrintfFormatString(const shared_ptr<FunctionCall> &node,
                                       bool new_line) {
    vector<string> specifiers;
    auto BaseCast = [&](const shared_ptr<ASTNode> &p) -> void {
        if (auto dp = dynamic_pointer_cast<Num>(p)) {
            specifiers.push_back("%d");
        } else if (auto dp = dynamic_pointer_cast<Real>(p)) {
            specifiers.push_back("%f");
        } else if (auto dp = dynamic_pointer_cast<String>(p)) {
            specifiers.push_back("%s");
        } else if (auto dp = dynamic_pointer_cast<Char>(p)) {
            specifiers.push_back("%c");
        } else {
            specifiers.push_back("%s");
        }
    };

    auto CastByVarType = [&](const VarType vt) -> void {
        if (vt == VarType::INT)
            specifiers.push_back("%d");
        else if (vt == VarType::REAL)
            specifiers.push_back("%f");
        else if (vt == VarType::STRING)
            specifiers.push_back("%s");
        else if (vt == VarType::CHAR)
            specifiers.push_back("%c");
        else
            specifiers.push_back("%s");
    };

    auto IVarCast = [&](const shared_ptr<IVar> &p) -> void {
        CastByVarType(p->GetVarType());
    };

    auto FuncCallCast = [&](const shared_ptr<FunctionCall> &p) -> void {
        CastByVarType(p->GetReturnType());
    };

    auto params = node->GetParameters();
    for (auto &p : params) {
        if (auto dp = dynamic_pointer_cast<IVar>(p))
            IVarCast(dp);
        else if (auto dp = dynamic_pointer_cast<FunctionCall>(p))
            FuncCallCast(dp);
        else
            BaseCast(p);
    }
    ostream_ << '"';
    for (auto &s : specifiers) {
        ostream_ << s;
    }
    ostream_ << (new_line ? "\\n\", " : "\", ");
}

void CodeGenerator::VisitFunctionCall(const shared_ptr<FunctionCall> &node) {
    // Rename function when meet writeln or write
    auto func_name = node->GetName();
    if (func_name == "writeln" || func_name == "write")
        func_name = "printf";
    else if (func_name == "read")
        func_name = "scanf";

    ostream_ << func_name << "(";
    // print Format string
    if (func_name != node->GetName()) {
        PrintfFormatString(node, (node->GetName() == "writeln"));
    }

    // Print parameters
    for (int i = 0; i < node->GetParameters().size(); i++) {
        // Is reference or not
        auto r = node->GetIsReference(i);
        if (r)
            ostream_ << "&";

        // Visit parameter
        auto p = node->GetParameters().at(i);
        Visit(p);
        if (i < node->GetParameters().size() - 1) {
            ostream_ << ", ";
        }
    }

    ostream_ << ")";
}

bool CodeGenerator::IsReferenceArg(const shared_ptr<Var> &node) const {
    return node->IsReference();
}

bool CodeGenerator::IsReturnVar(const shared_ptr<Var> &node) const {
    return node->IsReturnVar();
}

const string CodeGenerator::Indent() const {
    return string(indent_level_ * 4, ' ');
}

void CodeGenerator::IncIndent() { indent_level_++; }

void CodeGenerator::DecIndent() { indent_level_--; }

const string CodeGenerator::SymbolToC(const string &pascal_type) const {
    return type_tool_kit_.SymbolToC(pascal_type);
}
} // namespace code_generation
} // namespace pascal2c
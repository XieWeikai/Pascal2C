#include "code_generation/ast_converter.h"
#include "ast/ast.h"
#include "ast/expr.h"
#include "ast/program.h"
#include "ast/statement.h"
#include "code_generation/ast_adapter.h"
#include "code_generation/visitor.h"
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace pascal2c {
namespace code_generation {
using ::std::dynamic_pointer_cast;

void ASTConverter::Convert(const shared_ptr<ast::Ast> ast) {
    // Program AST
    if (auto casted_ast = dynamic_pointer_cast<ast::Program>(ast)) {
        ConvertProgram(casted_ast);
    } else if (auto casted_ast = dynamic_pointer_cast<ast::IdList>(ast)) {
        ConvertIdList(casted_ast);
    } else if (auto casted_ast = dynamic_pointer_cast<ast::Type>(ast)) {
        ConvertType(casted_ast);
    } else if (auto casted_ast = dynamic_pointer_cast<ast::Parameter>(ast)) {
        ConvertParameter(casted_ast);
    } else if (auto casted_ast =
                   dynamic_pointer_cast<ast::ConstDeclaration>(ast)) {
        ConvertConstDeclaration(casted_ast);
    } else if (auto casted_ast =
                   dynamic_pointer_cast<ast::VarDeclaration>(ast)) {
        ConvertVarDeclaration(casted_ast);
    } else if (auto casted_ast = dynamic_pointer_cast<ast::Subprogram>(ast)) {
        ConvertSubprogram(casted_ast);
    } else if (auto casted_ast = dynamic_pointer_cast<ast::AssignStatement>(
                   dynamic_pointer_cast<Assignment>(ast))) {
        ConvertAssignmentStatement(casted_ast);
    } else if (auto casted_ast =
                   dynamic_pointer_cast<ast::CompoundStatement>(ast)) {
        ConvertCompoundStatement(casted_ast);
    } else if (auto casted_ast = dynamic_pointer_cast<ast::IfStatement>(ast)) {
        ConvertIfStatement(casted_ast);
    } else if (auto casted_ast = dynamic_pointer_cast<ast::ForStatement>(ast)) {
        ConvertForStatement(casted_ast);
    } else if (auto casted_ast = dynamic_pointer_cast<ast::StringValue>(ast)) {
        ConvertStringValue(casted_ast);
    } else if (auto casted_ast = dynamic_pointer_cast<ast::IntegerValue>(ast)) {
        ConvertIntegerValue(casted_ast);
    } else if (auto casted_ast = dynamic_pointer_cast<ast::RealValue>(ast)) {
        ConvertRealValue(casted_ast);
    } else if (auto casted_ast = dynamic_pointer_cast<ast::CharValue>(ast)) {
        ConvertCharValue(casted_ast);
    } else if (auto casted_ast = dynamic_pointer_cast<ast::BooleanValue>(ast)) {
        convertBooleanValue(casted_ast);
    } else if (auto casted_ast = dynamic_pointer_cast<ast::CallOrVar>(ast)) {
        ConvertCallOrVar(casted_ast);
    } else if (auto casted_ast = dynamic_pointer_cast<ast::CallValue>(ast)) {
        ConvertCallValue(casted_ast);
    } else if (auto casted_ast = dynamic_pointer_cast<ast::Variable>(ast)) {
        ConvertVariable(casted_ast);
    } else if (auto casted_ast = dynamic_pointer_cast<ast::BinaryExpr>(ast)) {
        ConvertBinaryExpr(casted_ast);
    } else if (auto casted_ast = dynamic_pointer_cast<ast::UnaryExpr>(ast)) {
        ConvertUnaryExpr(casted_ast);
    }
}

const shared_ptr<Program>
ASTConverter::ConvertProgram(const shared_ptr<ast::Program> &node) {
    // Get name
    auto head = node->program_head();
    string name = head->id();
    // Drop parameters, as they are redundency.

    // Get Block
    // Get Declarations
    auto body = node->program_body();
    vector<shared_ptr<VarDeclaration>> var_declarations;
    for (auto &vd : body->var_declarations()) {
        auto converted_var_declaration = ConvertVarDeclaration(vd);
        var_declarations.push_back(std::move(converted_var_declaration));
    }

    vector<shared_ptr<ConstDeclaration>> const_declarations;
    for (auto &cd : body->const_declarations()) {
        auto converted_const_declaration = ConvertConstDeclaration(cd);
        const_declarations.push_back(std::move(converted_const_declaration));
    }

    vector<shared_ptr<Subprogram>> subprograms;
    for (auto &sp : body->subprogram_declarations()) {
        auto converted_subprogram = ConvertSubprogram(sp);
        subprograms.push_back(std::move(converted_subprogram));
    }

    vector<shared_ptr<ASTNode>> declarations;
    for (auto &a : var_declarations)
        declarations.push_back(std::move(a));
    for (auto &a : const_declarations)
        declarations.push_back(std::move(a));
    for (auto &a : subprograms)
        declarations.push_back(std::move(a));

    // Get Compound
    auto statements = body->statements();
    // TODO: Construct compound from statements
}

const vector<shared_ptr<Var>>
ASTConverter::ConvertIdList(const shared_ptr<ast::IdList> &node) {}
const shared_ptr<Type> ConvertType(const shared_ptr<ast::Type> &node) {}
const shared_ptr<Argument>
ASTConverter::ConvertParameter(const shared_ptr<ast::Parameter> &node) {}
const shared_ptr<ConstDeclaration> ASTConverter::ConvertConstDeclaration(
    const shared_ptr<ast::ConstDeclaration> &node) {}
const shared_ptr<VarDeclaration> ASTConverter::ConvertVarDeclaration(
    const shared_ptr<ast::VarDeclaration> &node) {}
const shared_ptr<Subprogram>
ASTConverter::ConvertSubprogram(const shared_ptr<ast::Subprogram> &node) {}
const shared_ptr<Assignment> ASTConverter::ConvertAssignmentStatement(
    const shared_ptr<ast::AssignStatement> &node) {}
const shared_ptr<Compound> ASTConverter::ConvertCompoundStatement(
    const shared_ptr<ast::CompoundStatement> &node) {}
const shared_ptr<IfStatement>
ASTConverter::ConvertIfStatement(const shared_ptr<ast::IfStatement> &node) {}
const shared_ptr<ForStatement>
ASTConverter::ConvertForStatement(const shared_ptr<ast::ForStatement> &node) {}
const shared_ptr<String>
ASTConverter::ConvertStringValue(const shared_ptr<ast::StringValue> &node) {}
const shared_ptr<Num>
ASTConverter::ConvertIntegerValue(const shared_ptr<ast::IntegerValue> &node) {}
const shared_ptr<Real>
ASTConverter::ConvertRealValue(const shared_ptr<ast::RealValue> &node) {}
const shared_ptr<Char>
ASTConverter::ConvertCharValue(const shared_ptr<ast::CharValue> &node) {}
const shared_ptr<Num>
convertBooleanValue(const shared_ptr<ast::BooleanValue> &node) {}
const shared_ptr<Var> ConvertCallOrVar(const shared_ptr<ast::CallOrVar> &node) {
}
const shared_ptr<FunctionCall>
ASTConverter::ConvertCallValue(const shared_ptr<ast::CallValue> &node) {}
const shared_ptr<Var> ConvertVariable(const shared_ptr<ast::Variable> &node) {}
const shared_ptr<BinaryOperation>
ASTConverter::ConvertBinaryExpr(const shared_ptr<ast::BinaryExpr> &node) {}
const shared_ptr<BinaryOperation>
ASTConverter::ConvertUnaryExpr(const shared_ptr<ast::UnaryExpr> &node) {}

} // namespace code_generation
} // namespace pascal2c
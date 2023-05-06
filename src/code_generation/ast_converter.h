#pragma once
#include "ast/ast.h"
#include "ast/expr.h"
#include "ast/program.h"
#include "ast/statement.h"
#include "code_generation/ast_adapter.h"
#include "code_generation/code_generator.h"
#include "code_generation/visitor.h"
#include <memory>
#include <string>
#include <vector>

namespace pascal2c {
namespace code_generation {
using ::std::shared_ptr;
using ::std::string;

class ASTConverter {
  public:
    ASTConverter(){};
    ~ASTConverter() = default;
    void Convert(const shared_ptr<ast::Ast> ast);
    const shared_ptr<code_generation::ASTRoot> &GetConvertedAST() const {
        return converted_ast_;
    }

  private:
    const shared_ptr<Program>
    ConvertProgram(const shared_ptr<ast::Program> &node);
    const vector<shared_ptr<Var>>
    ConvertIdList(const shared_ptr<ast::IdList> &node);
    const shared_ptr<Type> ConvertType(const shared_ptr<ast::Type> &node);
    const shared_ptr<Argument>
    ConvertParameter(const shared_ptr<ast::Parameter> &node);
    const shared_ptr<ConstDeclaration>
    ConvertConstDeclaration(const shared_ptr<ast::ConstDeclaration> &node);
    const shared_ptr<VarDeclaration>
    ConvertVarDeclaration(const shared_ptr<ast::VarDeclaration> &node);
    const shared_ptr<Subprogram>
    ConvertSubprogram(const shared_ptr<ast::Subprogram> &node);
    const shared_ptr<Assignment>
    ConvertAssignmentStatement(const shared_ptr<ast::AssignStatement> &node);
    const shared_ptr<Compound>
    ConvertCompoundStatement(const shared_ptr<ast::CompoundStatement> &node);
    const shared_ptr<IfStatement>
    ConvertIfStatement(const shared_ptr<ast::IfStatement> &node);
    const shared_ptr<ForStatement>
    ConvertForStatement(const shared_ptr<ast::ForStatement> &node);
    const shared_ptr<String>
    ConvertStringValue(const shared_ptr<ast::StringValue> &node);
    const shared_ptr<Num>
    ConvertIntegerValue(const shared_ptr<ast::IntegerValue> &node);
    const shared_ptr<Real>
    ConvertRealValue(const shared_ptr<ast::RealValue> &node);
    const shared_ptr<Char>
    ConvertCharValue(const shared_ptr<ast::CharValue> &node);
    const shared_ptr<Num>
    convertBooleanValue(const shared_ptr<ast::BooleanValue> &node);
    const shared_ptr<Var>
    ConvertCallOrVar(const shared_ptr<ast::CallOrVar> &node);
    const shared_ptr<FunctionCall>
    ConvertCallValue(const shared_ptr<ast::CallValue> &node);
    const shared_ptr<Var>
    ConvertVariable(const shared_ptr<ast::Variable> &node);
    const shared_ptr<BinaryOperation>
    ConvertBinaryExpr(const shared_ptr<ast::BinaryExpr> &node);
    const shared_ptr<BinaryOperation>
    ConvertUnaryExpr(const shared_ptr<ast::UnaryExpr> &node);

    shared_ptr<code_generation::ASTRoot> converted_ast_;
};
} // namespace code_generation
} // namespace pascal2c
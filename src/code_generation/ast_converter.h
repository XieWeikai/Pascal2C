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
    const vector<shared_ptr<Var>> ConvertIdList(const shared_ptr<ast::IdList>);
    const shared_ptr<Type> ConvertType(const shared_ptr<ast::Type>);
    const shared_ptr<Argument>
    ConvertParameter(const shared_ptr<ast::Parameter>);
    const shared_ptr<ConstDeclaration>
    ConvertConstDeclaration(const shared_ptr<ConstDeclaration>);
    const shared_ptr<VarDeclaration>
    ConvertVarDeclaration(const shared_ptr<ast::VarDeclaration>);
    const shared_ptr<Subprogram>
    ConvertSubprogram(const shared_ptr<ast::Subprogram>);
    const shared_ptr<Assignment>
    ConvertAssignment(const shared_ptr<ast::AssignStatement>);

    shared_ptr<code_generation::ASTRoot> converted_ast_;
};
} // namespace code_generation
} // namespace pascal2c
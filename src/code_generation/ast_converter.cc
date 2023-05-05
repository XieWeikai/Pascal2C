#include "code_generation/ast_converter.h"
#include "ast/ast.h"
#include "ast/expr.h"
#include "ast/program.h"
#include "ast/statement.h"
#include "code_generation/ast_adapter.h"
#include "code_generation/visitor.h"
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
    }
}
} // namespace code_generation
} // namespace pascal2c
#include "ast_adapter.h"
#include "code_generation/token_adapter.h"
#include <memory>

namespace pascal2c {
namespace code_generation {
ASTNode::~ASTNode() {}
VarDecl::~VarDecl() {
    var_node_.reset();
    type_node_.reset();
}

Type::~Type() { token_.reset(); }
Var::~Var() { token_.reset(); }
Oper::~Oper() { oper_.reset(); }
Num::~Num() { token_.reset(); }

void Program::Accept(Visitor &visitor) {
    visitor.VisitProgram(
        std::dynamic_pointer_cast<Program>(shared_from_this()));
}

Block::Block(const std::shared_ptr<Declaration> &declarations,
             const std::shared_ptr<Compound> &compound_statement)
    : declarations_(declarations), compound_statement_(compound_statement){};

Compound::Compound(const std::vector<std::shared_ptr<ASTNode>> &children)
    : children_(
          const_cast<std::vector<std::shared_ptr<ASTNode>> &>(children)){};

/**
 * add new node to children
 */
void Compound::AddChild(std::shared_ptr<ASTNode> node) {
    children_.push_back(node);
}
} // namespace code_generation
} // namespace pascal2c

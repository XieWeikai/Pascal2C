#include "ast_adapter.h"
#include "code_generation/token_adapter.h"

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

Block::Block(const vector<std::shared_ptr<ASTNode>> &declarations,
             const std::shared_ptr<Compound> &compound_statement)
    : declarations_(
          const_cast<vector<std::shared_ptr<ASTNode>> &>(declarations)),
      compound_statement_(compound_statement){};

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

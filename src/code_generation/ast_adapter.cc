#include "ast_adapter.h"

namespace pascal2c {
namespace semantic {
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
} // namespace semantic
} // namespace pascal2c

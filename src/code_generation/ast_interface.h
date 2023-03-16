#ifndef PASCAL2C_SRC_SEMANTIC_AST_H_
#define PASCAL2C_SRC_SEMANTIC_AST_H_
#include <memory>
#include <string>
#include <vector>

#include "lexer/token.h"

namespace pascal2c {
namespace semantic {
using string = ::std::string;
template <typename Tp> using vector = ::std::vector<Tp>;

class ASTNode {
  public:
    ASTNode(){};
    virtual ~ASTNode() = default;
};

// ASTRoot is an alias of ASTNode, represents root node of the AST.
typedef ASTNode ASTRoot;

class Block;
class Program : public ASTNode {
  public:
    Program(const string &name, const std::shared_ptr<Block> &block)
        : name_(name), block_(block){};

  private:
    string name_;
    std::shared_ptr<Block> block_;
};

class Compound;
class Block : public ASTNode {
  public:
    Block(const vector<std::shared_ptr<ASTNode>> &declarations,
          const std::shared_ptr<Compound> &compound_statement);

  private:
    vector<std::shared_ptr<ASTNode>> &declarations_;
    std::shared_ptr<Compound> compound_statement_;
};

class Var;
class Type;
class VarDecl : public ASTNode {
  public:
    VarDecl(const std::shared_ptr<Var> &var_node,
            const std::shared_ptr<Type> &type_node)
        : var_node_(var_node), type_node_(type_node){};

  private:
    std::shared_ptr<Var> var_node_;
    std::shared_ptr<Type> type_node_;
};

class Type : public ASTNode {
  public:
    Type(const std::shared_ptr<lexer::Token> token)
        : token_(token), type_(token->getType()){};

  private:
    std::shared_ptr<lexer::Token> token_;
    const lexer::TokenType type_;
};

class Compound : public ASTNode {
  public:
    Compound(){};
    explicit Compound(const std::vector<std::shared_ptr<ASTNode>> &children);
    void AddChild(std::shared_ptr<ASTNode> node);

  private:
    vector<std::shared_ptr<ASTNode>> children_;
};

class Assign : public ASTNode {
  public:
    Assign(const std::shared_ptr<ASTNode> &left, const lexer::Token &token,
           const std::shared_ptr<ASTNode> &right)
        : left_(left), token_(token), right_(right){};

  private:
    std::shared_ptr<ASTNode> left_;
    lexer::Token token_;
    std::shared_ptr<ASTNode> right_;
};

class Var : public ASTNode {
  public:
    Var(const std::shared_ptr<lexer::Token> &token)
        : token_(token), value_(token->GetValue()){};

  private:
    std::shared_ptr<lexer::Token> token_;
    string value_;
};

class Term : public ASTNode {
  public:
    Term() {}
};

class Factor : public ASTNode {
  public:
    Factor() {}
};

class Expr : public ASTNode {
  public:
    Expr() {}

  private:
    vector<std::shared_ptr<ASTNode>> children_;
};

class Num : public ASTNode {
  public:
    Num(std::shared_ptr<lexer::Token> &token)
        : token_(token), value_(token->GetValue()) {}

  private:
    std::shared_ptr<lexer::Token> token_;
    string value_;
};

class BinOp : public ASTNode {
  public:
    explicit BinOp(std::shared_ptr<Var> &left,
                   std::shared_ptr<lexer::Token> &token,
                   std::shared_ptr<Expr> &right)
        : left_(left), token_(token), right_(right) {}

    explicit BinOp(std::shared_ptr<Var> &left,
                   std::shared_ptr<lexer::Token> &token,
                   std::shared_ptr<Var> &right)
        : left_(left), token_(token), right_(right) {}

    explicit BinOp(std::shared_ptr<Expr> &left,
                   std::shared_ptr<lexer::Token> &token,
                   std::shared_ptr<Var> &right)
        : left_(left), token_(token), right_(right) {}

    explicit BinOp(std::shared_ptr<Expr> &left,
                   std::shared_ptr<lexer::Token> &token,
                   std::shared_ptr<Expr> &right)
        : left_(left), token_(token), right_(right) {}

  private:
    std::shared_ptr<ASTNode> left_;
    std::shared_ptr<lexer::Token> token_;
    std::shared_ptr<ASTNode> right_;
};

class NoOp : public ASTNode {
  public:
    NoOp(){};
};

} // namespace semantic
} // namespace pascal2c
#endif // !PASCAL2C_SRC_SEMANTIC_AST_H_

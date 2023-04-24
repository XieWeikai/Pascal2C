#ifndef PASCAL2C_SRC_CODE_GENERATION_AST_ADAPTER_H_
#define PASCAL2C_SRC_CODE_GENERATION_AST_ADAPTER_H_
#include <memory>
#include <string>
#include <vector>

#include "token_adapter.h"

namespace pascal2c {
namespace code_generation {
using string = ::std::string;
template <typename Tp> using vector = ::std::vector<Tp>;

class ASTNode {
  public:
    ASTNode() = default;
    virtual ~ASTNode() = 0;
};

// ASTRoot is an alias of ASTNode, represents root node of the AST.
typedef ASTNode ASTRoot;

class Block;
class Program : public ASTNode {
  public:
    Program(const string &name, const std::shared_ptr<Block> &block)
        : name_(name), block_(block){};
    string GetName() const { return name_; }
    const std::shared_ptr<Block> &GetBlock() const { return block_; }

  private:
    string name_;
    std::shared_ptr<Block> block_;
};

class Compound;
class Declaration : public ASTNode {
  public:
    Declaration(const vector<std::shared_ptr<ASTNode>> &declarations)
        : declaration_(std::move(declarations)) {}
    const vector<std::shared_ptr<ASTNode>> &GetDeclarations() const {
        return declaration_;
    }

  private:
    vector<std::shared_ptr<ASTNode>> declaration_;
};
class Block : public ASTNode {
  public:
    Block(const std::shared_ptr<Declaration> &declarations,
          const std::shared_ptr<Compound> &compound_statement);
    const vector<std::shared_ptr<ASTNode>> &GetDeclarations() const {
        return declarations_->GetDeclarations();
    }
    const std::shared_ptr<Compound> &GetCompoundStatement() const {
        return compound_statement_;
    }

  private:
    std::shared_ptr<Declaration> declarations_;
    std::shared_ptr<Compound> compound_statement_;
};

class Var;
class Type;
class VarDecl : public ASTNode {
  public:
    VarDecl(const std::shared_ptr<Var> &var_node,
            const std::shared_ptr<Type> &type_node)
        : var_node_(var_node), type_node_(type_node){};
    virtual ~VarDecl();
    const std::shared_ptr<Var> &GetVarNode() const { return var_node_; }
    const std::shared_ptr<Type> &GetTypeNode() const { return type_node_; }

  private:
    std::shared_ptr<Var> var_node_;
    std::shared_ptr<Type> type_node_;
};

class Type : public ASTNode {
  public:
    Type(const std::shared_ptr<Token> token)
        : token_(token), type_(token->GetValue()){};
    virtual ~Type();
    const string GetType() const { return type_; }

  private:
    std::shared_ptr<Token> token_;
    string type_;
};

class Compound : public ASTNode {
  public:
    Compound(){};
    explicit Compound(const std::vector<std::shared_ptr<ASTNode>> &children);
    void AddChild(std::shared_ptr<ASTNode> node);
    const vector<std::shared_ptr<ASTNode>> &GetChildren() const {
        return children_;
    }

  private:
    vector<std::shared_ptr<ASTNode>> children_;
};

class Assign : public ASTNode {
  public:
    Assign(const std::shared_ptr<ASTNode> &left,
           const std::shared_ptr<ASTNode> &right)
        : left_(left), right_(right){};
    const std::shared_ptr<ASTNode> &GetLeft() const { return left_; }
    const std::shared_ptr<ASTNode> &GetRight() const { return right_; }

  private:
    std::shared_ptr<ASTNode> left_;
    std::shared_ptr<ASTNode> right_;
};

class Var : public ASTNode {
  public:
    Var(const std::shared_ptr<Token> &token)
        : token_(token), value_(token->GetValue()){};
    virtual ~Var();
    const string GetValue() const { return value_; }

  private:
    std::shared_ptr<Token> token_;
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

/**
 * @brief Expression node
 * Pseudo-code example:
 * Expr e1 = Expr(2, +, 3);
 * Expr e2 = Expr(e1, -, 1);
 */
class Expr : public ASTNode {
  public:
    Expr() {}

  private:
    vector<std::shared_ptr<ASTNode>> children_;
};

class Num : public ASTNode {
  public:
    Num(const std::shared_ptr<Token> &token)
        : token_(token), value_(std::stoi(token->GetValue())) {}
    ~Num();
    int GetValue() const { return (value_); }

  private:
    std::shared_ptr<Token> token_;
    int value_;
};

// Operators, like '+', '-', etc.
class Oper : public ASTNode {
  public:
    Oper(const std::shared_ptr<Token> &oper)
        : oper_(oper), type_(oper->GetType()), value_(oper->GetValue()) {}
    virtual ~Oper();
    const TokenType GetType() { return type_; };

  private:
    std::shared_ptr<Token> oper_;
    TokenType type_;
    string value_;
};

class BinOp : public ASTNode {
  public:
    explicit BinOp(const std::shared_ptr<ASTNode> &left,
                   const std::shared_ptr<Oper> &oper,
                   const std::shared_ptr<ASTNode> &right)
        : left_(left), oper_(oper), right_(right) {}

    const std::shared_ptr<ASTNode> &GetLeft() { return left_; }
    const std::shared_ptr<Oper> &GetOper() { return oper_; }
    const std::shared_ptr<ASTNode> &GetRight() { return right_; }

  private:
    std::shared_ptr<ASTNode> left_;
    std::shared_ptr<Oper> oper_;
    std::shared_ptr<ASTNode> right_;
};

class NoOp : public ASTNode {
  public:
    NoOp(){};
};

} // namespace code_generation
} // namespace pascal2c
#endif // !PASCAL2C_SRC_CODE_GENERATION_AST_ADAPTER_H_

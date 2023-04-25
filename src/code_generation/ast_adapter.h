#ifndef PASCAL2C_SRC_CODE_GENERATION_AST_ADAPTER_H_
#define PASCAL2C_SRC_CODE_GENERATION_AST_ADAPTER_H_
#include <utility>
#pragma once
#include <memory>
#include <string>
#include <vector>

#include "token_adapter.h"

namespace pascal2c {
namespace code_generation {
using ::std::string;
template <typename Tp> using vector = ::std::vector<Tp>;

class Visitor;

class ASTNode : public std::enable_shared_from_this<ASTNode> {
  public:
    ASTNode() = default;
    virtual ~ASTNode() = 0;
    virtual void Accept(Visitor &visitor) = 0;
};

// ASTRoot is an alias of ASTNode, represents root node of the AST.
typedef ASTNode ASTRoot;

class Block;
class Program : public ASTNode {
  public:
    Program(const string &name, const std::shared_ptr<Block> &block)
        : name_(name), block_(block){};
    void Accept(Visitor &visitor) override;
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
    void Accept(Visitor &visitor) override;
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
    void Accept(Visitor &visitor) override;
    const std::shared_ptr<Declaration> &GetDeclatation() const {
        return declarations_;
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
class VarDeclaration : public ASTNode {
  public:
    VarDeclaration(const std::shared_ptr<Var> &var_node,
                   const std::shared_ptr<Type> &type_node)
        : var_node_(var_node), type_node_(type_node){};
    void Accept(Visitor &visitor) override;
    virtual ~VarDeclaration();
    const std::shared_ptr<Var> &GetVarNode() const { return var_node_; }
    const std::shared_ptr<Type> &GetTypeNode() const { return type_node_; }

  private:
    std::shared_ptr<Var> var_node_;
    std::shared_ptr<Type> type_node_;
};

class Const;
class ConstDeclaration : public ASTNode {
  public:
    ConstDeclaration(const std::shared_ptr<Const> &const_node,
                     const std::shared_ptr<Type> &type_node)
        : const_node_(const_node), type_node_(type_node) {}
    void Accept(Visitor &visitor) override;
    virtual ~ConstDeclaration();
    const std::shared_ptr<Const> &GetConstNode() const { return const_node_; }
    const std::shared_ptr<Type> &GetTypeNode() const { return type_node_; }

  private:
    std::shared_ptr<Const> const_node_;
    std::shared_ptr<Type> type_node_;
};

class ArrayDeclaration : public ASTNode {
  public:
    ArrayDeclaration(const std::shared_ptr<Var> &var_node,
                     const std::shared_ptr<Type> &type_node,
                     const std::vector<std::pair<int, int>> &bounds)
        : var_node_(var_node), type_node_(type_node), bounds_(bounds) {}
    void Accept(Visitor &visitor) override;
    virtual ~ArrayDeclaration();
    const std::shared_ptr<Var> &GetVarNode() const { return var_node_; }
    const std::shared_ptr<Type> &GetTypeNode() const { return type_node_; }
    const std::vector<std::pair<int, int>> &GetBounds() const {
        return bounds_;
    }

  private:
    std::shared_ptr<Var> var_node_;
    std::shared_ptr<Type> type_node_;
    std::vector<std::pair<int, int>> bounds_;
};

class Type : public ASTNode {
  public:
    Type(const std::shared_ptr<Token> &token) : type_(token->GetValue()){};
    virtual ~Type();
    void Accept(Visitor &visitor) override;
    const string GetType() const { return type_; }

  private:
    string type_;
};

class Compound : public ASTNode {
  public:
    Compound(){};
    explicit Compound(const std::vector<std::shared_ptr<ASTNode>> &children);
    void Accept(Visitor &visitor) override;
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
    void Accept(Visitor &visitor) override;
    const std::shared_ptr<ASTNode> &GetLeft() const { return left_; }
    const std::shared_ptr<ASTNode> &GetRight() const { return right_; }

  private:
    std::shared_ptr<ASTNode> left_;
    std::shared_ptr<ASTNode> right_;
};

class Var : public ASTNode {
  public:
    Var(const std::shared_ptr<Token> &token) : value_(token->GetValue()) {}
    virtual ~Var();
    void Accept(Visitor &visitor) override;
    const string GetValue() const { return value_; }

  private:
    string value_;
};

class Const : public ASTNode {
  public:
    Const(const std::shared_ptr<Token> &token) : value_(token->GetValue()) {}
    virtual ~Const();
    void Accept(Visitor &visitor) override;
    const string GetValue() const { return value_; }

  private:
    string value_;
};

class Term : public ASTNode {
  public:
    Term() {}
    void Accept(Visitor &visitor) override;
};

class Factor : public ASTNode {
  public:
    Factor() {}
    void Accept(Visitor &visitor) override;
};

/**
 * @brief Expression node
 * Pseudo-code example:
 * Expr e1 = Expr(2, +, 3);
 * Expr e2 = Expr(e1, -, 1);
 */
class Statement : public ASTNode {
  public:
    Statement() {}
    void Accept(Visitor &visitor) override;

  private:
    std::shared_ptr<ASTNode> left_hand_;
    std::shared_ptr<ASTNode> right_hand_;
    vector<std::shared_ptr<ASTNode>> children_;
};

class IfStatement : public ASTNode {};

class Num : public ASTNode {
  public:
    Num(const std::shared_ptr<Token> &token)
        : value_(std::stoi(token->GetValue())) {}
    ~Num();
    void Accept(Visitor &visitor) override;
    int GetValue() const { return (value_); }

  private:
    int value_;
};

// Operators, like '+', '-', etc.
class Oper : public ASTNode {
  public:
    Oper(const std::shared_ptr<Token> &oper)
        : type_(oper->GetType()), value_(oper->GetValue()) {}
    virtual ~Oper();
    void Accept(Visitor &visitor) override;
    const TokenType GetType() { return type_; };

  private:
    TokenType type_;
    string value_;
};

class BinOp : public ASTNode {
  public:
    explicit BinOp(const std::shared_ptr<ASTNode> &left,
                   const std::shared_ptr<Oper> &oper,
                   const std::shared_ptr<ASTNode> &right)
        : left_(left), oper_(oper), right_(right) {}
    void Accept(Visitor &visitor) override;
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
    void Accept(Visitor &visitor) override;
};

} // namespace code_generation
} // namespace pascal2c

#include "visitor.h"
#endif // !PASCAL2C_SRC_CODE_GENERATION_AST_ADAPTER_H_

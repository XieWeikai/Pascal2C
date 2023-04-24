#ifndef PASCAL2C_SRC_CODE_GENERATION_AST_ADAPTER_H_
#define PASCAL2C_SRC_CODE_GENERATION_AST_ADAPTER_H_
#pragma once
#include <memory>
#include <string>
#include <vector>

#include "token_adapter.h"

namespace pascal2c {
namespace code_generation {
using string = ::std::string;
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
    void Accept(Visitor &visitor) override;
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
    void Accept(Visitor &visitor) override;
    const string GetType() const { return type_; }

  private:
    std::shared_ptr<Token> token_;
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
    Var(const std::shared_ptr<Token> &token)
        : token_(token), value_(token->GetValue()){};
    virtual ~Var();
    void Accept(Visitor &visitor) override;
    const string GetValue() const { return value_; }

  private:
    std::shared_ptr<Token> token_;
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
class Expr : public ASTNode {
  public:
    Expr() {}
    void Accept(Visitor &visitor) override;

  private:
    vector<std::shared_ptr<ASTNode>> children_;
};

class Num : public ASTNode {
  public:
    Num(const std::shared_ptr<Token> &token)
        : token_(token), value_(std::stoi(token->GetValue())) {}
    ~Num();
    void Accept(Visitor &visitor) override;
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
    void Accept(Visitor &visitor) override;
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

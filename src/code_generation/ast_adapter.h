#ifndef PASCAL2C_SRC_CODE_GENERATION_AST_ADAPTER_H_
#define PASCAL2C_SRC_CODE_GENERATION_AST_ADAPTER_H_
#pragma once
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "token_adapter.h"

namespace pascal2c {
namespace code_generation {
using ::std::shared_ptr;
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

class Compound : public ASTNode {
  public:
    Compound(){};
    explicit Compound(const std::vector<shared_ptr<ASTNode>> &children);
    void Accept(Visitor &visitor) override;
    void AddChild(shared_ptr<ASTNode> node);
    const vector<shared_ptr<ASTNode>> &GetChildren() const { return children_; }

  private:
    vector<shared_ptr<ASTNode>> children_;
};

class Declaration : public ASTNode {
  public:
    Declaration(const vector<shared_ptr<ASTNode>> &declarations)
        : declaration_(std::move(declarations)) {}
    void Accept(Visitor &visitor) override;
    const vector<shared_ptr<ASTNode>> &GetDeclarations() const {
        return declaration_;
    }

  private:
    vector<shared_ptr<ASTNode>> declaration_;
};

class Block : public ASTNode {
  public:
    Block(const shared_ptr<Declaration> &declarations,
          const shared_ptr<Compound> &compound_statement);
    void Accept(Visitor &visitor) override;
    const shared_ptr<Declaration> &GetDeclatation() const {
        return declarations_;
    }
    const shared_ptr<Compound> &GetCompoundStatement() const {
        return compound_statement_;
    }

  private:
    shared_ptr<Declaration> declarations_;
    shared_ptr<Compound> compound_statement_;
};

class Var : public ASTNode {
  public:
    Var(const shared_ptr<Token> &token) : value_(token->GetValue()) {}
    virtual ~Var();
    void Accept(Visitor &visitor) override;
    const string GetValue() const { return value_; }

  private:
    string value_;
};

class Type : public ASTNode {
  public:
    Type(const shared_ptr<Token> &token) : type_(token->GetValue()){};
    virtual ~Type();
    void Accept(Visitor &visitor) override;
    const string GetType() const {
        return (is_const_) ? (string("const ") + type_) : type_;
    }

  private:
    bool is_const_;
    string type_;
};

class VarDeclaration : public ASTNode {
  public:
    VarDeclaration(const shared_ptr<Var> &var_node,
                   const shared_ptr<Type> &type_node)
        : var_node_(var_node), type_node_(type_node){};
    void Accept(Visitor &visitor) override;
    virtual ~VarDeclaration();
    const shared_ptr<Var> &GetVarNode() const { return var_node_; }
    const shared_ptr<Type> &GetTypeNode() const { return type_node_; }

  private:
    shared_ptr<Var> var_node_;
    shared_ptr<Type> type_node_;
};

class ConstDeclaration : public ASTNode {
  public:
    ConstDeclaration(const shared_ptr<Var> &var_node,
                     const shared_ptr<Type> &type_node)
        : var_node_(var_node), type_node_(type_node) {}
    void Accept(Visitor &visitor) override;
    virtual ~ConstDeclaration();
    const shared_ptr<Var> &GetConstNode() const { return var_node_; }
    const shared_ptr<Type> &GetTypeNode() const { return type_node_; }

  private:
    shared_ptr<Var> var_node_;
    shared_ptr<Type> type_node_;
};

class ArrayDeclaration : public ASTNode {
  public:
    ArrayDeclaration(const shared_ptr<Var> &var_node,
                     const shared_ptr<Type> &type_node,
                     const std::vector<std::pair<int, int>> &bounds)
        : var_node_(var_node), type_node_(type_node), bounds_(bounds) {}
    void Accept(Visitor &visitor) override;
    virtual ~ArrayDeclaration();
    const shared_ptr<Var> &GetVarNode() const { return var_node_; }
    const shared_ptr<Type> &GetTypeNode() const { return type_node_; }
    const std::vector<std::pair<int, int>> &GetBounds() const {
        return bounds_;
    }

  private:
    shared_ptr<Var> var_node_;
    shared_ptr<Type> type_node_;
    std::vector<std::pair<int, int>> bounds_;
};

class Subprogram : public ASTNode {
  public:
    Subprogram(const string &name, const shared_ptr<Block> &block)
        : name_(name), block_(block) {}
    void Accept(Visitor &visitor) override;
    const string GetName() const { return name_; }
    const shared_ptr<Block> &GetBlock() const { return block_; }

  private:
    string name_;
    shared_ptr<Block> block_;
};

class Function : public ASTNode {
  public:
    Function(const string &name, const shared_ptr<Type> &return_type,
             const vector<shared_ptr<VarDeclaration>> &params,
             const shared_ptr<Block> &block)
        : name_(name), return_type_(return_type), params_(params),
          block_(block) {}
    void Accept(Visitor &visitor) override;
    const string GetName() const { return name_; }
    const shared_ptr<Type> GetReturnType() const { return return_type_; }
    const vector<shared_ptr<VarDeclaration>> &GetParams() const {
        return params_;
    }
    const shared_ptr<Block> &GetBlock() const { return block_; }

  private:
    string name_;
    shared_ptr<Type> return_type_;
    vector<shared_ptr<VarDeclaration>> params_;
    shared_ptr<Block> block_;
};

class Program : public ASTNode {
  public:
    Program(const string &name, const shared_ptr<Block> &block)
        : name_(name), block_(block){};
    void Accept(Visitor &visitor) override;
    const string GetName() const { return name_; }
    const shared_ptr<Block> &GetBlock() const { return block_; }

  private:
    string name_;
    shared_ptr<Block> block_;
};

class Assign : public ASTNode {
  public:
    Assign(const shared_ptr<ASTNode> &left, const shared_ptr<ASTNode> &right)
        : left_(left), right_(right){};
    void Accept(Visitor &visitor) override;
    const shared_ptr<ASTNode> &GetLeft() const { return left_; }
    const shared_ptr<ASTNode> &GetRight() const { return right_; }

  private:
    shared_ptr<ASTNode> left_;
    shared_ptr<ASTNode> right_;
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
    Statement(const shared_ptr<ASTNode> &left_hand,
              const shared_ptr<ASTNode> &right_hand,
              const vector<shared_ptr<ASTNode>> &children)
        : left_hand_(left_hand), right_hand_(right_hand), children_(children) {}
    void Accept(Visitor &visitor) override;

  private:
    shared_ptr<ASTNode> left_hand_;
    shared_ptr<ASTNode> right_hand_;
    vector<shared_ptr<ASTNode>> children_;
};

class IfStatement : public ASTNode {
  public:
    IfStatement(const shared_ptr<ASTNode> &condition,
                const shared_ptr<ASTNode> &then_branch,
                const shared_ptr<ASTNode> &else_branch = nullptr)
        : condition_(condition), then_branch_(then_branch),
          else_branch_(else_branch) {}
    void Accept(Visitor &visitor) override;
    const shared_ptr<ASTNode> &GetCondition() const { return condition_; }
    const shared_ptr<ASTNode> &GetThenBranch() const { return then_branch_; }
    const shared_ptr<ASTNode> &GetElseBranch() const { return else_branch_; }

  private:
    shared_ptr<ASTNode> condition_;
    shared_ptr<ASTNode> then_branch_;
    shared_ptr<ASTNode> else_branch_;
};

class ForStatement : public ASTNode {
  public:
    ForStatement(const shared_ptr<ASTNode> &variable,
                 const shared_ptr<ASTNode> &start,
                 const shared_ptr<ASTNode> &end,
                 const shared_ptr<ASTNode> &body)
        : variable_(variable), start_(start), end_(end), body_(body) {}
    void Accept(Visitor &visitor) override;
    const shared_ptr<ASTNode> &GetVariable() const { return variable_; }
    const shared_ptr<ASTNode> &GetStart() const { return start_; }
    const shared_ptr<ASTNode> &GetEnd() const { return end_; }
    const shared_ptr<ASTNode> &GetBody() const { return body_; }

  private:
    shared_ptr<ASTNode> variable_;
    shared_ptr<ASTNode> start_;
    shared_ptr<ASTNode> end_;
    shared_ptr<ASTNode> body_;
};

class Num : public ASTNode {
  public:
    Num(const shared_ptr<Token> &token)
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
    Oper(const shared_ptr<Token> &oper)
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
    explicit BinOp(const shared_ptr<ASTNode> &left,
                   const shared_ptr<Oper> &oper,
                   const shared_ptr<ASTNode> &right)
        : left_(left), oper_(oper), right_(right) {}
    void Accept(Visitor &visitor) override;
    const shared_ptr<ASTNode> &GetLeft() { return left_; }
    const shared_ptr<Oper> &GetOper() { return oper_; }
    const shared_ptr<ASTNode> &GetRight() { return right_; }

  private:
    shared_ptr<ASTNode> left_;
    shared_ptr<Oper> oper_;
    shared_ptr<ASTNode> right_;
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

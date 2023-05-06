#ifndef PASCAL2C_SRC_CODE_GENERATION_AST_ADAPTER_H_
#define PASCAL2C_SRC_CODE_GENERATION_AST_ADAPTER_H_
#include <algorithm>
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
    virtual ~ASTNode() = default;
    virtual void Accept(Visitor &visitor) = 0;
};

// ASTRoot is an alias of ASTNode, representing the root node of an AST.
typedef ASTNode ASTRoot;

class Compound : public ASTNode {
  public:
    Compound(){};
    virtual ~Compound() = default;
    explicit Compound(const std::vector<std::shared_ptr<ASTNode>> &children)
        : children_(children) {}
    void Accept(Visitor &visitor) override;
    void AddChild(shared_ptr<ASTNode> node);
    const vector<shared_ptr<ASTNode>> &GetChildren() const { return children_; }

  private:
    vector<shared_ptr<ASTNode>> children_;
};

class Declaration : public ASTNode {
  public:
    Declaration() : declaration_() {}
    Declaration(const vector<shared_ptr<ASTNode>> &declarations)
        : declaration_(declarations) {}
    virtual ~Declaration() = default;
    void Accept(Visitor &visitor) override;
    const vector<shared_ptr<ASTNode>> &GetDeclarations() const {
        return declaration_;
    }

  private:
    vector<shared_ptr<ASTNode>> declaration_;
};

class Block : public ASTNode {
  public:
    Block(shared_ptr<Compound> &compound_statement)
        : declarations_(std::make_shared<Declaration>()),
          compound_statement_(compound_statement) {}
    Block(const shared_ptr<Declaration> &declarations,
          const shared_ptr<Compound> &compound_statement)
        : declarations_(declarations), compound_statement_(compound_statement) {
    }
    virtual ~Block() = default;
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

class Num : public ASTNode {
  public:
    Num(const shared_ptr<Token> &token)
        : value_(std::stoi(token->GetValue())) {}
    virtual ~Num() = default;
    void Accept(Visitor &visitor) override;
    int GetValue() const { return (value_); }

  private:
    int value_;
};

class String : public ASTNode {
  public:
    String(const shared_ptr<Token> &token)
        : value_(std::move(token->GetValue())) {}
    virtual ~String() = default;
    void Accept(Visitor &visitor) override;
    const string GetValue() const { return value_; };

  private:
    string value_;
};

class Real : public ASTNode {
  public:
    Real(const shared_ptr<Token> &token)
        : value_(std::move(token->GetValue())) {}
    virtual ~Real() = default;
    void Accept(Visitor &visitor) override;
    const string GetValue() const { return value_; }

  private:
    string value_;
};

class Char : public ASTNode {
  public:
    Char(const shared_ptr<Token> &token)
        : value_(std::move(token->GetValue())) {}
    virtual ~Char() = default;
    void Accept(Visitor &visitor) override;
    const string GetValue() const { return value_; }

  private:
    string value_;
};

class IVar : public ASTNode {
  public:
    virtual ~IVar() = default;
    virtual void Accept(Visitor &visitor) = 0;
    virtual const string GetName() const = 0;
};

class Var : public IVar {
  public:
    explicit Var(const shared_ptr<Token> &token) : name_(token->GetValue()) {}
    explicit Var(const string &name) : name_(name) {}
    virtual ~Var() = default;
    void Accept(Visitor &visitor) override;
    virtual const string GetName() const override { return name_; }

  private:
    string name_;
};

class IType : public ASTNode {
  public:
    virtual ~IType() = default;
    virtual void Accept(Visitor &visitor) = 0;
    virtual const string GetType() const = 0;
};

class Type : public IType {
  public:
    Type(const shared_ptr<Token> &token) : type_(token->GetValue()) {}
    virtual ~Type() = default;
    void Accept(Visitor &visitor) override;
    const string GetType() const override { return type_; }

  private:
    string type_;
};

class ConstType : public IType {
  public:
    ConstType(const shared_ptr<Token> &token) : type_(token->GetValue()) {}
    virtual ~ConstType() = default;
    void Accept(Visitor &visitor) override;
    const string GetType() const override { return type_; }

  private:
    string type_;
};

class VarDeclaration : public ASTNode {
  public:
    VarDeclaration(const shared_ptr<Var> &var_node,
                   const shared_ptr<Type> &type_node)
        : var_node_(var_node), type_node_(type_node){};
    virtual ~VarDeclaration() = default;
    void Accept(Visitor &visitor) override;
    const shared_ptr<Var> &GetVarNode() const { return var_node_; }
    const shared_ptr<Type> &GetTypeNode() const { return type_node_; }

  private:
    shared_ptr<Var> var_node_;
    shared_ptr<Type> type_node_;
};

class ConstDeclaration : public ASTNode {
  public:
    ConstDeclaration(const shared_ptr<Var> &var_node,
                     const shared_ptr<ConstType> &type_node)
        : var_node_(var_node), type_node_(type_node) {}
    virtual ~ConstDeclaration() = default;
    void Accept(Visitor &visitor) override;
    const shared_ptr<Var> &GetConstNode() const { return var_node_; }
    const shared_ptr<ConstType> &GetTypeNode() const { return type_node_; }

  private:
    shared_ptr<Var> var_node_;
    shared_ptr<ConstType> type_node_;
};

class ArrayType : public IType {
  public:
    ArrayType(const shared_ptr<Type> &type,
              vector<std::pair<int, int>> const bounds)
        : type_(type), bounds_(bounds) {}
    virtual ~ArrayType() = default;
    void Accept(Visitor &visitor) override;
    const string GetType() const override { return type_->GetType(); }
    const std::vector<std::pair<int, int>> &GetBounds() const {
        return bounds_;
    }

  private:
    shared_ptr<Type> type_;
    vector<std::pair<int, int>> bounds_;
};

class Array : public IVar {
  public:
    Array(const shared_ptr<Var> &var) : var_(var) {}
    virtual ~Array() = default;
    void Accept(Visitor &visitor) override;
    const string GetName() const override { return var_->GetName(); }
    const shared_ptr<Var> &GetVarNode() const { return var_; }

  private:
    shared_ptr<Var> var_;
};

class ArrayDeclaration : public ASTNode {
  public:
    ArrayDeclaration(const shared_ptr<Array> &array_node,
                     const shared_ptr<ArrayType> &type_node)
        : array_node_(array_node), type_node_(type_node) {}
    virtual ~ArrayDeclaration() = default;
    void Accept(Visitor &visitor) override;
    const shared_ptr<Array> &GetArrayNode() const { return array_node_; }
    const shared_ptr<ArrayType> &GetTypeNode() const { return type_node_; }

  private:
    shared_ptr<Array> array_node_;
    shared_ptr<ArrayType> type_node_;
};

// Access array member values by indes
class ArrayAccess : public IVar {
  public:
    ArrayAccess(const shared_ptr<Array> &array,
                const vector<shared_ptr<Num>> &indices)
        : array_(array), indices_(indices) {}
    virtual ~ArrayAccess() = default;
    void Accept(Visitor &visitor) override;
    const shared_ptr<Array> &GetArray() const { return array_; }
    const vector<shared_ptr<Num>> &GetIndices() const { return indices_; }
    const string GetName() const override { return array_->GetName(); }

  private:
    shared_ptr<Array> array_;
    vector<shared_ptr<Num>> indices_;
};

class Argument : public ASTNode {
  public:
    explicit Argument(const shared_ptr<IVar> &variable,
                      const shared_ptr<IType> &type, bool is_reference = false)
        : variable_(variable), type_(type), is_reference_(is_reference) {}
    virtual ~Argument() = default;
    void Accept(Visitor &visitor) override;
    const shared_ptr<IVar> &GetVariable() const { return variable_; }
    const shared_ptr<IType> &GetType() const { return type_; }
    const bool IsReference() const { return is_reference_; }

  private:
    shared_ptr<IVar> variable_;
    shared_ptr<IType> type_;
    bool is_reference_;
};

class Subprogram : public ASTNode {
  public:
    Subprogram(const string &name, const vector<shared_ptr<Argument>> &args,
               const shared_ptr<Block> &block)
        : name_(name), args_(args), block_(block) {}
    virtual ~Subprogram() = default;
    void Accept(Visitor &visitor) override;
    const string GetName() const { return name_; }
    const vector<shared_ptr<Argument>> &GetArgs() const { return args_; }
    const shared_ptr<Block> &GetBlock() const { return block_; }

  private:
    string name_;
    vector<shared_ptr<Argument>> args_;
    shared_ptr<Block> block_;
};

class Function : public ASTNode {
  public:
    Function(const string &name, const shared_ptr<Type> &return_type,
             const vector<shared_ptr<Argument>> &args,
             const shared_ptr<Block> &block)
        : name_(name), return_type_(return_type), args_(args), block_(block) {}
    virtual ~Function() = default;
    void Accept(Visitor &visitor) override;
    const string GetName() const { return name_; }
    const string GetReturnType() const { return return_type_->GetType(); }
    const vector<shared_ptr<Argument>> &GetArgs() const { return args_; }
    const shared_ptr<Block> &GetBlock() const { return block_; }

  private:
    string name_;
    shared_ptr<Type> return_type_;
    vector<shared_ptr<Argument>> args_;
    shared_ptr<Block> block_;
};

class Program : public ASTNode {
  public:
    Program(const string &name, const shared_ptr<Block> &block)
        : name_(name), block_(block){};
    virtual ~Program() = default;
    void Accept(Visitor &visitor) override;
    const string GetName() const { return name_; }
    const shared_ptr<Block> &GetBlock() const { return block_; }

  private:
    string name_;
    shared_ptr<Block> block_;
};

class Assignment : public ASTNode {
  public:
    Assignment(const shared_ptr<ASTNode> &left,
               const shared_ptr<ASTNode> &right)
        : left_(left), right_(right){};
    virtual ~Assignment() = default;
    void Accept(Visitor &visitor) override;
    const shared_ptr<ASTNode> &GetLeft() const { return left_; }
    const shared_ptr<ASTNode> &GetRight() const { return right_; }

  private:
    shared_ptr<ASTNode> left_;
    shared_ptr<ASTNode> right_;
};

// Operators, like '+', '-', etc.
class Oper : public ASTNode {
  public:
    Oper(const shared_ptr<Token> &oper) : oper_(oper->GetValue()) {}
    virtual ~Oper() = default;
    void Accept(Visitor &visitor) override;
    const string GetOper() { return oper_; };

  private:
    string oper_;
};

class BinaryOperation : public ASTNode {
  public:
    explicit BinaryOperation(const shared_ptr<ASTNode> &left,
                             const shared_ptr<Oper> &oper,
                             const shared_ptr<ASTNode> &right)
        : left_(left), oper_(oper), right_(right) {}
    virtual ~BinaryOperation() = default;
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
    virtual ~NoOp() = default;
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
              const shared_ptr<ASTNode> &right_hand)
        : left_hand_(left_hand), right_hand_(right_hand) {}
    virtual ~Statement() = default;
    void Accept(Visitor &visitor) override;
    const shared_ptr<ASTNode> &GetLeftHand() const { return left_hand_; }
    const shared_ptr<ASTNode> &GetRightHand() const { return right_hand_; }

  private:
    shared_ptr<ASTNode> left_hand_;
    shared_ptr<ASTNode> right_hand_;
};

class IfStatement : public ASTNode {
  public:
    IfStatement(const shared_ptr<ASTNode> &condition,
                const shared_ptr<Compound> &then_branch,
                const shared_ptr<Compound> &else_branch = nullptr)
        : condition_(condition), then_branch_(then_branch),
          else_branch_(else_branch) {}
    virtual ~IfStatement() = default;
    void Accept(Visitor &visitor) override;
    const shared_ptr<ASTNode> &GetCondition() const { return condition_; }
    const shared_ptr<Compound> &GetThenBranch() const { return then_branch_; }
    const shared_ptr<Compound> &GetElseBranch() const { return else_branch_; }

  private:
    shared_ptr<ASTNode> condition_;
    shared_ptr<Compound> then_branch_;
    shared_ptr<Compound> else_branch_;
};

class ForStatement : public ASTNode {
  public:
    ForStatement(const shared_ptr<Var> &variable, const shared_ptr<Num> &start,
                 const shared_ptr<Num> &end, const shared_ptr<Compound> &body)
        : variable_(variable), start_(start), end_(end), body_(body) {}
    virtual ~ForStatement() = default;
    void Accept(Visitor &visitor) override;
    const shared_ptr<Var> &GetVariable() const { return variable_; }
    const shared_ptr<Num> &GetStart() const { return start_; }
    const shared_ptr<Num> &GetEnd() const { return end_; }
    const shared_ptr<Compound> &GetBody() const { return body_; }

  private:
    shared_ptr<Var> variable_;
    shared_ptr<Num> start_;
    shared_ptr<Num> end_;
    shared_ptr<Compound> body_;
};

class FunctionCall : public ASTNode {
  public:
    FunctionCall(const string &name,
                 const vector<shared_ptr<ASTNode>> parameters)
        : name_(name), parameters_(std::move(parameters)) {}
    FunctionCall(const string &name) : name_(name), parameters_() {}
    virtual ~FunctionCall() = default;
    void Accept(Visitor &visitor) override;
    const string GetName() const { return name_; }
    const vector<shared_ptr<ASTNode>> &GetParameters() { return parameters_; }

  private:
    string name_;
    vector<shared_ptr<ASTNode>> parameters_;
};

} // namespace code_generation
} // namespace pascal2c

#include "visitor.h"
#endif // !PASCAL2C_SRC_CODE_GENERATION_AST_ADAPTER_H_

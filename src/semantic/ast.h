#ifndef PASCAL2C_SRC_SEMANTIC_AST_H_
#define PASCAL2C_SRC_SEMANTIC_AST_H_
#include <string>
#include <vector>

#include "nodes.h"

namespace pascal2c {
namespace semantic {
using string = ::std::string;
template <typename Tp> using vector = ::std::vector<Tp>;

class Token {
  public:
    Token(const string &value) : value_(value){};
    Token(const Token &token) : value_(token.value_){};
    const string GetValue() const { return value_; }

  private:
    string value_;
};

class AST {
  public:
    AST(){};
};

class Program : public AST {
  public:
    Program(const string &name, const string &block)
        : name_(name), block_(block){};

  private:
    string name_;
    string block_;
};

class Block : public AST {
  public:
    Block(const vector<string> &declarations, const string &compound_statement)
        : compound_statement_(compound_statement) {
        for (auto &dec : declarations) {
            declarations_.push_back(dec);
        }
    };

  private:
    vector<string> declarations_;
    string compound_statement_;
};

class VarDecl : public AST {
  public:
    VarDecl(const VarNode &var_node, const TypeNode &type_node)
        : var_node_(var_node), type_node_(type_node){};

  private:
    VarNode var_node_;
    TypeNode type_node_;
};

class Type : public AST {
  public:
    Type(){};

  private:
    vector<Node> children_;
};

class Compound : public AST {
  public:
    Compound(){};

  private:
    vector<Node> Children_;
};

class Assign : public AST {
  public:
    Assign(const Node &left, const Token &token, const Node &right)
        : left_(left), token_(token), right_(right){};

  private:
    Node left_;
    Token token_;
    Node right_;
};

class Var : public AST {
  public:
    Var(const Token &token) : token_(token), value_(token.GetValue()){};

  private:
    Token token_;
    string value_;
};

class NoOp : public AST {
  public:
    NoOp(){};
};

} // namespace semantic
} // namespace pascal2c
#endif // !PASCAL2C_SRC_SEMANTIC_AST_H_

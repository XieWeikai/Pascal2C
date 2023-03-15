#ifndef PASCAL2C_SRC_SEMANTIC_NDOES_H_
#define PASCAL2C_SRC_SEMANTIC_NDOES_H_
#include <string>

namespace pascal2c {
namespace semantic {
using string = ::std::string;
class Node {
  public:
    Node(){};
};

class VarNode : public Node {
  public:
    VarNode(const string &var) : var_(var){};

  private:
    string var_;
};

class TypeNode : public Node {
  public:
    TypeNode(const string &type) : type_(type){};

  private:
    string type_;
};
} // namespace semantic
} // namespace pascal2c
#endif // !PASCAL2C_SRC_SEMANTIC_NDOES_H_

#include "code_generation/ast_adapter.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace pascal2c {
namespace code_generation {
class MockCompound : public ASTNode {
  public:
    MOCK_METHOD(const vector<std::shared_ptr<ASTNode>> &, GetChildren, (),
                (const));
};
class MockBlock : public ASTNode {
  public:
    MOCK_METHOD(const vector<std::shared_ptr<ASTNode>> &, GetDeclarations, (),
                (const));
    MOCK_METHOD(const std::shared_ptr<Compound> &, GetCompoundStatement, (),
                (const));
};
class MockProgram : public ASTNode {
  public:
    MOCK_METHOD(string, GetName, (), (const));
    MOCK_METHOD(const std::shared_ptr<MockBlock> &, GetBlock, (), (const));
};

class MockVar : public ASTNode {
  public:
    MOCK_METHOD(const string, GetValue, (), (const));
};
class MockType : public ASTNode {
  public:
    MOCK_METHOD(const TokenType, GetType, (), (const));
};
class MockVarDecl : public ASTNode {
  public:
    MOCK_METHOD(const std::shared_ptr<MockVar> &, GetVarNode, (), (const));
    MOCK_METHOD(const std::shared_ptr<Type> &, GetTypeNode, (), (const));
};
class MockAssign : public ASTNode {
  public:
    MOCK_METHOD(const std::shared_ptr<ASTNode> &, GetLeft, (), (const));
    MOCK_METHOD(const std::shared_ptr<ASTNode> &, GetRight, (), (const));
};
class MockTerm : public ASTNode {
  public:
};
class MockFactor : public ASTNode {};
class MockExpr : public ASTNode {};
class MockNum : public ASTNode {
  public:
    MOCK_METHOD(int, GetValue, (), (const));
};
class MockBinOp : public ASTNode {
  public:
    MOCK_METHOD(const std::shared_ptr<ASTNode> &, GetLeft, ());
    MOCK_METHOD(const std::shared_ptr<Token> &, GetOper, ());
    MOCK_METHOD(const std::shared_ptr<ASTNode> &, GetRight, ());
};
class MockNoOp : public ASTNode {};

} // namespace code_generation
} // namespace pascal2c
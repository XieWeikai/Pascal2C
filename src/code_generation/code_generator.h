#ifndef PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATOR_H_
#define PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATOR_H_
#include <memory>
#include <sstream>
#include <string>

#include "ast_adapter.h"
#include "code_generation/abstract_symbol_table_adapter.h"
#include "code_generation/symbol_table_adapter.h"
#include "code_generation/type_adaper.h"
#include "visitor.h"

namespace pascal2c {
namespace code_generation {
using string = ::std::string;
template <typename T> using vector = ::std::vector<T>;
using ::std::shared_ptr;

class CodeGenerator : Visitor {
  public:
    CodeGenerator(const shared_ptr<ISymbolTable> &symbol_table = nullptr)
        : indent_level_(0), type_tool_kit_() {}
    void Interpret(const shared_ptr<ASTRoot> &node);
    const string GetCCode() const;

  private:
    virtual void Visit(const shared_ptr<ASTNode> &node,
                       bool indent = false) override;
    virtual void
    VisitExitStatement(const shared_ptr<ExitStatement> &node) override;
    virtual void VisitArgument(const shared_ptr<Argument> &node) override;
    virtual void VisitProgram(const shared_ptr<Program> &node) override;
    virtual void VisitSubprogram(const shared_ptr<Subprogram> &node) override;
    virtual void VisitFunction(const shared_ptr<Function> &node) override;
    virtual void VisitBlock(const shared_ptr<Block> &node) override;
    virtual void VisitDeclaration(const shared_ptr<Declaration> &node) override;
    virtual void VisitVarDecl(const shared_ptr<VarDeclaration> &node) override;
    virtual void
    VisitConstDeclaration(const shared_ptr<ConstDeclaration> &node) override;
    virtual void VisitArrayType(const shared_ptr<ArrayType> &node) override;
    virtual void VisitArray(const shared_ptr<Array> &node) override;
    virtual void
    VisitArrayDeclaration(const shared_ptr<ArrayDeclaration> &node) override;
    virtual void VisitArrayAccess(const shared_ptr<ArrayAccess> &node) override;
    virtual void VisitCompound(const shared_ptr<Compound> &node) override;
    virtual void VisitBinOp(const shared_ptr<BinaryOperation> &node) override;
    virtual void
    VisitUnaryOperation(const shared_ptr<UnaryOperation> &node) override;
    virtual void VisitOper(const shared_ptr<Oper> &node) override;
    virtual void VisitNum(const shared_ptr<Num> &node) override;
    virtual void VisitString(const shared_ptr<String> &node) override;
    virtual void VisitReal(const shared_ptr<Real> &node) override;
    virtual void VisitChar(const shared_ptr<Char> &node) override;
    virtual void VisitType(const shared_ptr<Type> &node) override;
    virtual void VisitConstType(const shared_ptr<ConstType> &node) override;
    virtual void VisitAssign(const shared_ptr<Assignment> &node) override;
    virtual void VisitVar(const shared_ptr<Var> &node) override;
    virtual void VisitNoOp(const shared_ptr<NoOp> &node) override;
    virtual void VisitStatement(const shared_ptr<Statement> &node) override;
    virtual void VisitIfStatement(const shared_ptr<IfStatement> &node) override;
    virtual void
    VisitForStatement(const shared_ptr<ForStatement> &node) override;
    virtual void
    VisitWhileStatement(const shared_ptr<WhileStatement> &node) override;
    virtual void
    VisitFunctionCall(const shared_ptr<FunctionCall> &node) override;

    // Get Symbol Table's current scope name
    // const string GetCurrentScope() const {
    // return symbol_table_->GetCurrentScope();
    // }

    // Set symbol table's current scope by name
    // void SetCurrentScope(const string &scope_name) const {
    //     symbol_table_->SetCurrentScope(scope_name);
    // }

    // Is this var passed-by-reference or passed-by-value in current scope
    bool IsReferenceArg(const shared_ptr<Var> &node) const;
    // Is return variable
    bool IsReturnVar(const shared_ptr<Var> &node) const;

    void PrintfFormatString(const shared_ptr<FunctionCall> &node,
                            bool new_line = false);

    // Get current indent blank string based on current indent level
    const string Indent() const;
    // Increase indent level
    void IncIndent();
    // Decrease indent level
    void DecIndent();
    const string SymbolToC(const string &pascal_type) const;
    const string eol_ = ";\n";

    // Symbol table
    // shared_ptr<ISymbolTable> symbol_table_;
    // ostream
    std::stringstream ostream_;
    // Current indent level
    int indent_level_;

    // TypeToolKit for type conversion
    const TypeToolKit type_tool_kit_;
};

} // namespace code_generation
} // namespace pascal2c
#endif // !PASCAL2C_SRC_CODE_GENERATION_CODE_GENERATION_H_
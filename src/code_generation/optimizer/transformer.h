#pragma once
#include "opti_common.h"


#include <unordered_map>
#include <optional>
#include <regex>

namespace pascal2c::code_generation {

class Transformer {
public : 
    /**
     * @attention It need analysis::init() has been executed.
    */
	explicit Transformer(std::shared_ptr<ast::Ast> root);
    Transformer() = delete;
    auto GetASTRoot() const {return ast_root;}

private :
	std::shared_ptr<ASTRoot> ast_root;
    analysiser::nameTable* table; // TODO : singleton
    std::shared_ptr<symbol_table::SymbolTableBlock> sym_block;
    std::shared_ptr<TypeToolKit> type_kit;

    shared_ptr<Program> transProgram(shared_ptr<ast::Program> cur);
    shared_ptr<ASTNode>
        transSubprogram(shared_ptr<ast::Subprogram> cur);

    /**
     * @brief Handle Program
    */
    template<typename T>
    shared_ptr<Block> transBody(shared_ptr<T> cur);
    template<typename T>
    shared_ptr<Declaration> transDeclaration(shared_ptr<T> body);
    shared_ptr<ConstDeclaration>
        transConstDeclaration(shared_ptr<ast::ConstDeclaration> cur);
    vector<shared_ptr<ASTNode>> 
        transVarDeclaration(shared_ptr<ast::VarDeclaration> cur);

    /**
     * @brief Handle Expressions
    */
    shared_ptr<ASTNode>
        transExpression(shared_ptr<ast::Expression> cur);
    std::pair<shared_ptr<ASTNode> , VarType>
        passExpr(shared_ptr<ast::Expression> cur);
    /**
     * @brief Handle Statements
    */
    shared_ptr<ASTNode>
        transStatement(shared_ptr<ast::Statement> cur);
    shared_ptr<Compound>
        transCompoundStatement(shared_ptr<ast::CompoundStatement> cur);
    shared_ptr<Assignment> 
        transAssignStatement(shared_ptr<ast::AssignStatement> cur);
    shared_ptr<Statement>
        transCallStatement(shared_ptr<ast::CallStatement> cur);
    shared_ptr<IfStatement>
        transIfStatement(shared_ptr<ast::IfStatement> cur);
    shared_ptr<ForStatement>
        transForStatement(shared_ptr<ast::ForStatement> cur);
    shared_ptr<ExitStatement>
        transExitStatement(shared_ptr<ast::ExitStatement> cur);
    shared_ptr<WhileStatement>
        transWhileStatement(shared_ptr<ast::WhileStatement> cur);


    std::tuple<bool ,bool , bool , bool , bool , symbol_table::ItemType>
        checkIdType(const string& id);
    std::optional<std::pair<symbol_table::ItemType , std::vector<std::pair<int , int>>>>
        checkArrayType(const string& id);
    string checkExprType(shared_ptr<ast::Expression> cur);

    bool checkIdTypeIfVar(const string& id);
    bool checkIdTypeIfConst(const string& id);   
    bool checkIdTypeIfFunc(const string& id);
    bool checkIdTypeIfRetVar(const string& id);
    bool checkIdTypeIfRef(const string& id);
};


} // End namespace 
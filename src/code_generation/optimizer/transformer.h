#pragma once
#include "opti_common.h"

namespace pascal2c::code_generation {

class Transformer {
public : 
    Transformer() = delete;
	Transformer(std::shared_ptr<ast::Program> root);
    auto getASTRoot() const {return ast_root;}

private :
	std::shared_ptr<ASTRoot> ast_root;

    shared_ptr<Program> transProgram(shared_ptr<ast::Program> cur);
    shared_ptr<ASTNode>
        transSubprogram(shared_ptr<ast::Subprogram> cur);


    template<typename T>
    shared_ptr<Declaration> transDeclaration(shared_ptr<T> body);
    shared_ptr<Assignment>
        transConstDeclaration(shared_ptr<ast::ConstDeclaration> cur);
    vector<shared_ptr<ASTNode>> 
        transVarDeclaration(shared_ptr<ast::VarDeclaration> cur);


    template<typename T>
    shared_ptr<Block> transBody(shared_ptr<T> cur);

    shared_ptr<ASTNode>
        transExpression(shared_ptr<ast::Expression> cur);

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

};


} // End namespace 
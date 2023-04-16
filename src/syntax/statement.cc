//
// Created by 谢卫凯 on 2023/3/24.
//
#include <set>

#include "syntax.h"
#include "ast/statement.h"

#include "lexer.h"

namespace Pascal2C::Syntax 
{
    template<typename Tp> using vector = ::std::vector<Tp>;

    static bool isStatementStartTok(int tok){
        static std::set<int> tokens = {TOK_IF,TOK_ID,TOK_BEGIN,TOK_ID};
        auto it = tokens.find(tok);
        if(it != tokens.end())
            return true;
        return false;
    }

    std::shared_ptr<ast::Statement> Parser::ParseStatement(){
        static char buff[1024];
        switch (cur.token) 
        {
        case TOK_IF:
            return std::move(ParseIFStatement());
        case TOK_FOR:
            return std::move(ParseForStatement());
        case TOK_BEGIN:
            return std::move(ParseCompoundStatement());
        case TOK_ID:
            return std::move(ParseAssignAndCallStatement());

        default:
            sprintf(buff ,"%d:%d: not expected token to parse statement" ,
                            cur.line ,cur.column);
            throw SyntaxErr{buff};
        }
        return nullptr;
    }

    std::shared_ptr<ast::Statement> Parser::ParseIFStatement(){
        Match(TOK_IF);
        auto cond = ParseExpr();
        Match(TOK_THEN);
        auto statement = ParseStatement();

        if(cur.token == TOK_ELSE) {
            Match(TOK_ELSE);
            auto else_part = ParseStatement();
            return std::move(std::make_shared<ast::IfStatement>(cond,statement,else_part));
        }

        return std::move(std::make_shared<ast::IfStatement>(cond,statement,nullptr));
    }

    std::shared_ptr<ast::Statement> Parser::ParseForStatement(){
        Match(TOK_FOR);
        auto id = cur.text;
        Match(TOK_ID);
        Match(TOK_ASSIGNOP);
        auto from = ParseExpr();
        Match(TOK_TO);
        auto to = ParseExpr();
        Match(TOK_DO);
        auto statement = ParseStatement();
        return std::move(std::make_shared<ast::ForStatement>(from,to,statement));
    }

    std::shared_ptr<ast::Statement> Parser::ParseCompoundStatement(){
        Match(TOK_BEGIN);
        vector<std::shared_ptr<ast::Statement> > statements;
        std::shared_ptr<ast::Statement> statement;

        while(cur.token != 0 && cur.token != TOK_END){
            try {
                statement = ParseStatement();
                statements.push_back(std::move(statement));
                if(cur.token == ';')
                    NextToken();
            }catch (SyntaxErr &e){
                err_msg_.push_back(std::string(e.what()));
                while(cur.token != 0 && !isStatementStartTok(cur.token) && cur.token != ';' && cur.token != TOK_END){
                    NextToken();
                }
                if(cur.token == ';')
                    NextToken();
            }
        }
        Match(TOK_END);
        return std::move(std::make_shared<ast::CompoundStatement>(statements));
    }

    std::shared_ptr<ast::Statement> Parser::ParseAssignAndCallStatement(){
        std::string id = cur.text;
        Match(TOK_ID);  // TODO: handle id not found
        vector<std::shared_ptr<ast::Expression> > expr_list;
        auto var = std::make_shared<ast::Variable>(id);
        bool is_var = false;

        switch (cur.token) {
            case '(':
                NextToken();
                if(cur.token == ')'){
                    NextToken();
                    return std::make_shared<ast::CallStatement>(id);
                }
                expr_list = ParseExprList();
                Match(')');
                return std::make_shared<ast::CallStatement>(id,expr_list);
            case '[':
                NextToken();
                expr_list = ParseExprList();
                Match(']');
                var = std::make_shared<ast::Variable>(id,expr_list);
                is_var = true;
        }

        if(!is_var && cur.token == ';')
            return std::make_shared<ast::CallStatement>(id);

        Match(TOK_ASSIGNOP);
        auto expr = ParseExpr();
        return std::make_shared<ast::AssignStatement>(var,expr);
    }

    // TODO: add line and column information to AST
}

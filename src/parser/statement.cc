//
// Created by 谢卫凯 on 2023/3/24.
//
#include <set>

#include "parser.h"
#include "ast/statement.h"

extern "C" {
    #include "lexer.h"
}

namespace pascal2c::parser {
    template<typename Tp> using vector = ::std::vector<Tp>;

    static bool isStatementStartTok(int tok){
        static std::set<int> tokens = {TOK_IF,TOK_ID,TOK_BEGIN,TOK_FOR};
        auto it = tokens.find(tok);
        if(it != tokens.end())
            return true;
        return false;
    }

    std::shared_ptr<ast::Statement> Parser::ParseStatement(){
        static char buff[1024];
        INIT_PARSE(line_,column_);
        std::shared_ptr<ast::Statement> statement;
        switch (token_) {
            case TOK_IF:
                statement = std::move(ParseIFStatement());
                break;
            case TOK_FOR:
                statement =  std::move(ParseForStatement());
                break;
            case TOK_BEGIN:
                statement = std::move(ParseCompoundStatement());
                break;
            case TOK_ID:
                statement = std::move(ParseAssignAndCallStatement());
                break;

            default:
                if(token_ == TOK_PROCEDURE || token_ == TOK_FUNCTION || token_ == TOK_VAR || token_ == TOK_CONST)
                    sprintf(buff,"%d:%d syntax error: declaration is not part of statement",line_,column_);
                else
                    sprintf(buff,"%d:%d not expected token to parse statement",line_,column_);
                throw SyntaxErr(buff);
        }
        statement->SetLineAndColumn(begin_line,begin_column);
        return std::move(statement);
    }

    std::shared_ptr<ast::Statement> Parser::ParseIFStatement(){
        Match(TOK_IF);
        auto cond = ParseExpr();
        Match(TOK_THEN);
        auto statement = ParseStatement();
        if(token_ == TOK_ELSE){
            Match(TOK_ELSE);
            auto else_part = ParseStatement();
            return std::move(std::make_shared<ast::IfStatement>(cond,statement,else_part));
        }
        return std::move(std::make_shared<ast::IfStatement>(cond,statement,nullptr));
    }

    std::shared_ptr<ast::Statement> Parser::ParseForStatement(){
        Match(TOK_FOR);
        auto id = text_;
        Match(TOK_ID,"syntax error: missing id in for statement");
        Match(TOK_ASSIGNOP,"syntax error: missing ':=' in for statement");
        auto from = ParseExpr();
        Match(TOK_TO, "syntax error: missing 'to' in for statement");
        auto to = ParseExpr();
        Match(TOK_DO, "missing 'do' in for statement");
        auto statement = ParseStatement();
        return std::move(std::make_shared<ast::ForStatement>(id,from,to,statement));
    }

    std::shared_ptr<ast::Statement> Parser::ParseCompoundStatement() noexcept{
        INIT_PARSE(line_,column_);
        try {
            Match(TOK_BEGIN, "syntax error: missing begin when parsing compound statement");
        }catch (SyntaxErr &e){
            err_msg_.push_back(std::string(e.what()));
        }
        vector<std::shared_ptr<ast::Statement> > statements;
        std::shared_ptr<ast::Statement> statement;
        static char buff[1024];

        while(token_ != 0 && token_ != TOK_END){
            try {
                statement = ParseStatement();
                statements.push_back(std::move(statement));
                if(token_ != TOK_END) {
                    Match(';', "syntax error: missing ';' at the end of statement");
                    if(token_ == TOK_END) {
                        sprintf(buff,"%d:%d last statement should not end with ;",line_,column_);
                        throw SyntaxErr(buff);
                    }
                }
            }catch (SyntaxErr &e){
                err_msg_.push_back(std::string(e.what()));
                while(token_ != 0 && !isStatementStartTok(token_) && token_ != ';' && token_ != TOK_END){
                    NextToken();
                }
                if(token_ == ';')
                    NextToken();
            }
        }
        try {
            Match(TOK_END, "syntax error: missing end when parsing compound statement");
        }catch (SyntaxErr &e){
            err_msg_.push_back(std::string(e.what()));
        }
        return MAKE_AND_MOVE_SHARED(ast::CompoundStatement,statements);
    }

    std::shared_ptr<ast::Statement> Parser::ParseAssignAndCallStatement(){
        std::string id = text_;
        INIT_PARSE(line_, column_);
        Match(TOK_ID);
        vector<std::shared_ptr<ast::Expression> > expr_list;
        auto var = std::make_shared<ast::Variable>(id);
        bool is_var = false;

        switch (token_) {
            case '(':
                NextToken();
                if(token_ == ')'){
                    NextToken();
                    return std::make_shared<ast::CallStatement>(id);
                }
                expr_list = ParseExprList();
                Match(')',"syntax error: unclosed parentheses");
                return std::make_shared<ast::CallStatement>(id,expr_list);
            case '[':
                NextToken();
                expr_list = ParseExprList();
                Match(']',"syntax error: unclosed brackets");
                var = std::make_shared<ast::Variable>(id,expr_list);
                is_var = true;
        }

        if(!is_var && token_ == ';')
            return std::make_shared<ast::CallStatement>(id);

        Match(TOK_ASSIGNOP,"syntax error: lost ':=' when parsing assign statement");
        auto expr = ParseExpr();
        var->SetLineAndColumn(begin_line,begin_column);
        return std::make_shared<ast::AssignStatement>(var,expr);
    }
}

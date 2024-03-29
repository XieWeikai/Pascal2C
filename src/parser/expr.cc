//
// Created by 谢卫凯 on 2023/3/22.
//
#include <unordered_map>
#include "parser.h"
#include "ast/expr.h"

namespace pascal2c::parser {

    static std::unordered_map<int, int> binary_prec = {
            {'=', 1},
            {TOK_NEQOP, 1},
            {'<', 1},
            {TOK_LEOP, 1},
            {'>', 1},
            {TOK_GEOP, 1},
            {'+', 2},
            {'-', 2},
            {TOK_OR, 2},
            {TOK_AND, 3},
            {TOK_DIV, 3},
            {'*', 3},
            {'/', 3},
            {TOK_MOD, 3},
    };

    static std::unordered_map<int, int> unary_prec = {
            {TOK_NOT, 4},
            {'+', 4},
            {'-', 4},
    };

    std::shared_ptr<ast::Expression> Parser::ParsePrimary(){
        INIT_PARSE(line_, column_);
        if(prefix_parser_[token_] == nullptr) {
            throw SyntaxErr("syntax error: parse expression error: no expected token", line_, column_);
        }
        auto expr = std::move((this->*prefix_parser_[token_])());
        expr->SetLineAndColumn(begin_line,begin_column);
        return std::move(expr);
    }

    std::shared_ptr<ast::Expression> Parser::ParseParen(){
        Match('(');
        auto expr = ParseExpr();
        Match(')');
        return std::move(expr);
    }

    // a := 3 * (4 + 6  ;
    // if ....

    std::shared_ptr<ast::Expression> Parser::ParsePrefix(){
        int op = token_;
        int prec = unary_prec[op];
        NextToken();
        auto expr = ParseExpr(prec + 1);

        return std::move(std::make_shared<ast::UnaryExpr>(op,expr));
    }

    std::shared_ptr<ast::Expression> Parser::ParseNumber() {
        std::shared_ptr<ast::Expression> expr;
        switch (token_) {
            case TOK_INTEGER:
                expr = std::make_shared<ast::IntegerValue>(tok_value_.intval);
                break;
            case TOK_REAL:
                expr = std::make_shared<ast::RealValue>(tok_value_.realval);
                break;
            default:
                break;
        }
        NextToken();
        return std::move(expr);
    }

    std::shared_ptr<ast::Expression> Parser::ParseStringAndChar() {
        size_t len = strlen(tok_value_.strval);
        std::shared_ptr<ast::Expression> res;
        if(len == 1)
            res = std::move(std::make_shared<ast::CharValue>(tok_value_.strval[0]));
        else
            res = std::move(std::make_shared<ast::StringValue>(tok_value_.strval));
        NextToken();
        return std::move(res);
    }

    std::shared_ptr<ast::Expression> Parser::ParseBoolean() {
        std::shared_ptr<ast::Expression> res;
        if(token_ == TOK_TRUE)
            res = std::move(std::make_shared<ast::BooleanValue>(true));
        else
            res = std::move(std::make_shared<ast::BooleanValue>(false));
        NextToken();
        return std::move(res);
    }

    std::shared_ptr<ast::Expression> Parser::ParseVariableAndCall(){
        std::string id = text_;
        NextToken();
        vector<std::shared_ptr<ast::Expression> > expr_list;
        switch (token_) {
            case '(':
                NextToken();
                if(token_ == ')'){
                    NextToken();
                    return std::move(std::make_shared<ast::CallValue>(id));
                } else {
                    expr_list = ParseExprList();
                    Match(')');
                    return std::move(std::make_shared<ast::CallValue>(id,expr_list));
                }
            case '[':
                NextToken();
                expr_list = ParseExprList();
                Match(']');
                return std::move(std::make_shared<ast::Variable>(id,expr_list));
            default:
                return std::move(std::make_shared<ast::CallOrVar>(id));
        }
    }

    std::shared_ptr<ast::Expression> Parser::ParseExpr() {
        return ParseExpr(1);
    }

    std::shared_ptr<ast::Expression> Parser::ParseExpr(int prec) {
        auto lhs = ParsePrimary();
        int op = token_;
        int precedence = binary_prec[op];
        while(precedence >= prec){
            NextToken();
            auto rhs = ParseExpr(precedence + 1);
            int tmp_line = lhs->line(), tmp_column = lhs->column();
            lhs = std::make_shared<ast::BinaryExpr>(op,lhs,rhs);
            lhs->SetLineAndColumn(tmp_line, tmp_column);
            op = token_;
            precedence = binary_prec[op];
        }
        return std::move(lhs);
    }

    vector<std::shared_ptr<ast::Expression> > Parser::ParseExprList() {
        std::shared_ptr<ast::Expression> expr;
        vector<std::shared_ptr<ast::Expression> > res;
        while(true){
            expr = ParseExpr();
            res.push_back(std::move(expr));
            if(token_ == ',')
                NextToken();
            else
                break;
        }
        return std::move(res);
    }
}

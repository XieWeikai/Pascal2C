//
// Created by 谢卫凯 on 2023/3/22.
//

#include "parser.h"

namespace pascal2c::parser {

    static int binary_prec[500] = {
            ['=']       = 1,
            [TOK_NEQOP] = 1,
            ['<']       = 1,
            [TOK_LEOP]  = 1,
            ['>']       = 1,

            [TOK_GEOP]  = 1,
            ['+']       = 2,
            ['-']       = 2,
            [TOK_OR]    = 2,

            [TOK_AND]   = 3,
            [TOK_DIV]   = 3,
            ['*']       = 3,
            ['/']       = 3,
            [TOK_MOD]   = 3,
    };

    static int unary_prec[302] = {
            [TOK_NOT] = 4,
            ['+']     = 4,
            ['-']     = 4,
    };

    std::shared_ptr<ast::Expression> Parser::ParsePrimary(){
        char buff[1024];
        if(prefix_parser_[token_] == nullptr) {
            sprintf(buff,"%d:%d: parse expression error: no expected token",line_,column_);
            throw SyntaxErr(buff);
        }
        return std::move((this->*prefix_parser_[token_])());
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

    std::shared_ptr<ast::Expression> Parser::ParseVariableAndCall(){
        std::string id = text_;
        int ch;
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
                return std::move(std::make_shared<ast::Variable>(id));
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
            lhs = std::make_shared<ast::BinaryExpr>(op,lhs,rhs);
            op = token_;
            precedence = binary_prec[op];
        }
        return lhs;
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

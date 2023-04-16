//
// Created by 谢卫凯 on 2023/3/22.
//
#include <unordered_map>
#include "syntax.h"
#include "ast/expr.h"

namespace Pascal2C::Syntax 
{

static std::unordered_map<int, int> 
binary_prec = 
{
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

static std::unordered_map<int, int> 
unary_prec = 
{
    {TOK_NOT, 4},
    {'+', 4},
    {'-', 4},
};

std::shared_ptr<ast::Expression> Parser::ParsePrimary(){
    char buff[1024];
    if(prefix_parser_[cur.token] == nullptr) {
        sprintf(buff,"%d:%d: parse expression error: no expected token",cur.line,cur.column);
        throw SyntaxErr(buff);
    }
    return std::move((this->*prefix_parser_[cur.token])());
}

std::shared_ptr<ast::Expression> Parser::ParseParen()
{
    Match('(');
    auto expr = ParseExpr();
    Match(')');
    return std::move(expr);
}

// a := 3 * (4 + 6  ;
// if ....

std::shared_ptr<ast::Expression> Parser::ParsePrefix(){
    int op = cur.token;
    int prec = unary_prec[op];
    NextToken();
    auto expr = ParseExpr(prec + 1);

    return std::move(std::make_shared<ast::UnaryExpr>(op,expr));
}

std::shared_ptr<ast::Expression> Parser::ParseNumber() {
    std::shared_ptr<ast::Expression> expr;
    switch (cur.token) {
        case TOK_INTEGER:
            expr = std::make_shared<ast::IntegerValue>(cur.tok_value.intval);
            break;
        case TOK_REAL:
            expr = std::make_shared<ast::RealValue>(cur.tok_value.realval);
            break;
        default:
            break;
    }
    NextToken();
    return std::move(expr);
}

std::shared_ptr<ast::Expression> Parser::ParseVariableAndCall(){
    std::string id = cur.text;
    int ch;
    NextToken();
    vector<std::shared_ptr<ast::Expression> > expr_list;
    switch (cur.token) {
        case '(':
            NextToken();
            if(cur.token == ')'){
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
    int op = cur.token;
    int precedence = binary_prec[op];
    while(precedence >= prec){
        NextToken();
        auto rhs = ParseExpr(precedence + 1);
        lhs = std::make_shared<ast::BinaryExpr>(op,lhs,rhs);
        op = cur.token;
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
        if(cur.token == ',')
            NextToken();
        else
            break;
    }
    return std::move(res);
}

// TODO:
//      parse boolean value, string value , char value
// TODO:
//      add a new class CallOrVariable to represent call or variable, this can be the base type of CallValue and Variable
//      we use CallOrVariable in case that we can't distinguish call and variable in the grammar, such as a := add;
//      we don't know whether add is a variable or a function call, so we use CallOrVariable to represent it
// TODO:
//     add line and column information to the ast node
}

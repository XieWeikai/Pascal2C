//
// Created by 谢卫凯 on 2023/3/24.
//

#include "parser.h"
#include "ast/statement.h"

extern "C" {
    #include "lexer/lexer.h"
}

namespace pascal2c::parser {
    std::shared_ptr<ast::Statement> Parser::ParseStatement(){
        switch (token_) {
            case TOK_IF:
                return std::move(ParseIFStatement());
            case TOK_FOR:
                return std::move(ParseCompoundStatement());
            case TOK_BEGIN:
                return std::move(ParseCompoundStatement());

        }
    }

    std::shared_ptr<ast::Statement> Parser::ParseIFStatement(){

    }

    std::shared_ptr<ast::Statement> Parser::ParseForStatement(){

    }

    std::shared_ptr<ast::Statement> Parser::ParseCompoundStatement(){

    }
}

#include <algorithm>
#include <string>
#include <memory>
#include <vector>
#include <cstdio>
#include <exception>

#include "parser.h"

extern "C" {
    #include "lexer/lexer.h"
}

namespace pascal2c::parser {

    Parser::Parser(FILE *in) {
        for(auto & i : prefix_parser_)
            i = nullptr;

        prefix_parser_[TOK_ID] = &Parser::ParseVariableAndCall;
        prefix_parser_[TOK_INTEGER] = &Parser::ParseNumber;
        prefix_parser_[TOK_REAL] = &Parser::ParseNumber;

        prefix_parser_['+'] = &Parser::ParsePrefix;
        prefix_parser_['-'] = &Parser::ParsePrefix;
        prefix_parser_[TOK_NOT] = &Parser::ParsePrefix;

        prefix_parser_['('] = &Parser::ParseParen;

        SetInput(in);
        next_token_ = yylex();
        next_tok_value_ = yylval;
        next_line_ = yylineno;
        next_column_ = yycolno;
        next_text_ = yytext;
        NextToken();
    }

    int Parser::NextToken() {
        token_ = next_token_;
        tok_value_ = next_tok_value_;
        line_ = next_line_;
        column_ = next_column_;
        text_ = next_text_;

        next_token_ = yylex();
        next_tok_value_ = yylval;
        next_line_ = yylineno;
        next_column_ = yycolno;
        next_text_ = yytext;
        return token_;
    }

    std::string Parser::GetLexerErrMsg(int err) {
        return {YYERRMSG[yyerrno]};
    }

    void Parser::Match(int token) {
        int tok = token_;
        int line = line_;
        int column = column_;
        char buff[1024];
        NextToken();
        if(tok != token) {
            sprintf(buff,"%d:%d syntax err:expected %c got %c",line,column,token,tok);
            throw SyntaxErr(std::string(buff));
        }
    }

}

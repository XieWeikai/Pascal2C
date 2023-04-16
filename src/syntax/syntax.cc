#include <algorithm>
#include <string>
#include <memory>
#include <vector>
#include <cstdio>
#include <exception>

#include "syntax.h"
#include "lexer.h"


namespace Pascal2C::Syntax 
{

    Parser::Parser(FILE *in) {
        // for(auto & i : prefix_parser_)
        //     i = nullptr;
        // shared_ptr is initialized with nullptr by default

        prefix_parser_[TOK_ID]      = std::make_shared<call_T>(ParseVariableAndCall);
        prefix_parser_[TOK_INTEGER] = std::make_shared<call_T>(ParseNumber);
        prefix_parser_[TOK_REAL]    = std::make_shared<call_T>(ParseNumber);

        prefix_parser_[TOK_NOT]     = std::make_shared<call_T>(ParsePrefix);
        prefix_parser_['+']         = std::make_shared<call_T>(ParsePrefix);
        prefix_parser_['-']         = std::make_shared<call_T>(ParsePrefix);
        
        prefix_parser_['(']         = std::make_shared<call_T>(ParseParen);

        SetInput(in);

        next_token_     = yylex();
        next_tok_value_ = yylval;
        next_line_      = yylineno;
        next_column_    = yycolno;
        next_text_      = yytext;

        NextToken();
    }

    int Parser::NextToken() {
        token_ = next_token_;
        tok_value_ = next_tok_value_;
        line_ = next_line_;
        column_ = next_column_;
        text_ = next_text_;
        lexer_errno_ = yyerrno;

        next_token_ = yylex();
        next_tok_value_ = yylval;
        next_line_ = yylineno;
        next_column_ = yycolno;
        next_text_ = yytext;
        return token_;
    }

    std::string Parser::GetLexerErrMsg() {
        return {YYERRMSG[lexer_errno_]};
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

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

    Parser::Parser(std::string_view file_name_ ,FILE *in) noexcept
        : file_name(file_name_) 
        {
        // for(auto & i : prefix_parser_)
        //     i = nullptr;
        // shared_ptr is initialized with nullptr by default

        prefix_parser_[TOK_ID]      = &Parser::ParseVariableAndCall;
        prefix_parser_[TOK_INTEGER] = &Parser::ParseNumber;
        prefix_parser_[TOK_REAL]    = &Parser::ParseNumber;

        prefix_parser_[TOK_NOT]     = &Parser::ParsePrefix;
        prefix_parser_['+']         = &Parser::ParsePrefix;
        prefix_parser_['-']         = &Parser::ParsePrefix;
        
        prefix_parser_['(']         = &Parser::ParseParen;

        SetInput(in);

        next = Cursor {
            .token      {yylex() } ,
            .tok_value  {yylval  } ,
            .line       {yylineno} ,
            .column     {yycolno } ,
            .text       {yytext  }
        };

        NextToken();
    }

    void Parser::NextToken() noexcept {
        cur = next;
        lexer_errno_    = yyerrno;

        next = Cursor {
            .token      {yylex() } ,
            .tok_value  {yylval  } ,
            .line       {yylineno} ,
            .column     {yycolno } ,
            .text       {yytext  }
        };
    }

    std::string Parser::GetLexerErrMsg() {
        return YYERRMSG[lexer_errno_];
    }

    void Parser::Match(i32 token) {
        static char buff[1024];
        NextToken();

        if(cur.token != token) {
            sprintf(buff ,"%d:%d syntax ERR:expected %c got %c",
                    cur.line, cur.column, token, cur.token);
            throw SyntaxErr{std::string{buff}};
        }
    }

}

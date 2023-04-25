#include <algorithm>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <cstdio>
#include <exception>

#include "parser.h"

extern "C" {
    #include "lexer.h"
}

namespace pascal2c::parser
{

    Parser::Parser(FILE *in)
    {
        for (auto &i : prefix_parser_)
            i = nullptr;

        prefix_parser_[TOK_ID] = &Parser::ParseVariableAndCall;
        prefix_parser_[TOK_INTEGER] = &Parser::ParseNumber;
        prefix_parser_[TOK_REAL] = &Parser::ParseNumber;

        prefix_parser_['+'] = &Parser::ParsePrefix;
        prefix_parser_['-'] = &Parser::ParsePrefix;
        prefix_parser_[TOK_NOT] = &Parser::ParsePrefix;

        prefix_parser_['('] = &Parser::ParseParen;
        prefix_parser_[TOK_STRING] = &Parser::ParseStringAndChar;
        prefix_parser_[TOK_TRUE] = &Parser::ParseBoolean;
        prefix_parser_[TOK_FALSE] = &Parser::ParseBoolean;

        yyreset(in);  // reset input file

        next_token_ = yylex();
        next_tok_value_ = yylval;
        next_line_ = yylineno;
        next_column_ = yycolno;
        next_text_ = yytext;

        NextToken();
    }

    int Parser::NextToken()
    {
        static char buff[MAX_STR_LEN];

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

        if(token_ == TOK_ERROR) {
            sprintf(buff,"%d:%d lexical error: %s",line_,column_, GetLexerErrMsg().c_str());
            throw SyntaxErr(buff);
        }

        return token_;
    }

    std::string Parser::GetLexerErrMsg()
    {
        return {YYERRMSG[lexer_errno_]};
    }

    void Parser::Match(int token)
    {
        int tok = token_;
        int line = line_;
        int column = column_;
        if (tok != token)
        {
            std::ostringstream err;
            err << line << ":" << column << " syntax err:expected " << TokenToString(token) << " got " << TokenToString(tok);
            throw SyntaxErr(err.str());
        }
        NextToken(); // only skip current token if it matches
    }

    void Parser::Match(int token, const std::string& err_msg) {
        int tok = token_;
        int line = line_;
        int column = column_;
        static char buff[1024];
        if(tok != token) {
            sprintf(buff,"%d:%d %s",line,column, err_msg.c_str());
            throw SyntaxErr(std::string(buff));
        }
        NextToken(); // only skip current token if it matches
    }

}

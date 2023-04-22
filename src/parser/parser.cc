#include <algorithm>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <cstdio>
#include <exception>

#include "parser.h"

<<<<<<< HEAD
extern "C"
{
#include "lexer.h"
=======
extern "C" {
    #include "lexer.h"
    extern int yycolno_next;
>>>>>>> dc17493533518273dcf013908cdc7eacdfe38545
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

        yyreset(in);  // reset input file
        yycolno_next = 1;  // reset token position marker

        next_token_ = yylex();
        next_tok_value_ = yylval;
        next_line_ = yylineno;
        next_column_ = yycolno;
        next_text_ = yytext;

        NextToken();
    }

    int Parser::NextToken()
    {
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

    std::string Parser::GetLexerErrMsg()
    {
        return {YYERRMSG[lexer_errno_]};
    }

    void Parser::Match(int token)
    {
        int tok = token_;
        int line = line_;
        int column = column_;
<<<<<<< HEAD
        NextToken();
        if (tok != token)
        {
            std::ostringstream err;
            err << line << ":" << column << " syntax err:expected " << token << " got " << tok;
            throw SyntaxErr(err.str());
=======
        char buff[1024];
        if(tok != token) {
            sprintf(buff,"%d:%d syntax err:expected %s got %s",line,column, TokenToString(token), TokenToString(tok));
            throw SyntaxErr(std::string(buff));
>>>>>>> dc17493533518273dcf013908cdc7eacdfe38545
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

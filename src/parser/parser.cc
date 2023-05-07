#include <algorithm>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <set>
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
            throw SyntaxErr(GetLexerErrMsg(), line_, column_);
        }

        return token_;
    }

    std::string Parser::GetLexerErrMsg() const
    {
        return {YYERRMSG[lexer_errno_]};
    }

    void Parser::Match(int token)
    {
        if (token_ != token)
        {
            std::ostringstream err;
            err << "syntax err:expected " << TokenToString(token) << " before " << TokenToString(token_);
            throw SyntaxErr( err.str(),line_,column_);
        }
        NextToken(); // only skip current token if it matches
    }

    int Parser::Match(const std::set<int> &tokens, const std::string &expected_token)
    {
        int tok = token_;
        if (tokens.find(token_) == tokens.end())
        {
            std::ostringstream err;
            err << "syntax err:expected " << expected_token << " before " << TokenToString(token_);
            throw SyntaxErr(err.str(), line_, column_);
        }
        NextToken(); // only skip current token if it matches
        return tok;
    }

    void Parser::Match(int token, const std::string &err_msg)
    {
        if (token_ != token)
        {
            throw SyntaxErr(err_msg, line_, column_);
        }
        NextToken(); // only skip current token if it matches
    }

    int Parser::CheckMatch(const int token, const std::set<int> &delimiters)
    {
        try
        {
            Match(token);
            return token;
        }
        catch (SyntaxErr &err)
        {
            AddSyntaxErr(err);
            while (token_ != token && delimiters.find(token_) == delimiters.end() && token_ != TOK_EOF)
            {
                NextToken();
            }
            if (token_ == token)
            {
                NextToken(); // skip to the next token when the expected token is met
                return token;
            }
            return token_;
        }
    }

    int Parser::CheckMatch(const std::set<int> &tokens, const std::string &expected_token, const std::set<int> &delimiters)
    {
        try
        {
            int ret = Match(tokens, expected_token);
            return ret;
        }
        catch (SyntaxErr &err)
        {
            AddSyntaxErr(err);
            while (tokens.find(token_) == tokens.end() && delimiters.find(token_) == delimiters.end() && token_ != TOK_EOF)
            {
                NextToken();
            }
            if (tokens.find(token_) != tokens.end())
            {
                int tok = token_;
                NextToken(); // skip to the next token when the expected token is met
                return tok;
            }
            return token_;
        }
    }

    SyntaxErr::SyntaxErr(const std::string& err_msg) {
        std::stringstream ss(err_msg);
        int line, col;
        char colon;
        std::string msg;
        ss >> line >> colon >> col ;
        std::getline(ss >> std::ws, msg);
        err_msg_ = std::move(msg);
        line_ = line;
        col_ = col;
    }

    void Parser::AddSyntaxErr(SyntaxErr &err) {
        syntax_errs_.push_back(err);
        err_msg_.push_back(err.what());
    }
}

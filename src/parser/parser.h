#ifndef PASCAL2C_SRC_PARSER_PARSER_H_
#define PASCAL2C_SRC_PARSER_PARSER_H_

#include <memory>
#include <utility>
#include <vector>
#include <cstdio>
#include <string>

#include "gtest/gtest.h"
#include "ast/ast.h"

extern "C" {
    #include "lexer/lexer.h"
};

namespace pascal2c::parser {
    template<typename Tp> using vector = ::std::vector<Tp>;

    class SyntaxErr : public ::std::exception {
    public:
        explicit SyntaxErr(std::string err_msg) : err_msg_(std::move(err_msg)) {}
        inline const char* what() const noexcept override {
            return err_msg_.c_str();
        }

    private:
        std::string err_msg_;
    };

    class Parser{
    public:
        explicit Parser(FILE *in);
    private:
        FRIEND_TEST(TokenTest,TestNextToken);
        FRIEND_TEST(ExprParserTest,TestParsePrimary);
        FRIEND_TEST(ExprParserTest,TestParseExpr);

        int token_,next_token_;
        int lexer_errno_;
        YYSTYPE tok_value_,next_tok_value_;
        int line_,next_line_;                        // line number of token in the input file
        int column_,next_column_;                      // column number of token in the input file
        std::string text_,next_text_;

        std::shared_ptr<ast::Expression> (Parser::*prefix_parser_[302])();

        vector<std::string> err_msg_;     // error massages

        int NextToken();

        void Match(int token);

        std::string GetLexerErrMsg();

        std::shared_ptr<ast::Expression> ParseExpr();

        std::shared_ptr<ast::Expression> ParseExpr(int prec);

        vector<std::shared_ptr<ast::Expression> > ParseExprList();

        std::shared_ptr<ast::Expression> ParsePrimary();

        std::shared_ptr<ast::Expression> ParseParen();

        std::shared_ptr<ast::Expression> ParsePrefix();

        std::shared_ptr<ast::Expression> ParseNumber();

        std::shared_ptr<ast::Expression> ParseVariableAndCall();

        std::shared_ptr<ast::Statement> ParseStatement();

        std::shared_ptr<ast::Statement> ParseIFStatement();

        std::shared_ptr<ast::Statement> ParseForStatement();

        std::shared_ptr<ast::Statement> ParseCompoundStatement();

        std::shared_ptr<ast::Statement> ParseAssignAndCallStatement();
    };
}


#endif // !PASCAL2C_SRC_PARSER_PARSER_H_

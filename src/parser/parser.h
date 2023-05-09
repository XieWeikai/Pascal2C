#ifndef PASCAL2C_SRC_PARSER_PARSER_H_
#define PASCAL2C_SRC_PARSER_PARSER_H_

#include <memory>
#include <utility>
#include <vector>
#include <cstdio>
#include <string>
#include <sstream>

#include "gtest/gtest.h"
#include "ast/ast.h"
#include "ast/expr.h"
#include "ast/program.h"

extern "C"
{
#include "lexer.h"
}

#define TOK_EOF 0

// init the begin line and the begin column before parsing
#define INIT_PARSE(line, column) \
    int begin_line = line;       \
    int begin_column = column;

// make the shared pointer of Ast node
#define MAKE_SHARED(constructor, ...) std::make_shared<constructor>(begin_line, begin_column, __VA_ARGS__)

// make the shared pointer of Ast node with no argument
#define MAKE_SHARED_WITH_NO_ARGUMENT(constructor) std::make_shared<constructor>(begin_line, begin_column)

// make and move the shared pointer of Ast node
#define MAKE_AND_MOVE_SHARED(constructor, ...) \
    std::move(MAKE_SHARED(constructor, __VA_ARGS__))

namespace pascal2c::parser
{
    template <typename Tp>
    using vector = ::std::vector<Tp>;

    // exception class for syntax error
    class SyntaxErr : public ::std::exception
    {
    public:
        // param:
        //     err_msg is the error message
        explicit SyntaxErr(const std::string& err_msg);
        SyntaxErr(std::string err_msg, int line, int col) : err_msg_(std::move(err_msg)), line_(line), col_(col) {}

        // get the error message
        // return:
        //     the error message
        inline const char *what() const noexcept override
        {
            static char buff[1024];
            sprintf(buff, "%d:%d %s", line_, col_, err_msg_.c_str());
            return buff;
        }

        GETTER(std::string, err_msg);
        GETTER(int, line);
        GETTER(int, col);
    private:
        std::string err_msg_; // error message
        int line_, col_;      // line and column number
    };

    // parser class
    // parse the pascal program
    // usage:
    //     Parser parser = new Parser(file_ptr);
    //     std::shared_ptr<ast::Program> program = std::move(parser.Parse());
    class Parser
    {
    public:
        // param:
        //     in is the input file
        explicit Parser(FILE *in);

        GETTER(vector<std::string>, err_msg);
        GETTER(vector<SyntaxErr>, syntax_errs);

        // parse the whole program
        // return:
        //     the ast of the program
        inline std::shared_ptr<ast::Program> Parse()
        {
            return ParseProgram();
        }

    private:
        FRIEND_TEST(TokenTest, TestNextToken);
        FRIEND_TEST(TotalParserTest, TestParse);
        FRIEND_TEST(ProgramParserTest, TestParseProgram);
        FRIEND_TEST(ProgramParserTest, TestParseProgramHead);
        FRIEND_TEST(ProgramParserTest, TestParseProgramBody);
        FRIEND_TEST(ProgramParserTest, TestParseConstDecl);
        FRIEND_TEST(ProgramParserTest, TestParseVarDecl);
        FRIEND_TEST(ProgramParserTest, TestParseSubProgramDecl);
        FRIEND_TEST(ProgramParserTest, TestParseSubProgramHead);
        FRIEND_TEST(ProgramParserTest, TestParseSubProgramBody);
        FRIEND_TEST(ProgramParserTest, TestParseIdList);
        FRIEND_TEST(ProgramParserTest, TestParseType);
        FRIEND_TEST(ProgramParserTest, TestParsePeriod);
        FRIEND_TEST(ProgramParserTest, TestParseParameter);
        FRIEND_TEST(ExprParserTest, TestParsePrimary);
        FRIEND_TEST(ExprParserTest, TestParseExpr);
        FRIEND_TEST(StatementParserTest, TestAssignStatement);
        FRIEND_TEST(StatementParserTest, TestIfStatement);
        FRIEND_TEST(StatementParserTest, TestForStatement);
        FRIEND_TEST(StatementParserTest, TestCompoundStatement);
        FRIEND_TEST(ErrorHandleTest, TestErrorHandle);
        FRIEND_TEST(ExprParserTest, TestParserErr);

        int token_, next_token_; // current token and next token

        // if a lex error occur, error number will be stored in this variable
        int lexer_errno_;

        // token value
        YYSTYPE tok_value_, next_tok_value_;
        int line_, next_line_;     // line number of token in the input file
        int column_, next_column_; // column number of token in the input file
        std::string text_, next_text_;

        vector<std::string> err_msg_; // error massages
        vector<SyntaxErr>   syntax_errs_; // syntax error

        std::shared_ptr<ast::Expression> (Parser::*prefix_parser_[500])();

        void AddSyntaxErr(SyntaxErr &err);

        // get next token
        // return:
        //     the next token
        int NextToken();

        // match token and get next token (only skip current token if it matches)
        // param:
        //     token is the token to match
        // throw:
        //     SyntaxErr if token not match
        void Match(int token);

        // match token and get next token (only skip current token if it matches)
        // param:
        //     token is the token to match
        //     err_msg is the error message that is going to be thrown if token not match
        // throw:
        //     SyntaxErr if token not match
        void Match(int token,const std::string& err_msg);

        // match tokens and get next token (only skip current token if it matches)
        // param:
        //     tokens are the tokens to match
        //     expected_token is the primary expected token in the err_msg
        // return:
        //     the matched token or the unexpected token
        // throw:
        //     SyntaxErr if token not match
        int Match(const std::set<int> &tokens, const std::string &expected_token);

        // Check if the token is matched,
        // if not, get next token until either the expected token or the delimiters or the end of the file is found
        // Note:
        //     the current token will skip to the next token of the expected token if the expected token is found
        //     otherwise, the current token will jump to the position of the delimiters or the end of the file
        // param:
        //     token is the token to match
        //     delimiters is the end symbols to stop finding the expected token, that is the expected token of the next part
        // return:
        //     the matched token or the delimiters or end of the file
        int CheckMatch(const int token, const std::set<int> &delimiters);

        // Check if the token is matched,
        // if not, get next token until either the expected tokens or the delimiters or the end of the file is found
        // Note:
        //     the current token will skip to the next token of the expected token only if the expected token is found
        //     otherwise, the current token will jump to the position of the delimiters or the end of the file
        // param:
        //     tokens are the tokens to match
        //     expected_token is the primary expected token in the err_msg
        //     delimiters is the end symbols to stop finding the expected token, that is the expected token of the next part
        // return:
        //     the matched token or the delimiters or end of the file
        int CheckMatch(const std::set<int> &tokens, const std::string &expected_token, const std::set<int> &delimiters);

        // get the error message of lexer
        // return:
        //     the error message
        std::string GetLexerErrMsg() const;

        // parse the whole program
        // e.g. program p; const a = 1; var b : integer; function f(a : integer) : integer; begin end; begin end.
        // return:
        //     the ast of the program
        std::shared_ptr<ast::Program> ParseProgram();

        // parse the program head
        // eg. program p(a, b, c);
        // return:
        //     the ast of the program head
        std::shared_ptr<ast::ProgramHead> ParseProgramHead();

        // parse the program body
        // e.g. const a = 1; var b : integer; function f(a : integer) : integer; begin end; begin end
        // return:
        //     the ast of the program body
        std::shared_ptr<ast::ProgramBody> ParseProgramBody();

        // parse the const declaration
        // eg. const a = 1;
        // return:
        //     the ast of the const declaration
        std::shared_ptr<ast::ConstDeclaration> ParseConstDeclaration();

        // parse the var declaration
        // eg. var a : integer;
        // return:
        //     the ast of the var declaration
        std::shared_ptr<ast::VarDeclaration> ParseVarDeclaration();

        // parse the subprogram declaration
        // eg. function f(a : integer) : integer; begin end;
        // eg. procedure p(a : integer); begin end;
        // return:
        //     the ast of the subprogram declaration
        std::shared_ptr<ast::Subprogram> ParseSubprogram();

        // parse the subprogram head
        // eg. function f(a : integer) : integer;
        // eg. procedure p(a : integer);
        // return:
        //     the ast of the subprogram head
        std::shared_ptr<ast::SubprogramHead> ParseSubprogramHead();

        // parse the subprogram body
        // eg. begin end
        // return:
        //     the ast of the subprogram body
        std::shared_ptr<ast::SubprogramBody> ParseSubprogramBody();

        // parse the id list
        // eg. a, b, c
        // return:
        //     the ast of the id list
        std::shared_ptr<ast::IdList> ParseIdList();

        // parse the type
        // eg. integer
        // eg. array[1..10] of integer
        // return:
        //     the ast of the type
        std::shared_ptr<ast::Type> ParseType();

        // parse the period
        // eg. 1..10
        // return:
        //     the ast of the period
        ast::Type::Period ParsePeriod();

        // parse the parameter
        // eg. a : integer
        // eg. var a : integer
        // return:
        //     the ast of the parameter
        std::shared_ptr<ast::Parameter> ParseParameter();

        // parse the expression
        // eg. a + b
        // return:
        //     the ast of the expression
        std::shared_ptr<ast::Expression> ParseExpr();

        // parse the expression
        // param:
        //     prec is the precedence of the operator
        // return:
        //     the ast of the expression
        std::shared_ptr<ast::Expression> ParseExpr(int prec);

        // parse the expression list
        // return:
        //     the ast of the expression list
        vector<std::shared_ptr<ast::Expression>> ParseExprList();

        // parse the primary unit of expression
        // return:
        //     the ast of the primary unit of expression
        std::shared_ptr<ast::Expression> ParsePrimary();

        // parse the parenthesis
        // return:
        //     the ast of the parenthesis
        std::shared_ptr<ast::Expression> ParseParen();

        // parse the prefix
        // return:
        //     the ast of the prefix
        std::shared_ptr<ast::Expression> ParsePrefix();

        // parse integer or real number
        // return:
        //     the integer or real number ast node
        std::shared_ptr<ast::Expression> ParseNumber();

        // parse the variable and call ast node such as a[10] or f(a, b)
        // return:
        //  if meet the variable, return the Variable ast node
        //  if meet the call, return the Call ast node
        //  if we can't determine whether it is variable or call, return CallOrVar ast node
        std::shared_ptr<ast::Expression> ParseVariableAndCall();

        // parse the string and char when meet something like 'abc' or 'a'
        // return:
        //   String ast node if meet 'aaf'
        //   Char ast node if meet 'a'
        std::shared_ptr<ast::Expression> ParseStringAndChar();

        // parse the boolean value
        // return:
        //   Boolean ast node
        std::shared_ptr<ast::Expression> ParseBoolean();

        // parse the statement
        // include: if statement, for statement, compound statement, assign and call statement
        // return:
        //     the ast of the statement
        std::shared_ptr<ast::Statement> ParseStatement();

        // parse the if statement
        // return:
        //     the ast of the if statement
        std::shared_ptr<ast::Statement> ParseIFStatement();

        // parse the for statement
        // return:
        //     the ast of the for statement
        std::shared_ptr<ast::Statement> ParseForStatement();

        // parse the compound statement
        // this function will not throw exception
        // all the exception occur when parsing statement
        // will be handled in this function
        // return:
        //     the ast of the compound statement
        std::shared_ptr<ast::Statement> ParseCompoundStatement() noexcept;

        // parse the assign and call statement
        // return:
        //     the ast of the assign statement if meet the assign statement
        //     the ast of the call statement if meet the call statement
        std::shared_ptr<ast::Statement> ParseAssignAndCallStatement();
    };
}

#endif // !PASCAL2C_SRC_PARSER_PARSER_H_

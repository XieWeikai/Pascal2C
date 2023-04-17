#ifndef PASCAL2C_SRC_PARSER_PARSER_H_
#define PASCAL2C_SRC_PARSER_PARSER_H_

#include <memory>
#include <utility>
#include <vector>
#include <cstdio>
#include <string>
#include <functional>
#include "common/common_type.h"

#include "gtest/gtest.h"
#include "ast/ast.h"
#include "ast/expr.h"
#include "ast/program.h"

extern "C" {
    #include "lexer.h"
}

namespace Pascal2C::Syntax
{

template <typename Tp>
using vector = std::vector<Tp>;

struct Cursor {
    i32 token;          // current token and next token
    YYSTYPE tok_value;  // token value offered by flex
    i32 line;           // line   number of token in the input file
    i32 column;         // column number of token in the input file
    std::string text;
};

#define SYN_ERROR(str , ...) \
    ERROR(file_name.c_str() ,cur.line ,"Col:%d " str ,cur.column , ##__VA_ARGS__)

// exception class for syntax error
class SyntaxErr : public std::exception
{
public:
    // param:
    //     err_msg is the error message
    explicit SyntaxErr(std::string err_msg) : err_msg_(std::move(err_msg)) {}

    // get the error message
    // return:
    //     the error message
    inline const char *what() const noexcept override
        { return err_msg_.c_str(); }

private:
    std::string err_msg_; // error message
};

class Parser
{
public:
    /**
     * @brief 创建新的语法分析器
     * @param in C风格的文件指针
    */
    explicit Parser(std::string_view file_name_ ,FILE *in) noexcept;

private:
    FRIEND_TEST(TokenTest, TestNextToken);
    FRIEND_TEST(ExprParserTest, TestParsePrimary);
    FRIEND_TEST(ExprParserTest, TestParseExpr);
    FRIEND_TEST(StatementParserTest, TestAssignStatement);
    FRIEND_TEST(StatementParserTest, TestIfStatement);
    FRIEND_TEST(StatementParserTest, TestForStatement);
    FRIEND_TEST(StatementParserTest, TestCompoundStatement);


    // if a lex error occur, error number will be stored in this variable
    int lexer_errno_;

    // token value
    Cursor cur , next;

    // Source File name
    std::string file_name;

    // error massages and its buffer
    char buff[1024];              
    vector<std::string> err_msg_;

    // Mapping from token to handler
    // using call_T = std::function<std::shared_ptr<ast::Expression>()>;
    // std::shared_ptr<call_T> prefix_parser_[512];
    std::shared_ptr<ast::Expression> (Parser::*prefix_parser_[512])();

    /**
     * @brief Get next token
    */
    void NextToken() noexcept;

    /**
     * @brief match token and get next token
     * @param token is the token to match
     * @throw SyntaxErr
    */
    void Match(i32 token) ;

    // get the error message of lexer
    // return:
    //     the error message
    std::string GetLexerErrMsg();

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
    // e.g. const a = 1; var b : integer; function f(a : integer) : integer; begin end; begin end.
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

    /** parse the subprogram declaration
    // eg. function f(a : integer) : integer; begin end;
    // eg. procedure p(a : integer); begin end;
    // @returns :
    //     the ast of the subprogram declaration
    */
    std::shared_ptr<ast::Subprogram> ParseSubprogram();

    // parse the subprogram head
    // eg. function f(a : integer) : integer;
    // eg. procedure p(a : integer);
    // return:
    //     the ast of the subprogram head
    std::shared_ptr<ast::SubprogramHead> ParseSubprogramHead();

    // parse the subprogram body
    // eg. begin end;
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

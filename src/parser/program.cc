#include "program.h"

extern "C"
{
#include "lexer/lexer.h"
}

namespace pascal2c::ast
{
    template <typename Tp>
    using vector = ::std::vector<Tp>;

    std::shared_ptr<ast::Program> Parser::ParseProgram()
    {
        auto program_head = ParseProgramHead();
        Match(TOK_SEMICOLON);
        auto program_body = ParseProgramBody();
        Match(TOK_DOT);
        return std::move(std::make_shared<ast::Program>(std::move(program_head), std::move(program_body)));
    }

    std::shared_ptr<ast::ProgramHead> Parser::ParseProgramHead()
    {
        Match(TOK_PROGRAM);
        auto name = text_;
        Match(TOK_ID);
        if (token_ == TOK_LPAREN)
        {
            Match(TOK_LPAREN);
            auto id_list = ParseIdList();
            Match(TOK_RPAREN);
            return std::move(std::make_shared<ast::ProgramHead>(name, std::move(id_list)));
        }
        return std::move(std::make_shared<ast::ProgramHead>(name));
    }

    std::shared_ptr<ast::ProgramBody> Parser::ParseProgramBody()
    {
        auto program_body = std::make_shared<ast::ProgramBody>();

        // Parse const declarations
        if (token_ = TOK_CONST)
        {
            Match(TOK_CONST);
            while (token_ != TOK_VAR && token_ != TOK_PROCEDURE && token_ != TOK_FUNCTION && token_ != TOK_BEGIN)
            {
                auto const_decl = ParseConstDeclaration();
                Match(TOK_SEMICOLON);
                program_body->AddConstDeclaration(std::move(const_decl));
            }
        }

        // Parse var declarations
        if (token_ = TOK_VAR)
        {
            Match(TOK_VAR);
            while (token_ != TOK_PROCEDURE && token_ != TOK_FUNCTION && token_ != TOK_BEGIN)
            {
                auto var_decl = ParseVarDeclaration();
                Match(TOK_SEMICOLON);
                program_body->AddVarDeclaration(std::move(var_decl));
            }
        }

        // Parse subprograms
        if (token_ = TOK_PROCEDURE || token_ = TOK_FUNCTION)
        {
            while (token_ != TOK_BEGIN)
            {
                auto subprogram = ParseSubprogram();
                Match(TOK_SEMICOLON);
                program_body->AddSubprogram(std::move(subprogram));
            }
        }

        // Parse compound statement
        Match(TOK_BEGIN);
        while (token_ != TOK_END)
        {
            auto statement = ParseStatement();
            Match(TOK_SEMICOLON);
            program_body->AddStatement(std::move(statement));
        }
        Match(TOK_END);
        return std::move(program_body);
    }

    std::shared_ptr<ast::ConstDeclaration> Parser::ParseConstDeclaration()
    {
        auto name = text_;
        Match(TOK_ID);
        Match(TOK_ASSIGNOP);
    }
}

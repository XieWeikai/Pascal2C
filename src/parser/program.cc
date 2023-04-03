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
            NextToken();
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
            NextToken();
            while (token_ != TOK_VAR && token_ != TOK_PROCEDURE && token_ != TOK_FUNCTION && token_ != TOK_BEGIN)
            {
                program_body->AddConstDeclaration(std::move(ParseConstDeclaration()));
                Match(TOK_SEMICOLON);
            }
        }

        // Parse var declarations
        if (token_ = TOK_VAR)
        {
            NextToken();
            while (token_ != TOK_PROCEDURE && token_ != TOK_FUNCTION && token_ != TOK_BEGIN)
            {
                program_body->AddVarDeclaration(std::move(ParseVarDeclaration()));
                Match(TOK_SEMICOLON);
            }
        }

        // Parse subprograms
        if (token_ = TOK_PROCEDURE || token_ = TOK_FUNCTION)
        {
            while (token_ != TOK_BEGIN)
            {
                program_body->AddSubprogram(std::move(ParseSubprogram()));
                Match(TOK_SEMICOLON);
            }
        }

        // Parse compound statement
        Match(TOK_BEGIN);
        while (token_ != TOK_END)
        {
            program_body->AddStatement(std::move(ParseStatement()));
            Match(TOK_SEMICOLON);
        }
        Match(TOK_END);
        return std::move(program_body);
    }

    std::shared_ptr<ast::ConstDeclaration> Parser::ParseConstDeclaration()
    {
        auto name = text_;
        Match(TOK_ID);
        Match(TOK_ASSIGNOP);
        // TODO: Parse const value
    }

    std::shared_ptr<ast::VarDeclaration> Parser::ParseVarDeclaration()
    {
        auto id_list = ParseIdList();
        Match(TOK_COLON);
        auto type = ParseType();
        return std::move(std::make_shared<ast::VarDeclaration>(std::move(id_list), std::move(type)));
    }

    std::shared_ptr<ast::Subprogram> Parser::ParseSubprogram()
    {
        auto subprogram_head = ParseSubprogramHead();
        Match(TOK_SEMICOLON);
        auto subprogram_body = ParseSubprogramBody();
        return std::move(std::make_shared<ast::Subprogram>(std::move(subprogram_head), std::move(subprogram_body)));
    }

    std::shared_ptr<ast::SubprogramHead> Parser::ParseSubprogramHead()
    {
        if (token_ == TOK_PROCEDURE)
        {
            NextToken();
            auto name = text_;
            Match(TOK_ID);
            auto subprogram_head = std::make_shared<ast::SubprogramHead>(name);
            if (token_ == TOK_LPAREN)
            {
                NextToken();
                while (true)
                {
                    subprogram_head->AddParam(std::move(ParseParament()));
                    if (token_ == TOK_SEMICOLON)
                    {
                        NextToken();
                    }
                    else
                    {
                        break;
                    }
                }
                Match(TOK_RPAREN);
            }
            return std::move(subprogram_head);
        }
        else if (token_ == TOK_FUNCTION)
        {
            NextToken();
            auto name = text_;
            Match(TOK_ID);
            auto subprogram_head = std::make_shared<ast::SubprogramHead>(name);
            if (token_ == TOK_LPAREN)
            {
                NextToken();
                subprogram_head->AddParam(std::move(ParseParameter()));
                while (token_ == TOK_SEMICOLON)
                {
                    NextToken();
                    subprogram_head->AddParam(std::move(ParseParameter()));
                }
                Match(TOK_RPAREN);
            }
            Match(TOK_COLON);
            if (token_ == TOK_INTEGER_TYPE || token_ == TOK_REAL_TYPE || token_ == TOK_CHAR_TYPE || token_ == TOK_BOOLEAN_TYPE)
            {
                subprogram_head->set_return_type(token_);
                NextToken();
            }
            else
            {
                sprintf(buff, "%d:%d syntax err:expected basic type(integer, real, char, boolean) got %c", line_, col_, token_);
                NextToken();
                throw SyntaxErr(std::string(buff));
            }
            return std::move(subprogram_head);
        }
        else
        {
            sprintf(buff, "%d:%d syntax err:expected function or procedure got %c", line_, col_, token_);
            NextToken();
            throw SyntaxErr(std::string(buff));
        }
    }

    std::shared_ptr<ast::SubprogramBody> Parser::ParseSubprogramBody()
    {
        auto subprogram_body = std::make_shared<ast::SubprogramBody>();

        // Parse const declarations
        if (token_ = TOK_CONST)
        {
            NextToken();
            while (token_ != TOK_VAR && token_ != TOK_BEGIN)
            {
                subprogram_body->AddConstDeclaration(std::move(ParseConstDeclaration()));
                Match(TOK_SEMICOLON);
            }
        }

        // Parse var declarations
        if (token_ = TOK_VAR)
        {
            NextToken();
            while (token_ != TOK_BEGIN)
            {
                subprogram_body->AddVarDeclaration(std::move(ParseVarDeclaration()));
                Match(TOK_SEMICOLON);
            }
        }

        // Parse compound statement
        Match(TOK_BEGIN);
        while (token_ != TOK_END)
        {
            subprogram_body->AddStatement(std::move(ParseStatement()));
            Match(TOK_SEMICOLON);
        }
        Match(TOK_END);
        return std::move(subprogram_body);
    }

    std::shared_ptr<ast::IdList> Parser::ParseIdList()
    {
        auto id_list = std::make_shared<ast::IdList>();
        id_list->AddId(text_);
        Match(TOK_ID);
        while (token_ == TOK_COMMA)
        {
            NextToken();
            id_list->AddId(text_);
            Match(TOK_ID);
        }
        return std::move(id_list);
    }

    std::shared_ptr<ast::Type> Parser::ParseType()
    {
        if (token_ == TOK_INTEGER_TYPE || token_ == TOK_REAL_TYPE || token_ == TOK_CHAR_TYPE || token_ == TOK_BOOLEAN_TYPE)
        {
            auto type = std::make_shared<ast::Type>(false, token_);
            NextToken();
            return std::move(type);
        }
        else if (token_ == TOK_ARRAY)
        {
            auto type = std::make_shared<ast::Type>(true);
            NextToken();
            Match(TOK_LBRACK);
            type->AddPeriod(std::move(ParsePeriod()));
            while (token_ == TOK_COMMA)
            {
                NextToken();
                type->AddPeriod(std::move(ParsePeriod()));
            }
            Match(TOK_RBRACK);
            Match(TOK_OF);
            if (token_ == TOK_INTEGER_TYPE || token_ == TOK_REAL_TYPE || token_ == TOK_CHAR_TYPE || token_ == TOK_BOOLEAN_TYPE)
            {
                type->set_basic_type(token_);
                NextToken();
            }
            else
            {
                sprintf(buff, "%d:%d syntax err:expected basic type(integer, real, char, boolean) got %c", line_, col_, token_);
                NextToken();
                throw SyntaxErr(std::string(buff));
            }
        }
        else
        {
            sprintf(buff, "%d:%d syntax err:expected basic type(integer, real, char, boolean) or array got %c", line_, col_, token_);
            NextToken();
            throw SyntaxErr(std::string(buff));
        }
    }

    std::shared_ptr<ast::Period> Parser::ParsePeriod()
    {
        // TODO: ParsePeriod
    }

    std::shared_ptr<ast::Parameter> Parser::ParseParameter()
    {
        bool is_var;
        if (token_ == TOK_VAR)
        {
            is_var = true;
            NextToken();
        }
        else
        {
            is_var = false;
        }
        auto id_list = ParseIdList();
        Match(TOK_COLON);
        int type;
        if (token_ == TOK_INTEGER || token_ == TOK_REAL || token_ == TOK_CHAR || token_ == TOK_BOOLEAN)
        {
            type = token_;
            NextToken();
        }
        else
        {
            sprintf(buff, "%d:%d syntax err:expected basic type(integer, real, char, boolean) got %c", line_, col_, token_);
            NextToken();
            throw SyntaxErr(std::string(buff));
        }
        return std::make_shared<ast::Parameter>(is_var, std::move(id_list), type);
    }
}

#include <vector>

#include "parser.h"
#include "ast/program.h"

extern "C"
{
#include "lexer.h"
}

namespace pascal2c::parser
{
    template <typename Tp>
    using vector = ::std::vector<Tp>;

    std::shared_ptr<ast::Program> Parser::ParseProgram()
    {
        auto program_head = ParseProgramHead();
        Match(';');
        auto program_body = ParseProgramBody();
        Match('.');
        return std::move(std::make_shared<ast::Program>(std::move(program_head), std::move(program_body)));
    }

    std::shared_ptr<ast::ProgramHead> Parser::ParseProgramHead()
    {
        Match(TOK_PROGRAM);
        auto name = text_;
        Match(TOK_ID);
        if (token_ == '(')
        {
            NextToken();
            auto id_list = ParseIdList();
            Match(')');
            return std::move(std::make_shared<ast::ProgramHead>(name, std::move(id_list)));
        }
        return std::move(std::make_shared<ast::ProgramHead>(name));
    }

    std::shared_ptr<ast::ProgramBody> Parser::ParseProgramBody()
    {
        auto program_body = std::make_shared<ast::ProgramBody>();

        // Parse const declarations
        if (token_ == TOK_CONST)
        {
            NextToken();
            while (token_ != TOK_VAR && token_ != TOK_PROCEDURE && token_ != TOK_FUNCTION && token_ != TOK_BEGIN)
            {
                program_body->AddConstDeclaration(std::move(ParseConstDeclaration()));
                Match(';');
            }
        }

        // Parse var declarations
        if (token_ == TOK_VAR)
        {
            NextToken();
            while (token_ != TOK_PROCEDURE && token_ != TOK_FUNCTION && token_ != TOK_BEGIN)
            {
                program_body->AddVarDeclaration(std::move(ParseVarDeclaration()));
                Match(';');
            }
        }

        // Parse subprograms
        if (token_ == TOK_PROCEDURE || token_ == TOK_FUNCTION)
        {
            while (token_ != TOK_BEGIN)
            {
                program_body->AddSubprogram(std::move(ParseSubprogram()));
                Match(';');
            }
        }

        // Parse compound statement
        Match(TOK_BEGIN);
        while (token_ != TOK_END)
        {
            program_body->AddStatement(std::move(ParseStatement()));
            Match(';');
        }
        Match(TOK_END);
        return std::move(program_body);
    }

    std::shared_ptr<ast::ConstDeclaration> Parser::ParseConstDeclaration()
    {
        auto name = text_;
        Match(TOK_ID);
        Match(TOK_ASSIGNOP);
        return std::move(std::make_shared<ast::ConstDeclaration>(name, std::move(ParsePrimary())));
    }

    std::shared_ptr<ast::VarDeclaration> Parser::ParseVarDeclaration()
    {
        auto id_list = ParseIdList();
        Match(':');
        auto type = ParseType();
        return std::move(std::make_shared<ast::VarDeclaration>(std::move(id_list), std::move(type)));
    }

    std::shared_ptr<ast::Subprogram> Parser::ParseSubprogram()
    {
        auto subprogram_head = ParseSubprogramHead();
        Match(';');
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
            if (token_ == '(')
            {
                NextToken();
                while (true)
                {
                    subprogram_head->AddParameter(std::move(ParseParameter()));
                    if (token_ == ';')
                    {
                        NextToken();
                    }
                    else
                    {
                        break;
                    }
                }
                Match(')');
            }
            return std::move(subprogram_head);
        }
        else if (token_ == TOK_FUNCTION)
        {
            NextToken();
            auto name = text_;
            Match(TOK_ID);
            auto subprogram_head = std::make_shared<ast::SubprogramHead>(name);
            if (token_ == '(')
            {
                NextToken();
                subprogram_head->AddParameter(std::move(ParseParameter()));
                while (token_ == ';')
                {
                    NextToken();
                    subprogram_head->AddParameter(std::move(ParseParameter()));
                }
                Match(')');
            }
            Match(':');
            if (token_ == TOK_INTEGER_TYPE || token_ == TOK_REAL_TYPE || token_ == TOK_CHAR_TYPE || token_ == TOK_BOOLEAN_TYPE)
            {
                subprogram_head->set_return_type(token_);
                NextToken();
            }
            else
            {
                char buff[1024];
                sprintf(buff, "%d:%d syntax err:expected basic type(integer, real, char, boolean) got %c", line_, column_, token_);
                NextToken();
                throw SyntaxErr(std::string(buff));
            }
            return std::move(subprogram_head);
        }
        else
        {
            char buff[1024];
            sprintf(buff, "%d:%d syntax err:expected function or procedure got %c", line_, column_, token_);
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
                Match(';');
            }
        }

        // Parse var declarations
        if (token_ = TOK_VAR)
        {
            NextToken();
            while (token_ != TOK_BEGIN)
            {
                subprogram_body->AddVarDeclaration(std::move(ParseVarDeclaration()));
                Match(';');
            }
        }

        // Parse compound statement
        Match(TOK_BEGIN);
        while (token_ != TOK_END)
        {
            subprogram_body->AddStatement(std::move(ParseStatement()));
            Match(';');
        }
        Match(TOK_END);
        return std::move(subprogram_body);
    }

    std::shared_ptr<ast::IdList> Parser::ParseIdList()
    {
        auto id_list = std::make_shared<ast::IdList>();
        id_list->AddId(text_);
        Match(TOK_ID);
        while (token_ == ',')
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
            Match('[');
            type->AddPeriod(ParsePeriod());
            while (token_ == ',')
            {
                NextToken();
                type->AddPeriod(ParsePeriod());
            }
            Match(']');
            Match(TOK_OF);
            if (token_ == TOK_INTEGER_TYPE || token_ == TOK_REAL_TYPE || token_ == TOK_CHAR_TYPE || token_ == TOK_BOOLEAN_TYPE)
            {
                type->set_basic_type(token_);
                NextToken();
            }
            else
            {
                char buff[1024];
                sprintf(buff, "%d:%d syntax err:expected basic type(integer, real, char, boolean) got %c", line_, column_, token_);
                NextToken();
                throw SyntaxErr(std::string(buff));
            }
        }
        else
        {
            char buff[1024];
            sprintf(buff, "%d:%d syntax err:expected basic type(integer, real, char, boolean) or array got %c", line_, column_, token_);
            NextToken();
            throw SyntaxErr(std::string(buff));
        }
    }

    ast::Type::Period Parser::ParsePeriod()
    {
        int value1 = tok_value_.intval;
        Match(TOK_INTEGER);
        Match('.');
        Match('.');
        Match('.');
        int value2 = tok_value_.intval;
        Match(TOK_INTEGER);
        return {value1, value2};
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
        Match(':');
        int type;
        if (token_ == TOK_INTEGER_TYPE || token_ == TOK_REAL_TYPE || token_ == TOK_CHAR_TYPE || token_ == TOK_BOOLEAN_TYPE)
        {
            type = token_;
            NextToken();
        }
        else
        {
            char buff[1024];
            sprintf(buff, "%d:%d syntax err:expected basic type(integer, real, char, boolean) got %c", line_, column_, token_);
            NextToken();
            throw SyntaxErr(std::string(buff));
        }
        return std::make_shared<ast::Parameter>(is_var, std::move(id_list), type);
    }
}

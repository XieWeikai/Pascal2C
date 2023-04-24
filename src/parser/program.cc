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
        INIT_PARSE(line_, column_);

        auto program_head = ParseProgramHead();
        Match(';');
        auto program_body = ParseProgramBody();
        Match('.');
        return MAKE_AND_MOVE_SHARED(ast::Program, std::move(program_head), std::move(program_body));
    }

    std::shared_ptr<ast::ProgramHead> Parser::ParseProgramHead()
    {
        INIT_PARSE(line_, column_);

        Match(TOK_PROGRAM);
        auto name = text_;
        Match(TOK_ID);
        if (token_ == '(')
        {
            NextToken();
            auto id_list = ParseIdList();
            Match(')');
            return MAKE_AND_MOVE_SHARED(ast::ProgramHead, name, std::move(id_list));
        }
        return MAKE_AND_MOVE_SHARED(ast::ProgramHead, name);
    }

    std::shared_ptr<ast::ProgramBody> Parser::ParseProgramBody()
    {
        INIT_PARSE(line_, column_);

        auto program_body = std::make_shared<ast::ProgramBody>();
        program_body->SetLineAndColumn(begin_line, begin_column);

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
        program_body->set_statements(std::move(ParseCompoundStatement()));

        return std::move(program_body);
    }

    std::shared_ptr<ast::ConstDeclaration> Parser::ParseConstDeclaration()
    {
        INIT_PARSE(line_, column_);

        auto name = text_;
        Match(TOK_ID);
        Match(TOK_ASSIGNOP);
        return MAKE_AND_MOVE_SHARED(ast::ConstDeclaration, name, std::move(ParsePrimary()));
    }

    std::shared_ptr<ast::VarDeclaration> Parser::ParseVarDeclaration()
    {
        INIT_PARSE(line_, column_);

        auto id_list = ParseIdList();
        Match(':');
        auto type = ParseType();
        return MAKE_AND_MOVE_SHARED(ast::VarDeclaration, std::move(id_list), std::move(type));
    }

    std::shared_ptr<ast::Subprogram> Parser::ParseSubprogram()
    {
        INIT_PARSE(line_, column_);

        auto subprogram_head = ParseSubprogramHead();
        Match(';');
        auto subprogram_body = ParseSubprogramBody();
        return MAKE_AND_MOVE_SHARED(ast::Subprogram, std::move(subprogram_head), std::move(subprogram_body));
    }

    std::shared_ptr<ast::SubprogramHead> Parser::ParseSubprogramHead()
    {
        INIT_PARSE(line_, column_);

        if (token_ == TOK_PROCEDURE)
        {
            NextToken();
            auto name = text_;
            Match(TOK_ID);
            auto subprogram_head = MAKE_SHARED(ast::SubprogramHead, name);
            if (token_ == '(')
            {
                NextToken();
                while (token_ != ')')
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
            auto subprogram_head = MAKE_SHARED(ast::SubprogramHead, name);
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
                ThrowSyntaxErr("basic type(integer, real, char, boolean)");
            }
            return std::move(subprogram_head);
        }
        else
        {
            ThrowSyntaxErr("function or procedure");
        }
    }

    std::shared_ptr<ast::SubprogramBody> Parser::ParseSubprogramBody()
    {
        INIT_PARSE(line_, column_);

        auto subprogram_body = std::make_shared<ast::SubprogramBody>();
        subprogram_body->SetLineAndColumn(begin_line, begin_column);

        // Parse const declarations
        if (token_ == TOK_CONST)
        {
            NextToken();
            while (token_ != TOK_VAR && token_ != TOK_BEGIN)
            {
                subprogram_body->AddConstDeclaration(std::move(ParseConstDeclaration()));
                Match(';');
            }
        }

        // Parse var declarations
        if (token_ == TOK_VAR)
        {
            NextToken();
            while (token_ != TOK_BEGIN)
            {
                subprogram_body->AddVarDeclaration(std::move(ParseVarDeclaration()));
                Match(';');
            }
        }

        // Parse compound statement
        subprogram_body->set_statements(std::move(ParseCompoundStatement()));

        return std::move(subprogram_body);
    }

    std::shared_ptr<ast::IdList> Parser::ParseIdList()
    {
        INIT_PARSE(line_, column_);

        auto id_list = std::make_shared<ast::IdList>();
        id_list->SetLineAndColumn(begin_line, begin_column);

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
        INIT_PARSE(line_, column_);

        if (token_ == TOK_INTEGER_TYPE || token_ == TOK_REAL_TYPE || token_ == TOK_CHAR_TYPE || token_ == TOK_BOOLEAN_TYPE)
        {
            auto type = MAKE_SHARED(ast::Type, false, token_);
            NextToken();
            return std::move(type);
        }
        else if (token_ == TOK_ARRAY)
        {
            auto type = MAKE_SHARED(ast::Type, true);
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
                return std::move(type);
            }
            else
            {
                ThrowSyntaxErr("basic type(integer, real, char, boolean)");
            }
        }
        else
        {
            ThrowSyntaxErr("basic type(integer, real, char, boolean) or array");
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
        INIT_PARSE(line_, column_);

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
            ThrowSyntaxErr("basic type(integer, real, char, boolean)");
        }
        return MAKE_AND_MOVE_SHARED(ast::Parameter, is_var, id_list, type);
    }
}

#include <vector>
#include <common/logger.h>

#include "syntax.h"
#include "ast/program.h"

#define TOK_SEMICOLON   ';'
#define TOK_COLON       ':'
#define TOK_LPAREN      '('
#define TOK_RPAREN      ')'
#define TOK_LBRACK      '['
#define TOK_RBRACK      ']'
#define TOK_COMMA       ','

namespace Pascal2C::Syntax
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
        auto name = cur.text;
        Match(TOK_ID);
        if (cur.token == '(')
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
        if (cur.token == TOK_CONST)
        {
            NextToken();
            while ( cur.token != TOK_VAR && 
                    cur.token != TOK_PROCEDURE && 
                    cur.token != TOK_FUNCTION && 
                    cur.token != TOK_BEGIN )
            {
                program_body->AddConstDeclaration(std::move(ParseConstDeclaration()));
                Match(TOK_SEMICOLON);
            }
        }

        // Parse var declarations
        if (cur.token == TOK_VAR)
        {
            NextToken();
            while (cur.token != TOK_PROCEDURE && cur.token != TOK_FUNCTION && cur.token != TOK_BEGIN)
            {
                program_body->AddVarDeclaration(std::move(ParseVarDeclaration()));
                Match(TOK_SEMICOLON);
            }
        }

        // Parse subprograms
        if (cur.token == TOK_PROCEDURE || cur.token == TOK_FUNCTION)
        {
            while (cur.token != TOK_BEGIN)
            {
                program_body->AddSubprogram(std::move(ParseSubprogram()));
                Match(TOK_SEMICOLON);
            }
        }

        // Parse compound statement
        Match(TOK_BEGIN);
        while (cur.token != TOK_END)
        {
            program_body->AddStatement(std::move(ParseStatement()));
            Match(TOK_SEMICOLON);
        }
        Match(TOK_END);
        return std::move(program_body);
    }

    std::shared_ptr<ast::ConstDeclaration> Parser::ParseConstDeclaration()
    {
        auto name = cur.text;
        Match(TOK_ID);
        Match(TOK_ASSIGNOP);
        return std::move(std::make_shared<ast::ConstDeclaration>(name, std::move(ParsePrimary())));
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
        if (cur.token == TOK_PROCEDURE)
        {
            NextToken();
            auto name = cur.text;
            Match(TOK_ID);
            auto subprogram_head = std::make_shared<ast::SubprogramHead>(name);
            if (cur.token == TOK_LPAREN)
            {
                NextToken();
                while (true)
                {
                    subprogram_head->AddParameter(std::move(ParseParameter()));
                    if (cur.token == TOK_SEMICOLON)
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
        else if (cur.token == TOK_FUNCTION)
        {
            NextToken();
            auto name = cur.text;
            Match(TOK_ID);
            auto subprogram_head = std::make_shared<ast::SubprogramHead>(name);
            if (cur.token == TOK_LPAREN)
            {
                NextToken();
                subprogram_head->AddParameter(std::move(ParseParameter()));
                while (cur.token == TOK_SEMICOLON)
                {
                    NextToken();
                    subprogram_head->AddParameter(std::move(ParseParameter()));
                }
                Match(TOK_RPAREN);
            }
            Match(TOK_COLON);
            if (cur.token == TOK_INTEGER_TYPE || cur.token == TOK_REAL_TYPE || cur.token == TOK_CHAR_TYPE || cur.token == TOK_BOOLEAN_TYPE)
            {
                subprogram_head->set_return_type(cur.token);
                NextToken();
            }
            else
            {
                SYN_ERROR("Someing fatal");
                NextToken();
                throw SyntaxErr(std::string(buff));
            }
            return std::move(subprogram_head);
        }
        else
        {
            SYN_ERROR("Someing fatal");
            NextToken();
            throw SyntaxErr(std::string(buff));
        }
    }

    std::shared_ptr<ast::SubprogramBody> Parser::ParseSubprogramBody()
    {
        auto subprogram_body = std::make_shared<ast::SubprogramBody>();

        // Parse const declarations
        if (cur.token = TOK_CONST)
        {
            NextToken();
            while (cur.token != TOK_VAR && cur.token != TOK_BEGIN)
            {
                subprogram_body->AddConstDeclaration(std::move(ParseConstDeclaration()));
                Match(TOK_SEMICOLON);
            }
        }

        // Parse var declarations
        if (cur.token = TOK_VAR)
        {
            NextToken();
            while (cur.token != TOK_BEGIN) {
                subprogram_body->AddVarDeclaration(std::move(ParseVarDeclaration()));
                Match(TOK_SEMICOLON);
            }
        }

        // Parse compound statement
        Match(TOK_BEGIN);
        while (cur.token != TOK_END)
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
        id_list->AddId(cur.text);
        Match(TOK_ID);
        while (cur.token == TOK_COMMA)
        {
            NextToken();
            id_list->AddId(cur.text);
            Match(TOK_ID);
        }
        return std::move(id_list);
    }

    std::shared_ptr<ast::Type> Parser::ParseType()
    {
        static char buff[256];
        if (cur.token == TOK_INTEGER_TYPE || cur.token == TOK_REAL_TYPE || cur.token == TOK_CHAR_TYPE || cur.token == TOK_BOOLEAN_TYPE)
        {
            auto type = std::make_shared<ast::Type>(false, cur.token);
            NextToken();
            return std::move(type);
        }
        else if (cur.token == TOK_ARRAY)
        {
            auto type = std::make_shared<ast::Type>(true);
            NextToken();
            Match(TOK_LBRACK);
            type->AddPeriod(ParsePeriod());
            while (cur.token == TOK_COMMA)
            {
                NextToken();
                type->AddPeriod(ParsePeriod());
            }
            Match(TOK_RBRACK);
            Match(TOK_OF);
            if (cur.token == TOK_INTEGER_TYPE || cur.token == TOK_REAL_TYPE || cur.token == TOK_CHAR_TYPE || cur.token == TOK_BOOLEAN_TYPE)
            {
                type->set_basic_type(cur.token);
                NextToken();
            }
            else
            {
                sprintf(buff, "%d:%d syntax err:expected basic type(integer, real, char, boolean) got %c", 
                                cur.line, cur.column, cur.token);
                NextToken();
                throw SyntaxErr(std::string(buff));
            }
        }
        else
        {
            sprintf(buff, "%d:%d syntax err:expected basic type(integer, real, char, boolean) or array got %c", cur.line, cur.column, cur.token);
            NextToken();
            throw SyntaxErr(std::string(buff));
        }
        TODO; // no return
        return nullptr;
    }


    ast::Type::Period Parser::ParsePeriod()
    {
        int value1 = cur.tok_value.intval;
        Match(TOK_INTEGER);
        Match('.');
        Match('.');
        Match('.');
        int value2 = cur.tok_value.intval;
        Match(TOK_INTEGER);
        TODO; //这个区间类不一定是数字
        return {value1, value2};
    }

    std::shared_ptr<ast::Parameter> Parser::ParseParameter()
    {
        bool is_var = false;
        if (cur.token == TOK_VAR) {
            is_var = true;
            NextToken();
        } 
        
        auto id_list = ParseIdList();
        Match(TOK_COLON);
        int type;

        if (cur.token == TOK_INTEGER_TYPE|| 
            cur.token == TOK_REAL_TYPE   || 
            cur.token == TOK_CHAR_TYPE   || 
            cur.token == TOK_BOOLEAN_TYPE )
        {
            type = cur.token;
            NextToken();
        
        } else {
            static char buff[256];
            sprintf(buff, "%d:%d syntax err:expected basic type(integer, real, char, boolean) got %c", cur.line, cur.column, cur.token);
            NextToken();
            throw SyntaxErr(std::string(buff));
        }
        return std::make_shared<ast::Parameter>(is_var, std::move(id_list), type);
    }
}

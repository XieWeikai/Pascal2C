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

    // ProgramHead ; ProgramBody .
    std::shared_ptr<ast::Program> Parser::ParseProgram()
    {
        INIT_PARSE(line_, column_);

        auto program_head = ParseProgramHead();
        CheckMatch(';', {TOK_CONST, TOK_VAR, TOK_PROCEDURE, TOK_FUNCTION, TOK_BEGIN});
        auto program_body = ParseProgramBody();
        CheckMatch('.', {});
        return MAKE_AND_MOVE_SHARED(ast::Program, std::move(program_head), std::move(program_body));
    }

    // TOK_PROGRAM TOK_ID [(IdList)] ;
    std::shared_ptr<ast::ProgramHead> Parser::ParseProgramHead()
    {
        INIT_PARSE(line_, column_);

        CheckMatch(TOK_PROGRAM, {TOK_ID, '(', ';'});
        auto name = text_;

        // Parse the program id
        int ret = CheckMatch(TOK_ID, {'(', ';'});
        if (ret == ';' || ret == TOK_EOF)
        {
            return MAKE_AND_MOVE_SHARED(ast::ProgramHead, "");
        }
        else if (ret == '(')
        {
            name = "";
        }

        // If the program has a parameter list, parse it
        if (token_ == '(')
        {
            NextToken();
            auto id_list = ParseIdList();
            CheckMatch(')', {';'});
            return MAKE_AND_MOVE_SHARED(ast::ProgramHead, name, std::move(id_list));
        }
        return MAKE_AND_MOVE_SHARED(ast::ProgramHead, name);
    }

    // [TOK_CONST {ConstDeclaration;}]
    // [TOK_VAR {VarDeclaration;}]
    // {Subprogram;}
    // CompoundStatement
    std::shared_ptr<ast::ProgramBody> Parser::ParseProgramBody()
    {
        INIT_PARSE(line_, column_);

        auto program_body = MAKE_SHARED_WITH_NO_ARGUMENT(ast::ProgramBody);

        // Parse const declarations
        if (token_ == TOK_CONST)
        {
            NextToken();
            while (token_ == TOK_ID)
            {
                program_body->AddConstDeclaration(std::move(ParseConstDeclaration()));
                CheckMatch(';', {TOK_ID, TOK_VAR, TOK_PROCEDURE, TOK_FUNCTION, TOK_BEGIN});
            }
        }

        // Parse var declarations
        if (token_ == TOK_VAR)
        {
            NextToken();
            while (token_ == TOK_ID)
            {
                program_body->AddVarDeclaration(std::move(ParseVarDeclaration()));
                CheckMatch(';', {TOK_ID, TOK_PROCEDURE, TOK_FUNCTION, TOK_BEGIN});
            }
        }

        // Parse subprograms
        while (token_ == TOK_PROCEDURE || token_ == TOK_FUNCTION)
        {
            program_body->AddSubprogram(std::move(ParseSubprogram()));
            CheckMatch(';', {TOK_PROCEDURE, TOK_FUNCTION, TOK_BEGIN});
        }

        // Parse compound statement
        program_body->set_statements(std::move(ParseCompoundStatement()));

        return std::move(program_body);
    }

    // TOK_ID = PrimaryExpression
    std::shared_ptr<ast::ConstDeclaration> Parser::ParseConstDeclaration()
    {
        INIT_PARSE(line_, column_);

        // Parse id
        auto name = text_;
        int ret = CheckMatch(TOK_ID, {'=', ';'});
        if (ret != TOK_ID)
        {
            name = "";
        }

        CheckMatch('=', {TOK_INTEGER, TOK_REAL, '+', '-', TOK_STRING, ';'});

        // Parse const value
        std::shared_ptr<ast::Expression> const_value;
        while (true)
        {
            try
            {
                const_value = std::move(ParsePrimary());
                break;
            }
            catch (SyntaxErr &err)
            {
                AddSyntaxErr(err);
                std::set<int> tokens({TOK_INTEGER, TOK_REAL, '-', '+', TOK_STRING});
                int delimiter = ';';

                while (tokens.find(token_) == tokens.end() && delimiter != token_ && token_ != TOK_EOF)
                {
                    NextToken();
                }
                if (tokens.find(token_) == tokens.end())
                {
                    break;
                }
            }
        }

        return MAKE_AND_MOVE_SHARED(ast::ConstDeclaration, name, std::move(const_value));
    }

    // IdList : Type
    std::shared_ptr<ast::VarDeclaration> Parser::ParseVarDeclaration()
    {
        INIT_PARSE(line_, column_);

        auto id_list = ParseIdList();
        CheckMatch(':', {TOK_ARRAY, TOK_INTEGER_TYPE, TOK_REAL_TYPE, TOK_CHAR_TYPE, TOK_BOOLEAN_TYPE, ';'});
        auto type = ParseType();
        return MAKE_AND_MOVE_SHARED(ast::VarDeclaration, std::move(id_list), std::move(type));
    }

    // SubprogramHead ; SubprogramBody
    std::shared_ptr<ast::Subprogram> Parser::ParseSubprogram()
    {
        INIT_PARSE(line_, column_);

        auto subprogram_head = ParseSubprogramHead();
        CheckMatch(';', {TOK_CONST, TOK_VAR, TOK_BEGIN});
        auto subprogram_body = ParseSubprogramBody();
        return MAKE_AND_MOVE_SHARED(ast::Subprogram, std::move(subprogram_head), std::move(subprogram_body));
    }

    // TOK_PROCEDURE TOK_ID [(Parameter {; Parameter})]
    // TOK_FUNCTION TOK_ID [(Parameter {; Parameter})] : TOK_INTEGER_TYPE | TOK_REAL_TYPE | TOK_CHAR_TYPE | TOK_BOOLEAN_TYPE
    std::shared_ptr<ast::SubprogramHead> Parser::ParseSubprogramHead()
    {
        INIT_PARSE(line_, column_);

        if (token_ == TOK_PROCEDURE)
        {
            // Parse procedure head
            NextToken();

            // Parse procedure id
            auto name = text_;
            int ret = CheckMatch(TOK_ID, {'(', ';'});
            if (ret == ';' || ret == TOK_EOF)
            {
                return MAKE_AND_MOVE_SHARED(ast::SubprogramHead, "");
            }
            else if (ret == '(')
            {
                name = "";
            }

            // If the procedure has a parameter list, parse it
            auto subprogram_head = MAKE_SHARED(ast::SubprogramHead, name);
            if (token_ == '(')
            {
                NextToken();
                if (token_ != ')')
                {
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
                }
                CheckMatch(')', {';'});
            }
            return std::move(subprogram_head);
        }
        else
        {
            // Parse function head
            NextToken();

            // Parse function id
            auto name = text_;
            int ret = CheckMatch(TOK_ID, {'(', ';'});
            if (ret == ';' || ret == TOK_EOF)
            {
                return MAKE_AND_MOVE_SHARED(ast::SubprogramHead, "");
            }
            else if (ret == '(')
            {
                name = "";
            }

            // If the function has a parameter list, parse it
            auto subprogram_head = MAKE_SHARED(ast::SubprogramHead, name);
            if (token_ == '(')
            {
                NextToken();
                if (token_ != ')')
                {
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
                }
                CheckMatch(')', {':', ';'});
            }

            CheckMatch(':', {TOK_INTEGER_TYPE, TOK_REAL_TYPE, TOK_CHAR_TYPE, TOK_BOOLEAN_TYPE, ';'});

            // Parse return type
            ret = CheckMatch({TOK_INTEGER_TYPE, TOK_REAL_TYPE, TOK_CHAR_TYPE, TOK_BOOLEAN_TYPE}, "basic type(integer, real, bool, char)", {';'});
            if (ret != ';' && ret != TOK_EOF)
            {
                subprogram_head->set_return_type(ret);
            }
            else
            {
                subprogram_head->set_return_type(0);
            }

            return std::move(subprogram_head);
        }
    }

    // [TOK_CONST {ConstDeclaration;}]
    // [TOK_VAR {VarDeclaration;}]
    // CompoundStatement
    std::shared_ptr<ast::SubprogramBody> Parser::ParseSubprogramBody()
    {
        INIT_PARSE(line_, column_);

        auto subprogram_body = MAKE_SHARED_WITH_NO_ARGUMENT(ast::SubprogramBody);

        // Parse const declarations
        if (token_ == TOK_CONST)
        {
            NextToken();
            while (token_ == TOK_ID)
            {
                subprogram_body->AddConstDeclaration(std::move(ParseConstDeclaration()));
                CheckMatch(';', {TOK_ID, TOK_VAR, TOK_BEGIN});
            }
        }

        // Parse var declarations
        if (token_ == TOK_VAR)
        {
            NextToken();
            while (token_ == TOK_ID)
            {
                subprogram_body->AddVarDeclaration(std::move(ParseVarDeclaration()));
                CheckMatch(';', {TOK_ID, TOK_BEGIN});
            }
        }

        // Parse compound statement
        subprogram_body->set_statements(std::move(ParseCompoundStatement()));

        return std::move(subprogram_body);
    }

    // TOK_ID {, TOK_ID}
    std::shared_ptr<ast::IdList> Parser::ParseIdList()
    {
        INIT_PARSE(line_, column_);

        auto id_list = MAKE_SHARED_WITH_NO_ARGUMENT(ast::IdList);

        // Parse the first id
        std::string id = text_;
        int ret = CheckMatch(TOK_ID, {',', ':', ')', ';'});
        if (ret == TOK_ID)
        {
            id_list->AddId(id);
        }
        else if (ret != ',')
        {
            return std::move(id_list);
        }

        // Parse the following ids, like , id2, id3, ...
        while (token_ == ',')
        {
            NextToken();
            std::string id = text_;
            int ret = CheckMatch(TOK_ID, {',', ':', ')', ';'});
            if (ret == TOK_ID)
            {
                id_list->AddId(id);
            }
            else if (ret != ',')
            {
                return std::move(id_list);
            }
        }
        return std::move(id_list);
    }

    // TOK_INTEGER_TYPE | TOK_REAL_TYPE | TOK_CHAR_TYPE | TOK_BOOLEAN_TYPE
    // TOK_ARRAY \[ Period {, Period} \] TOK_OF TOK_INTEGER_TYPE | TOK_REAL_TYPE | TOK_CHAR_TYPE | TOK_BOOLEAN_TYPE
    std::shared_ptr<ast::Type> Parser::ParseType()
    {
        INIT_PARSE(line_, column_);

        int ret = CheckMatch({TOK_ARRAY, TOK_INTEGER_TYPE, TOK_REAL_TYPE, TOK_CHAR_TYPE, TOK_BOOLEAN_TYPE}, "basic type(integer, real, bool, char) or array", {'[', ';'});

        if (ret == TOK_INTEGER_TYPE || ret == TOK_REAL_TYPE || ret == TOK_CHAR_TYPE || ret == TOK_BOOLEAN_TYPE)
        {
            return MAKE_AND_MOVE_SHARED(ast::Type, false, ret);
        }
        else if (ret == TOK_ARRAY || ret == '[')
        {
            auto type = MAKE_SHARED(ast::Type, true);
            CheckMatch('[', {TOK_INTEGER, ']', ';'});

            // If the array has a period list, parse it
            if (token_ != ']')
            {
                while (true)
                {
                    type->AddPeriod(ParsePeriod());
                    if (token_ == ',')
                    {
                        NextToken();
                    }
                    else
                    {
                        break;
                    }
                }
            }
            CheckMatch(']', {TOK_OF, ';'});

            CheckMatch(TOK_OF, {TOK_INTEGER_TYPE, TOK_REAL_TYPE, TOK_CHAR_TYPE, TOK_BOOLEAN_TYPE, ';'});

            ret = CheckMatch({TOK_INTEGER_TYPE, TOK_REAL_TYPE, TOK_CHAR_TYPE, TOK_BOOLEAN_TYPE}, "basic type(integer, real, bool, char)", {';'});
            if (ret != ';' && ret != TOK_EOF)
            {
                type->set_basic_type(ret);
            }

            return std::move(type);
        }
        else
        {
            return MAKE_AND_MOVE_SHARED(ast::Type, false);
        }
    }

    // TOK_INTEGER TOK_DOTDOT TOK_INTEGER
    ast::Type::Period Parser::ParsePeriod()
    {
        int value1 = tok_value_.intval;
        if (CheckMatch(TOK_INTEGER, {TOK_DOTDOT, ';'}) != TOK_INTEGER)
        {
            value1 = -1;
        }

        CheckMatch(TOK_DOTDOT, {TOK_INTEGER, ';'});

        int value2 = tok_value_.intval;
        if (CheckMatch(TOK_INTEGER, {',', ']', ';'}) != TOK_INTEGER)
        {
            value2 = -1;
        }

        return {value1, value2};
    }

    // [TOK_VAR] IdList : TOK_INTEGER_TYPE | TOK_REAL_TYPE | TOK_CHAR_TYPE | TOK_BOOLEAN_TYPE
    std::shared_ptr<ast::Parameter> Parser::ParseParameter()
    {
        INIT_PARSE(line_, column_);

        // Check if the parameter is a var parameter
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

        // Parse the type of the parameter
        int type = -1;
        CheckMatch(':', {TOK_INTEGER_TYPE, TOK_REAL_TYPE, TOK_CHAR_TYPE, TOK_BOOLEAN_TYPE, ';'});
        int ret = CheckMatch({TOK_INTEGER_TYPE, TOK_REAL_TYPE, TOK_CHAR_TYPE, TOK_BOOLEAN_TYPE}, "basic type(integer, real, bool, char)", {';'});
        if (ret != ';' && ret != TOK_EOF)
        {
            type = ret;
        }

        return MAKE_AND_MOVE_SHARED(ast::Parameter, is_var, id_list, type);
    }
}

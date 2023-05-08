#include <string>
#include <sstream>

#include "program.h"

extern "C"
{
#include "lexer.h"
}

// Output the line and column number of the current token and the current level of indentation.
#define INIT_TOSTRING(str_s, level) \
    std::stringstream str_s;        \
    LineColumnOutput(str_s);        \
    IndentOutput(str_s, level);

namespace pascal2c::ast
{
    using string = ::std::string;
    using stringstream = ::std::stringstream;

    const string TypeToString(const int type)
    {
        switch (type)
        {
        case TOK_INTEGER_TYPE:
            return "integer";
        case TOK_REAL_TYPE:
            return "real";
        case TOK_CHAR_TYPE:
            return "char";
        case TOK_BOOLEAN_TYPE:
            return "boolean";
        default:
            return "unknown";
        }
    }
    // eg. IdList: a, b, c
    const string IdList::ToString(const int level) const
    {
        INIT_TOSTRING(str_s, level);

        str_s << "IdList: ";

        if (id_list_.size() > 0)
        {
            str_s << id_list_[0];
        }
        for (int i = 1; i < id_list_.size(); i++)
        {
            str_s << ", " << id_list_[i];
        }
        return str_s.str();
    }

    // eg. Type: array [1..10, 1..10] of integer
    // eg. Type: integer
    const string Type::ToString(const int level) const
    {
        INIT_TOSTRING(str_s, level);

        str_s << "Type: ";
        if (is_array_)
        {
            str_s << "array [";
            if (periods_.size() > 0)
            {
                str_s << periods_[0].digits_1 << ".." << periods_[0].digits_2;
                for (int i = 1; i < periods_.size(); i++)
                {
                    str_s << ", " << periods_[i].digits_1 << ".." << periods_[i].digits_2;
                }
            }
            str_s << "] of ";
            str_s << TypeToString(basic_type_);
        }
        else
        {
            str_s << TypeToString(basic_type_);
        }
        return str_s.str();
    }

    // eg. Parameter: var integer
    //         IdList: a, b, c
    // eg. Parameter: integer
    //        IdList: a, b, c
    const string Parameter::ToString(const int level) const
    {
        INIT_TOSTRING(str_s, level);

        str_s << "Parameter: ";

        if (is_var_)
        {
            str_s << "var ";
        }
        str_s << TypeToString(type_) << std::endl;

        str_s << id_list_->ToString(level + 1);
        return str_s.str();
    }

    // eg. ConstDeclaration: a
    //         1
    const string ConstDeclaration::ToString(const int level) const
    {
        INIT_TOSTRING(str_s, level);

        str_s << "ConstDeclaration: " << id_ << std::endl;
        if (const_value_)
        {
            str_s << const_value_->ToString(level + 1);
        }
        else
        {
            IndentOutput(str_s, level + 1);
            str_s << "unknown value" << std::endl;
        }
        return str_s.str();
    }

    // eg. VarDeclaration:
    //         Type: integer
    //         IdList: a, b, c
    const string VarDeclaration::ToString(const int level) const
    {
        INIT_TOSTRING(str_s, level);

        str_s << "VarDeclaration: " << std::endl;
        str_s << type_->ToString(level + 1) << std::endl;
        str_s << id_list_->ToString(level + 1);
        return str_s.str();
    }

    // eg. SubprogramHead: procedure a
    //         Parameter: integer
    //             IdList: a, b, c
    // eg. SubprogramHead: function a integer
    //         Parameter: integer
    //             IdList: a
    //         Parameter: real
    //             IdList: b, c
    const string SubprogramHead::ToString(const int level) const
    {
        INIT_TOSTRING(str_s, level);

        str_s << "SubprogramHead: ";
        if (return_type_ == -1)
        {
            str_s << "procedure " << id_ << std::endl;
        }
        else
        {
            str_s << "function " << id_ << " " << TypeToString(return_type_) << std::endl;
        }
        if (parameters_.size() > 0)
        {
            str_s << parameters_[0]->ToString(level + 1);
            for (int i = 1; i < parameters_.size(); i++)
            {
                str_s << std::endl
                      << parameters_[i]->ToString(level + 1);
            }
        }
        return str_s.str();
    }

    // eg. SubprogramBody:
    //         ConstDeclaration: a
    //             1
    //         VarDeclaration:
    //             Type: integer
    //             IdList: a, b, c
    //         CompoundStatement :0
    const string SubprogramBody::ToString(const int level) const
    {
        INIT_TOSTRING(str_s, level);

        str_s << "SubprogramBody: " << std::endl;
        for (int i = 0; i < const_declarations_.size(); i++)
        {
            str_s << const_declarations_[i]->ToString(level + 1) << std::endl;
        }

        for (int i = 0; i < var_declarations_.size(); i++)
        {
            str_s << var_declarations_[i]->ToString(level + 1) << std::endl;
        }

        str_s << statements_->ToString(level + 1) << std::endl;

        return str_s.str();
    }

    // eg. Subprogram:
    //         SubprogramHead: procedure a
    //         SubprogramBody:
    //             CompoundStatement: 0
    const string Subprogram::ToString(const int level) const
    {
        INIT_TOSTRING(str_s, level);

        str_s << "Subprogram: " << std::endl;
        str_s << subprogram_head_->ToString(level + 1) << std::endl;
        str_s << subprogram_body_->ToString(level + 1);
        return str_s.str();
    }

    // eg. ProgramHead: a
    //         IdList: a, b, c
    const string ProgramHead::ToString(const int level) const
    {
        INIT_TOSTRING(str_s, level);

        str_s << "ProgramHead: " << id_ << std::endl;
        if (id_list_ != nullptr)
        {
            str_s << id_list_->ToString(level + 1);
        }
        return str_s.str();
    }

    // eg. ProgramBody:
    //         ConstDeclaration: a
    //             1
    //         VarDeclaration:
    //             Type: integer
    //             IdList: a, b, c
    //         Subprogram:
    //             SubprogramHead: procedure a
    //             SubprogramBody:
    //                 CompoundStatement: 0
    //         CompoundStatement :0
    const string ProgramBody::ToString(const int level) const
    {
        INIT_TOSTRING(str_s, level);

        str_s << "ProgramBody: " << std::endl;
        for (int i = 0; i < const_declarations_.size(); i++)
        {
            str_s << const_declarations_[i]->ToString(level + 1) << std::endl;
        }

        for (int i = 0; i < var_declarations_.size(); i++)
        {
            str_s << var_declarations_[i]->ToString(level + 1) << std::endl;
        }

        for (int i = 0; i < subprogram_declarations_.size(); i++)
        {
            str_s << subprogram_declarations_[i]->ToString(level + 1) << std::endl;
        }

        str_s << statements_->ToString(level + 1) << std::endl;

        return str_s.str();
    }

    // eg. Program:
    //         ProgramHead: a
    //         ProgramBody:
    //             CompoundStatement: 0
    const string Program::ToString(const int level) const
    {
        INIT_TOSTRING(str_s, level);

        str_s << "Program: " << std::endl;
        str_s << program_head_->ToString(level + 1) << std::endl;
        str_s << program_body_->ToString(level + 1);
        return str_s.str();
    }
}
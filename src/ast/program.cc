#include <string>
#include <sstream>

#include "program.h"

extern "C"
{
#include "lexer.h"
}

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
        }
    }
    const string IdList::ToString(const int level) const
    {
        INIT_TOSTRING(str_s, level);

        str_s << "IdList: ";

        str_s << id_list_[0];
        for (int i = 1; i < id_list_.size(); i++)
        {
            str_s << ", " << id_list_[i];
        }
        return str_s.str();
    }

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

    const string ConstDeclaration::ToString(const int level) const
    {
        INIT_TOSTRING(str_s, level);

        str_s << "ConstDeclaration: " << id_ << std::endl;
        str_s << const_value_->ToString(level + 1);
        return str_s.str();
    }

    const string VarDeclaration::ToString(const int level) const
    {
        INIT_TOSTRING(str_s, level);

        str_s << "VarDeclaration: " << std::endl;
        str_s << type_->ToString(level + 1) << std::endl;
        str_s << id_list_->ToString(level + 1);
        return str_s.str();
    }

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

    const string SubprogramBody::ToString(const int level) const
    {
        INIT_TOSTRING(str_s, level);

        str_s << "SubprogramBody: " << std::endl;
        if (const_declarations_.size() > 0)
        {
            str_s << const_declarations_[0]->ToString(level + 1);
            for (int i = 1; i < const_declarations_.size(); i++)
            {
                str_s << std::endl
                      << const_declarations_[i]->ToString(level + 1);
            }
        }
        if (var_declarations_.size() > 0)
        {
            str_s << var_declarations_[0]->ToString(level + 1);
            for (int i = 1; i < var_declarations_.size(); i++)
            {
                str_s << std::endl
                      << var_declarations_[i]->ToString(level + 1);
            }
        }

        if (statement_list_.size() > 0)
        {
            str_s << statement_list_[0]->ToString(level + 1);
            for (int i = 1; i < statement_list_.size(); i++)
            {
                str_s << std::endl
                      << statement_list_[i]->ToString(level + 1);
            }
        }
        return str_s.str();
    }

    const string Subprogram::ToString(const int level) const
    {
        INIT_TOSTRING(str_s, level);

        str_s << "Subprogram: " << std::endl;
        str_s << subprogram_head_->ToString(level + 1) << std::endl;
        str_s << subprogram_body_->ToString(level + 1);
        return str_s.str();
    }

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

    const string ProgramBody::ToString(const int level) const
    {
        INIT_TOSTRING(str_s, level);

        str_s << "ProgramBody: " << std::endl;
        if (const_declarations_.size() > 0)
        {
            str_s << const_declarations_[0]->ToString(level + 1);
            for (int i = 1; i < const_declarations_.size(); i++)
            {
                str_s << std::endl
                      << const_declarations_[i]->ToString(level + 1);
            }
        }
        if (var_declarations_.size() > 0)
        {
            str_s << var_declarations_[0]->ToString(level + 1);
            for (int i = 1; i < var_declarations_.size(); i++)
            {
                str_s << std::endl
                      << var_declarations_[i]->ToString(level + 1);
            }
        }
        if (subprogram_declarations_.size() > 0)
        {
            str_s << subprogram_declarations_[0]->ToString(level + 1);
            for (int i = 1; i < subprogram_declarations_.size(); i++)
            {
                str_s << std::endl
                      << subprogram_declarations_[i]->ToString(level + 1);
            }
        }
        if (statement_list_.size() > 0)
        {
            str_s << statement_list_[0]->ToString(level + 1);
            for (int i = 1; i < statement_list_.size(); i++)
            {
                str_s << std::endl
                      << statement_list_[i]->ToString(level + 1);
            }
        }
        return str_s.str();
    }

    const string Program::ToString(const int level) const
    {
        INIT_TOSTRING(str_s, level);

        str_s << "Program: " << std::endl;
        str_s << program_head_->ToString(level + 1) << std::endl;
        str_s << program_body_->ToString(level + 1);
        return str_s.str();
    }
}
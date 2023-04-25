//
// Created by 谢卫凯 on 2023/3/20.
//
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "expr.h"

extern "C"
{
#include "lexer.h"
}

namespace pascal2c
{

    static int Op(int token)
    {
        static bool inited = false;
        static std::unordered_map<int,int> op;
        if(!inited){
            for(int i = 0;i < 500;i ++)
                op[i] = i;
            op['+'] = '+';
            op['-'] = '-';
            op['*'] = '*';
            op['/'] = '/';
            op[TOK_NEQOP] = 'N';
            op[TOK_LEOP] = 'L';
            op[TOK_GEOP] = 'G';
            op[TOK_OR] = 'o';
            op[TOK_AND] = 'a';
            op[TOK_MOD] = 'm';
            op[TOK_DIV] = 'd';
            op[TOK_NOT] = 'n';
            inited = true;
        }
        return op[token];
    }

    void ast::CallValue::AddParam(std::shared_ptr<Expression> expr)
    {
        params_.push_back(std::move(expr));
    }

    void ast::Variable::AddExpr(std::shared_ptr<Expression> expr)
    {
        expr_list_.push_back(std::move(expr));
    }

    std::string ast::CallValue::ToString(int level) const
    {
        std::stringstream str_s;
        IndentOutput(str_s, level);
        str_s << "function:" << id_;

        for (int i = 0; i < params_.size(); i++)
        {
            str_s << "\n";
            IndentOutput(str_s, level);
            str_s << "expr " << i + 1 << ":\n"
                  << params_[i]->ToString(level + 1);
        }
        return str_s.str();
    }

    std::string ast::Variable::ToString(int level) const
    {
        std::stringstream str_s;
        IndentOutput(str_s, level);
        str_s << "variable:" << id_;

        for (int i = 0; i < expr_list_.size(); i++)
        {
            str_s << "\n";
            IndentOutput(str_s, level);
            str_s << "index " << i + 1 << ":\n"
                  << expr_list_[i]->ToString(level + 1);
        }
        return str_s.str();
    }

    std::string ast::IntegerValue::ToString(int level) const
    {
        std::stringstream str_s;
        IndentOutput(str_s, level);
        str_s << value_;
        return str_s.str();
    }

    std::string ast::RealValue::ToString(int level) const
    {
        std::stringstream str_s;
        IndentOutput(str_s, level);
        str_s << std::fixed << std::setprecision(4) << value_ << std::defaultfloat;
        return str_s.str();
    }

    std::string ast::CharValue::ToString(int level) const
    {
        char ch = (char)ch_;
        std::stringstream str_s;
        IndentOutput(str_s, level);
        str_s << '\'' << ch << '\'';
        return str_s.str();
    }

    std::string ast::BooleanValue::ToString(int level) const
    {
        std::stringstream str_s;
        IndentOutput(str_s, level);
        if(value_)
            str_s << "true";
        else
            str_s << "false";
        return str_s.str();
    }

    std::string ast::BinaryExpr::ToString(int level) const
    {
        std::stringstream str_s;
        IndentOutput(str_s, level);
        str_s << "binary_op:" << '\'' << (char)Op(op_) << '\'' << "\n";
        IndentOutput(str_s, level);
        str_s << "lhs :\n";
        str_s << lhs_->ToString(level + 1) << "\n";
        IndentOutput(str_s, level);
        str_s << "rhs :\n";
        str_s << rhs_->ToString(level + 1);
        return str_s.str();
    }

    std::string ast::UnaryExpr::ToString(int level) const
    {
        std::stringstream str_s;
        IndentOutput(str_s, level);
        str_s << "unary_op:" << '\'' << (char)Op(op_) << '\'' << "\n";
        IndentOutput(str_s, level);
        str_s << "expr :\n"
              << factor_->ToString(level + 1);
        return str_s.str();
    }

    std::string ast::CallOrVar::ToString(int level) const {
        std::stringstream str_s;
        IndentOutput(str_s, level);
        str_s << "CallOrVar: " << id_;

        return str_s.str();
    }

    std::string ast::StringValue::ToString(int level) const {
        std::stringstream str_s;
        IndentOutput(str_s, level);
        str_s << "string: " << value_;

        return str_s.str();
    }
}

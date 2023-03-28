//
// Created by 谢卫凯 on 2023/3/24.
//

#include <string>
#include <iomanip>
#include <sstream>

#include "statement.h"

namespace pascal2c::ast {

    static void IndentOutput(std::stringstream &str_s, int level) {
        while (level-- > 0)
            str_s << "    ";
    }

    std::string AssignStatement::ToString(int level) const {
        std::stringstream str_s;
        IndentOutput(str_s,level);
        str_s << "AssignStatement :\n";
        IndentOutput(str_s,level);
        str_s << "Variable:\n" << var_->ToString(level + 1) << "\n";
        IndentOutput(str_s,level);
        str_s << "Expr :\n" << expr_->ToString(level + 1);
        return str_s.str();
    }

    std::string CallStatement::ToString(int level) const {
        std::stringstream str_s;
        IndentOutput(str_s,level);
        str_s << "CallStatement :\n";
        IndentOutput(str_s,level);
        str_s << "name:" << name_;

        for (int i = 0; i < expr_list_.size(); i++) {
            str_s << "\n";
            IndentOutput(str_s, level);
            str_s << "expr " << i + 1 << ":\n" << expr_list_[i]->ToString(level + 1);
        }

        return str_s.str();
    }

    // TODO
    std::string CompoundStatement::ToString(int level) const {
        std::stringstream str_s;
        IndentOutput(str_s,level);
        str_s << "CompoundStatement :" << statements_.size();
        for(int i = 0; i < statements_.size();i ++){
            str_s << "\n";
            IndentOutput(str_s,level);
            str_s << "statement :" << i + 1 << "\n" << statements_[i]->ToString(level + 1);
        }
        return str_s.str();
    }

    // TODO
    std::string IfStatement::ToString(int level) const {
        std::stringstream str_s;
        IndentOutput(str_s,level);
        str_s << "IfStatement :\n";
        IndentOutput(str_s,level);
        str_s << "if_part:\n" << then_->ToString(level + 1);
        if(else_ != nullptr){
            str_s << "\n";
            IndentOutput(str_s,level);
            str_s << "else_part:\n" << else_->ToString(level + 1);
        }
        return str_s.str();
    }

    // TODO
    std::string ForStatement::ToString(int level) const {
        std::stringstream str_s;
        IndentOutput(str_s,level);
        str_s << "ForStatement:\n";
        IndentOutput(str_s,level);
        str_s << "from:\n" << from_->ToString(level + 1) << "\n";
        IndentOutput(str_s,level);
        str_s << "to:\n" << to_->ToString(level + 1) << "\n";
        IndentOutput(str_s,level);
        str_s << "do:\n" << statement_->ToString(level + 1);
        return str_s.str();
    }
}
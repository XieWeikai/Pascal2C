//
// Created by 谢卫凯 on 2023/3/16.
//

#ifndef PASCAL2C_STATEMENT_H
#define PASCAL2C_STATEMENT_H

#include <memory>
#include <vector>
#include <string>

#include "expr.h"

namespace pascal2c::ast {
    template<typename Tp> using vector = ::std::vector<Tp>;

    enum StatementType{
        ASSIGN_STATEMENT = 1,
        CALL_STATEMENT = 2,
        COMPOUND_STATEMENT = 3,
        IF_STATEMENT = 4,
        FOR_STATEMENT = 5,
        READ_STATEMENT = 6,
        WRITE_STATEMENT = 7,
    };

    // base type of statement
    class Statement {
    public:
        virtual std::string ToString(int level) const = 0;
        virtual StatementType GetType() const = 0;
    };

    // var_ := expr_
    // var_ can be: var_name    var_name[expr_list]  func_id
    class AssignStatement : public Statement {
    public:
        AssignStatement(std::shared_ptr<Variable> var,std::shared_ptr<Expression> expr) : var_(std::move(var)),expr_(
                std::move(expr)) {}

        inline StatementType GetType() const override { return ASSIGN_STATEMENT; }

        std::string ToString(int level) const override;
    private:
        std::shared_ptr<Variable> var_;
        std::shared_ptr<Expression> expr_;
    };

    // represents procedure and function call
    // name_ is procedure name or function name
    // expr_list_ is the arguments of procedure or function
    // expr_list_ can be empty
    class CallStatement : public Statement {
    public:
        CallStatement(std::string name, vector<std::shared_ptr<Expression> > expr_list) :
                name_(std::move(name)), expr_list_(std::move(expr_list)) {}

        explicit CallStatement(std::string name) : name_(std::move(name)) {}

        inline StatementType GetType() const override { return CALL_STATEMENT; }

        std::string ToString(int level) const override;
    private:
        std::string name_;

        vector<std::shared_ptr<Expression> > expr_list_; // can be empty:   procedure_name;   func();
    };

    // a series of statements
    // begin
    //    statement ;
    //    ...
    // end
    class CompoundStatement: public Statement {
    public:
        explicit CompoundStatement(vector<std::shared_ptr<Statement> > statements) : statements_(std::move(statements)) {}

        inline StatementType GetType() const override { return COMPOUND_STATEMENT; }
    private:
        vector<std::shared_ptr<Statement> > statements_;
    };

    // if condition_ then
    //      then_
    // else
    //      else_
    class IfStatement : public Statement {
    public:
        IfStatement(std::shared_ptr<Expression> cond,std::shared_ptr<Statement> then,
                    std::shared_ptr<Statement> else_part)
                    : condition_(std::move(cond)),then_(std::move(then)),else_(std::move(else_part)) {}

        inline StatementType GetType() const override { return IF_STATEMENT; }
    private:
        std::shared_ptr<Expression> condition_;
        std::shared_ptr<Statement>  then_;
        std::shared_ptr<Statement>  else_;
    };

    // for id_ := from_ to to_ do statement_
    class ForStatement : public Statement {
    public:
        ForStatement(std::shared_ptr<Expression> from,std::shared_ptr<Expression> to,
                     std::shared_ptr<Statement>  statement)
                     : from_(std::move(from)),to_(std::move(to)),statement_(std::move(statement)) {}

        inline StatementType GetType() const override { return FOR_STATEMENT; }
    private:
        std::string id_;
        std::shared_ptr<Expression> from_;
        std::shared_ptr<Expression> to_;
        std::shared_ptr<Statement>  statement_;
    };

//    // read(var_list_)
//    class ReadStatement : public Statement {
//    public:
//        explicit ReadStatement(vector<std::shared_ptr<Variable> > var_list)
//            : var_list_(std::move(var_list)) {}
//
//        inline StatementType GetType() const override { return READ_STATEMENT; }
//    private:
//        vector<std::shared_ptr<Variable> > var_list_;
//    };
//
//    // write(expr_list_)
//    class WriteStatement : public Statement {
//    public:
//        explicit WriteStatement(vector<std::shared_ptr<Expression> > expr_list)
//            : expr_list_(std::move(expr_list)) {}
//
//        inline StatementType GetType() const override { return WRITE_STATEMENT; }
//    private:
//        vector<std::shared_ptr<Expression> > expr_list_;
//    };

}

#endif //PASCAL2C_STATEMENT_H

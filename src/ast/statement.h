//
// Created by 谢卫凯 on 2023/3/16.
//

#ifndef PASCAL2C_STATEMENT_H
#define PASCAL2C_STATEMENT_H

#include <memory>
#include <vector>
#include <string>

#include "ast.h"
#include "expr.h"

namespace pascal2c::ast
{
    template <typename Tp>
    using vector = ::std::vector<Tp>;

    // to identify what type a statement is
    enum StatementType
    {
        ASSIGN_STATEMENT = 1,
        CALL_STATEMENT = 2,
        COMPOUND_STATEMENT = 3,
        IF_STATEMENT = 4,
        FOR_STATEMENT = 5,
    };

    // base type of statement
    class Statement : public Ast
    {
    public:
        // for test use
        // param:
        //     level is the level of indentation that should be applied to the returned string
        // return:
        //     a string represents the statement
        virtual std::string ToString(int level) const = 0;
        // to get exact statement type of the statement
        // return:
        //     exact type of statement
        virtual StatementType GetType() const = 0;
    };

    // var_ := expr_
    // var_ can be: var_name    var_name[expr_list]  func_id
    class AssignStatement : public Statement
    {
    public:
        // basic constructor
        // param:
        //     var is used to initialize the class member var_
        //     expr is used to initialize the class member expr_
        AssignStatement(std::shared_ptr<Variable> var, std::shared_ptr<Expression> expr) : var_(std::move(var)), expr_(
                                                                                                                     std::move(expr)) {}

        inline StatementType GetType() const override { return ASSIGN_STATEMENT; }

        std::string ToString(int level) const override;

        GETTER(std::shared_ptr<Variable>, var);
        GETTER(std::shared_ptr<Expression>, expr);

    private:
        std::shared_ptr<Variable> var_;    // lhs of the assign statement
        std::shared_ptr<Expression> expr_; // rhs of the assign statement
    };

    // represents procedure and function call
    // name_ is procedure name or function name
    // expr_list_ is the arguments of procedure or function
    // expr_list_ can be empty
    // write and read are treated as CallStatement as built-in function
    class CallStatement : public Statement
    {
    public:
        CallStatement(std::string name, vector<std::shared_ptr<Expression>> expr_list) : name_(std::move(name)), expr_list_(std::move(expr_list)) {}

        explicit CallStatement(std::string name) : name_(std::move(name)) {}

        inline StatementType GetType() const override { return CALL_STATEMENT; }

        std::string ToString(int level) const override;

        GETTER(std::string, name);
        GETTER(vector<std::shared_ptr<Expression>>, expr_list);

    private:
        std::string name_; // procedure name or function name

        vector<std::shared_ptr<Expression>> expr_list_; // can be empty:  e.g. procedure_name;   func();
    };

    // a series of statements
    // begin
    //    statement1 ;
    //    statement2 ;
    //    ...
    // end
    class CompoundStatement : public Statement
    {
    public:
        explicit CompoundStatement(vector<std::shared_ptr<Statement>> statements) : statements_(std::move(statements)) {}

        inline StatementType GetType() const override { return COMPOUND_STATEMENT; }

        std::string ToString(int level) const override;

        GETTER(vector<std::shared_ptr<Statement>>, statements);

    private:
        vector<std::shared_ptr<Statement>> statements_; // vector of statement
    };

    // if condition_ then
    //      then_
    // else
    //      else_part_
    class IfStatement : public Statement
    {
    public:
        IfStatement(std::shared_ptr<Expression> cond, std::shared_ptr<Statement> then,
                    std::shared_ptr<Statement> else_part)
            : condition_(std::move(cond)), then_(std::move(then)), else_part_(std::move(else_part)) {}

        inline StatementType GetType() const override { return IF_STATEMENT; }

        std::string ToString(int level) const override;

        GETTER(std::shared_ptr<Expression>, condition);
        GETTER(std::shared_ptr<Statement>, then);
        GETTER(std::shared_ptr<Statement>, else_part);

    private:
        std::shared_ptr<Expression> condition_; // condition expression
        std::shared_ptr<Statement> then_;       // then part of if statement
        std::shared_ptr<Statement> else_part_;       // else part of if statement, can be empty
    };

    // for id_ := from_ to to_ do statement_
    class ForStatement : public Statement
    {
    public:
        ForStatement(std::shared_ptr<Expression> from, std::shared_ptr<Expression> to,
                     std::shared_ptr<Statement> statement)
            : from_(std::move(from)), to_(std::move(to)), statement_(std::move(statement)) {}

        inline StatementType GetType() const override { return FOR_STATEMENT; }

        std::string ToString(int level) const override;

        GETTER(std::string, id);
        GETTER(std::shared_ptr<Expression>, from);
        GETTER(std::shared_ptr<Expression>, to);
        GETTER(std::shared_ptr<Statement>, statement);

    private:
        std::string id_;
        std::shared_ptr<Expression> from_;
        std::shared_ptr<Expression> to_;
        std::shared_ptr<Statement> statement_;
    };
}

#endif // PASCAL2C_STATEMENT_H

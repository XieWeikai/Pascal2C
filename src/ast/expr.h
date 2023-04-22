//
// Created by 谢卫凯 on 2023/3/16.
//

#ifndef PASCAL2C_EXPR_H
#define PASCAL2C_EXPR_H

#include <utility>
#include <vector>
#include <memory>
#include <string>

#include "ast.h"

namespace pascal2c::ast
{
    template <typename Tp>
    using vector = ::std::vector<Tp>;

    enum ExprType
    {
        INT = 0,
        REAL = 1,
        CHAR = 2,
        BOOLEAN = 3,
        CALL_OR_VAR = 4,
        VARIABLE = 5,
        CALL = 6,
        BINARY = 7,
        UNARY = 8,
    };

    // base class for expression
    class Expression : public Ast
    {
    public:
        // for test use
        // param:
        //     level is the level of indentation that should be applied to the returned string
        // return:
        //     a string represents the statement
        virtual std::string ToString(int level) const = 0;

        // get the exact type of the expression
        // return:
        //     one of ExprType
        virtual ExprType GetType() const = 0;
    };

    // leaf node of an expression
    // this node represents an integer value in pascal
    // e.g. 123  482
    // value_ stores the value of the integer
    class IntegerValue : public Expression
    {
    public:
        explicit IntegerValue(int value) : value_(value) {}

        std::string ToString(int level) const override;
        inline ExprType GetType() const override { return INT; }

    private:
        int value_;
    };

    // leaf node of an expression
    // this node represents a real value in pascal
    // e.g. 12.34
    // value_ stores the value of the real number
    class RealValue : public Expression
    {
    public:
        explicit RealValue(double value) : value_(value) {}

        std::string ToString(int level) const override;
        inline ExprType GetType() const override { return REAL; }

    private:
        double value_;
    };

    // leaf node
    // represent a character
    // e.g. 'a'
    // ch_ store the ascii code of the character
    class CharValue : public Expression
    {
    public:
        explicit CharValue(int ch) : ch_(ch) {}

        std::string ToString(int level) const override;
        inline ExprType GetType() const override { return CHAR; }

    private:
        int ch_;
    };

    // leaf node
    // represent a boolean value
    // e.g. true  false
    // value_ store the value
    class BooleanValue : public Expression
    {
    public:
        explicit BooleanValue(bool value) : value_(value) {}

        std::string ToString(int level) const override;
        inline ExprType GetType() const override { return BOOLEAN; }

    private:
        bool value_;
    };

    // when we met some statement like var1 := A;
    // we don't know whether var1 is a variable or a function call,
    // so we use this node to represent it
    class CallOrVar : public Expression
    {
    public:
        CallOrVar(std::string id) : id_(std::move(id)) {}

        std::string ToString(int level) const override;
        inline virtual ExprType GetType() const override { return CALL_OR_VAR; }
    protected:
        // id_ is the name of the variable or function
        // in the example of var1 := A;
        // id_ is "var1"
        // and we do not need an expression list since no matter
        // it is a variable or a function call, it has no parameters
        std::string id_;
    };

    // represent a function call
    // e.g. add(3+4,5)
    // params_ is a vector of expressions used as parameters of the function
    // params_ can be empty
    class CallValue : public CallOrVar
    {
    public:
        explicit CallValue(std::string func_name) : CallOrVar(std::move(func_name)) {}
        CallValue(std::string func_name, vector<std::shared_ptr<Expression>> params) : CallOrVar(std::move(func_name)) , params_(std::move(params)) {}
        void AddParam(std::shared_ptr<Expression> expr);

        std::string ToString(int level) const override;
        inline ExprType GetType() const override { return CALL; }

    private:
        // id is defined in CallOrValue base class
        // std::string id_;
        vector<std::shared_ptr<Expression>> params_;
    };

    // represent variable in pascal
    // e.g. count  num[3+i]  pos[3,4]
    // in the example of pos[3,4] the elements of expr_list_ is 3 and 4
    // expr_list_ can be empty
    class Variable : public CallOrVar
    {
    public:
        explicit Variable(std::string id) : CallOrVar(std::move(id)) {}
        Variable(std::string id, vector<std::shared_ptr<Expression>> expr_list) : CallOrVar(std::move(id)) , expr_list_(
                                                                                                          std::move(expr_list)) {}

        void AddExpr(std::shared_ptr<Expression> expr);

        std::string ToString(int level) const override;
        inline ExprType GetType() const override { return VARIABLE; }

    private:
        // id is defined in CallOrValue base class
        // std::string id_;
        vector<std::shared_ptr<Expression>> expr_list_;
    };

    // represent binary expression
    // e.g. 3 + 4 + 5    3 * ((6 + 7) - 2)
    // in the example of 3 + 4 + 5, lhs_ is 3 + 4 ,rhs_ is 5 and op_ is '+'
    class BinaryExpr : public Expression
    {
    public:
        BinaryExpr(int op, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs) : op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

        std::string ToString(int level) const override;
        inline ExprType GetType() const override { return BINARY; }

    private:
        int op_;                                // operator
        std::shared_ptr<Expression> lhs_, rhs_; // two operands
    };

    // represent unary expression
    // e.g. -3
    // in the above example, op_ is '-' and factor is 3
    class UnaryExpr : public Expression
    {
    public:
        UnaryExpr(int op, std::shared_ptr<Expression> factor) : op_(op), factor_(std::move(factor)) {}

        std::string ToString(int level) const override;
        inline ExprType GetType() const override { return UNARY; }

    private:
        int op_;
        std::shared_ptr<Expression> factor_;
    };
}

#endif // PASCAL2C_EXPR_H

//
// Created by 谢卫凯 on 2023/3/20.
//

#include <gtest/gtest.h>
#include <iostream>
#include <iomanip>

#include "ast/expr.h"
#include "ast/statement.h"

using namespace pascal2c;

TEST(TestToString,TestInteger) {
    auto integer = std::make_shared<ast::IntegerValue>(123);
    ast::Expression *ep = integer.get();
    EXPECT_EQ(ep->ToString(0),"123");
    EXPECT_EQ(ep->ToString(1),"    123");
    EXPECT_EQ(ep->ToString(2),"        123");
}

TEST(TestToString,TestReal) {
    auto integer = std::make_shared<ast::RealValue>(123.123);
    ast::Expression *ep = integer.get();
    EXPECT_EQ(ep->ToString(0),"123.1230");
    EXPECT_EQ(ep->ToString(1),"    123.1230");
    EXPECT_EQ(ep->ToString(2),"        123.1230");
}

TEST(TestToString,TestChar) {
    auto integer = std::make_shared<ast::CharValue>('*');
    ast::Expression *ep = integer.get();
    EXPECT_EQ(ep->ToString(0),"'*'");
    EXPECT_EQ(ep->ToString(1),"    '*'");
    EXPECT_EQ(ep->ToString(2),"        '*'");
}

TEST(TestToString,TestBinaryOp) {
    auto expr1 = std::make_shared<ast::IntegerValue>(3);
    auto expr2 = std::make_shared<ast::IntegerValue>(4);
    auto expr3 = std::make_shared<ast::IntegerValue>(5);
    auto binary_op = std::make_shared<ast::BinaryExpr>('+',expr1,expr2);
    binary_op = std::make_shared<ast::BinaryExpr>('*',binary_op,expr3);
    ast::Expression *ep = binary_op.get();
    auto res = R"(binary_op:'*'
lhs :
    binary_op:'+'
    lhs :
        3
    rhs :
        4
rhs :
    5)";

    EXPECT_EQ(ep->ToString(0),res);
}

TEST(TestToString,TestUnaryOp) {
    auto expr1 = std::make_shared<ast::IntegerValue>(3);
    auto expr2 = std::make_shared<ast::IntegerValue>(4);
    auto expr3 = std::make_shared<ast::IntegerValue>(5);
    auto binary_op = std::make_shared<ast::BinaryExpr>('+',expr1,expr2);
    binary_op = std::make_shared<ast::BinaryExpr>('*',binary_op,expr3);
    auto unary_op = std::make_shared<ast::UnaryExpr>('-',binary_op);
    ast::Expression *ep = unary_op.get();
    auto res =
            R"(unary_op:'-'
expr :
    binary_op:'*'
    lhs :
        binary_op:'+'
        lhs :
            3
        rhs :
            4
    rhs :
        5)";

    EXPECT_EQ(ep->ToString(0),res);
}

TEST(TestToString,TestVariable) {
    auto var1 = std::make_shared<ast::Variable>("count");
    auto var2 = std::make_shared<ast::Variable>("num");
    auto expr1 = std::make_shared<ast::BinaryExpr>('+',var1, std::make_shared<ast::IntegerValue>(3));
    auto expr2 = std::make_shared<ast::BinaryExpr>('+',var1, std::make_shared<ast::IntegerValue>(4));
    var2->AddExpr(expr1);
    var2->AddExpr(expr2);

    auto res1 = R"(variable:count)";
    auto res2 =
R"(variable:num
index 1:
    binary_op:'+'
    lhs :
        variable:count
    rhs :
        3
index 2:
    binary_op:'+'
    lhs :
        variable:count
    rhs :
        4)" ;

    EXPECT_EQ(var1->ToString(0),res1);
    EXPECT_EQ(var2->ToString(0),res2);
}

TEST(TestToString,CallValue) {
    auto var1 = std::make_shared<ast::Variable>("count");
    auto add_func = std::make_shared<ast::CallValue>("add");
    auto expr1 = std::make_shared<ast::BinaryExpr>('+',var1, std::make_shared<ast::IntegerValue>(3));
    auto expr2 = std::make_shared<ast::BinaryExpr>('+',var1, std::make_shared<ast::IntegerValue>(4));
    add_func->AddParam(expr1);
    add_func->AddParam(expr2);

    auto res =
            R"(function:add
expr 1:
    binary_op:'+'
    lhs :
        variable:count
    rhs :
        3
expr 2:
    binary_op:'+'
    lhs :
        variable:count
    rhs :
        4)" ;

    EXPECT_EQ(add_func->ToString(0),res);
}

TEST(TestToString,TestAssignStatement){
    auto var = std::make_shared<ast::Variable>("i");
    auto expr = std::make_shared<ast::BinaryExpr>(
            '+', std::make_shared<ast::Variable>("i"), std::make_shared<ast::IntegerValue>(1));
    std::shared_ptr<ast::Statement> statement = std::make_shared<ast::AssignStatement>(var,expr);

    auto res = R"(AssignStatement :
Variable:
    variable:i
Expr :
    binary_op:'+'
    lhs :
        variable:i
    rhs :
        1)";

    EXPECT_EQ(statement->ToString(0),res);
}

TEST(TestToString,TestCallStatement) {
    std::vector<std::shared_ptr<ast::Expression> > expr_list;
    expr_list.push_back(std::move(std::make_shared<ast::Variable>("a")));
    expr_list.push_back(std::move(std::make_shared<ast::Variable>("b")));
    std::string name = "add";

    auto call1 = std::make_shared<ast::CallStatement>("sayhello");
    auto call2 = std::make_shared<ast::CallStatement>(name,expr_list);

    auto res1 = R"(CallStatement :
name:sayhello)";
    auto res2 = R"(CallStatement :
name:add
expr 1:
    variable:a
expr 2:
    variable:b)";

    EXPECT_EQ(call1->ToString(0),res1);
    EXPECT_EQ(call2->ToString(0),res2);
}

TEST(TestToString,TestCompoundStatement) {
    auto var = std::make_shared<ast::Variable>("i");
    auto expr = std::make_shared<ast::BinaryExpr>(
            '+', std::make_shared<ast::Variable>("i"), std::make_shared<ast::IntegerValue>(1));
    std::shared_ptr<ast::Statement> assign = std::make_shared<ast::AssignStatement>(var,expr);

    std::vector<std::shared_ptr<ast::Expression> > expr_list;
    expr_list.push_back(std::move(std::make_shared<ast::Variable>("a")));
    expr_list.push_back(std::move(std::make_shared<ast::Variable>("b")));
    std::string name = "add";
    auto call = std::make_shared<ast::CallStatement>(name,expr_list);

    std::vector<std::shared_ptr<ast::Statement> > statements = {assign,call};

    auto compound = std::make_shared<ast::CompoundStatement>(statements);
    auto res =
            "CompoundStatement :2\n"
            "statement :1\n"
            "    AssignStatement :\n"
            "    Variable:\n"
            "        variable:i\n"
            "    Expr :\n"
            "        binary_op:'+'\n"
            "        lhs :\n"
            "            variable:i\n"
            "        rhs :\n"
            "            1\n"
            "statement :2\n"
            "    CallStatement :\n"
            "    name:add\n"
            "    expr 1:\n"
            "        variable:a\n"
            "    expr 2:\n"
            "        variable:b";
    EXPECT_EQ(compound->ToString(0),res);
}

TEST(TestToString,TestIfStatement) {
    auto expr = std::make_shared<ast::BinaryExpr>('<', std::make_shared<ast::IntegerValue>(3),
                                                  std::make_shared<ast::IntegerValue>(4));
    auto var = std::make_shared<ast::Variable>("i");
    auto cond = std::make_shared<ast::BinaryExpr>(
            '+', std::make_shared<ast::Variable>("i"), std::make_shared<ast::IntegerValue>(1));
    std::shared_ptr<ast::Statement> assign = std::make_shared<ast::AssignStatement>(var,expr);

    std::vector<std::shared_ptr<ast::Expression> > expr_list;
    expr_list.push_back(std::move(std::make_shared<ast::Variable>("a")));
    expr_list.push_back(std::move(std::make_shared<ast::Variable>("b")));
    std::string name = "add";
    auto call = std::make_shared<ast::CallStatement>(name,expr_list);

    std::vector<std::shared_ptr<ast::Statement> > statements = {assign,call};

    auto compound = std::make_shared<ast::CompoundStatement>(statements);

    auto if_statement = std::make_shared<ast::IfStatement>(cond,compound, nullptr);
    auto call2 = std::make_shared<ast::CallStatement>("sayhello");
    auto res =
            "IfStatement :\n"
            "if_part:\n"
            "    CompoundStatement :2\n"
            "    statement :1\n"
            "        AssignStatement :\n"
            "        Variable:\n"
            "            variable:i\n"
            "        Expr :\n"
            "            binary_op:'<'\n"
            "            lhs :\n"
            "                3\n"
            "            rhs :\n"
            "                4\n"
            "    statement :2\n"
            "        CallStatement :\n"
            "        name:add\n"
            "        expr 1:\n"
            "            variable:a\n"
            "        expr 2:\n"
            "            variable:b";
    EXPECT_EQ(if_statement->ToString(0),res);

    if_statement = std::make_shared<ast::IfStatement>(cond,compound,call2);
    res =
            "IfStatement :\n"
            "if_part:\n"
            "    CompoundStatement :2\n"
            "    statement :1\n"
            "        AssignStatement :\n"
            "        Variable:\n"
            "            variable:i\n"
            "        Expr :\n"
            "            binary_op:'<'\n"
            "            lhs :\n"
            "                3\n"
            "            rhs :\n"
            "                4\n"
            "    statement :2\n"
            "        CallStatement :\n"
            "        name:add\n"
            "        expr 1:\n"
            "            variable:a\n"
            "        expr 2:\n"
            "            variable:b\n"
            "else_part:\n"
            "    CallStatement :\n"
            "    name:sayhello";
    EXPECT_EQ(if_statement->ToString(0),res);
}

TEST(TestToString,TestForStatement) {
    std::string id = "i";
    auto from = std::make_shared<ast::IntegerValue>(1);
    auto to = std::make_shared<ast::IntegerValue>(10);

    auto expr = std::make_shared<ast::BinaryExpr>('<', std::make_shared<ast::IntegerValue>(3),
                                                  std::make_shared<ast::IntegerValue>(4));
    auto var = std::make_shared<ast::Variable>("i");
    auto cond = std::make_shared<ast::BinaryExpr>(
            '+', std::make_shared<ast::Variable>("i"), std::make_shared<ast::IntegerValue>(1));
    std::shared_ptr<ast::Statement> assign = std::make_shared<ast::AssignStatement>(var,expr);

    std::vector<std::shared_ptr<ast::Expression> > expr_list;
    expr_list.push_back(std::move(std::make_shared<ast::Variable>("a")));
    expr_list.push_back(std::move(std::make_shared<ast::Variable>("b")));
    std::string name = "add";
    auto call = std::make_shared<ast::CallStatement>(name,expr_list);

    std::vector<std::shared_ptr<ast::Statement> > statements = {assign,call};

    auto compound = std::make_shared<ast::CompoundStatement>(statements);

    auto for_statement = std::make_shared<ast::ForStatement>(from,to,compound);
    auto res =
            "ForStatement:\n"
            "from:\n"
            "    1\n"
            "to:\n"
            "    10\n"
            "do:\n"
            "    CompoundStatement :2\n"
            "    statement :1\n"
            "        AssignStatement :\n"
            "        Variable:\n"
            "            variable:i\n"
            "        Expr :\n"
            "            binary_op:'<'\n"
            "            lhs :\n"
            "                3\n"
            "            rhs :\n"
            "                4\n"
            "    statement :2\n"
            "        CallStatement :\n"
            "        name:add\n"
            "        expr 1:\n"
            "            variable:a\n"
            "        expr 2:\n"
            "            variable:b";
    EXPECT_EQ(for_statement->ToString(0),res);
}

TEST(TestGetType,TestInteger){
    std::shared_ptr<ast::Expression> ep(new ast::IntegerValue(4));
    EXPECT_EQ(ep->GetType(),ast::INT);
}

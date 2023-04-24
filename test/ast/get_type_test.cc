//
// Created by 谢卫凯 on 2023/4/22.
//
#include <gtest/gtest.h>
#include <iostream>
#include <iomanip>

#include "ast/expr.h"
#include "ast/statement.h"

using namespace pascal2c;

TEST(TestGetType,TestInteger){
    std::shared_ptr<ast::Expression> ep(new ast::IntegerValue(4));
    EXPECT_EQ(ep->GetType(),ast::INT);
}

TEST(TestGetType,TestVariable){
    std::shared_ptr<ast::Expression> ep(new ast::Variable("a"));
    EXPECT_EQ(ep->GetType(),ast::VARIABLE);
}

TEST(TestGetType, TestCallValue){
    auto call = std::make_shared<ast::CallValue>("add");
    EXPECT_EQ(call->GetType(),ast::CALL);
}

TEST(TestGetType, TestCallOrVar){
    std::shared_ptr<ast::CallOrVar> call = std::make_shared<ast::CallOrVar>("add");
    EXPECT_EQ(call->GetType(),ast::CALL_OR_VAR);
}

#include "code_generation/optimizer/calculater.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace pascal2c::code_generation::Optimizer;
using namespace pascal2c::ast;

TEST(OptimizerTest , CalculatorTest) {
		auto _1 = std::make_shared<IntegerValue>(1);
		auto _2 = std::make_shared<IntegerValue>(2);
		auto _3 = std::make_shared<IntegerValue>(3);
		auto _4 = std::make_shared<IntegerValue>(4);
		auto _A = std::make_shared<Variable>("Aval");


		auto _1p2 = std::make_shared<BinaryExpr>('+' , _1 , _2);
		auto _3m4 = std::make_shared<BinaryExpr>('*' , _3 , _4);
		auto _1p2p3 = std::make_shared<BinaryExpr>('+' , _1p2 , _3);
		
		// 1 + 2 + 3 + 4;
		auto _1p2p3p4 = std::make_shared<BinaryExpr>('+' , _1p2p3 , _4);

		// 1 + 2 + 3 * 4;
		auto _1p2p3m4 = std::make_shared<BinaryExpr>('+' , _1p2 , _3m4);

		// A + 3 * 4;
		auto _Ap3m4 = std::make_shared<BinaryExpr>('+' , _A , _3m4);
		Calculator cal{_Ap3m4};

		auto res = cal.getResultExpr();

		ASSERT_NE(res , nullptr);

		switch(res->GetType()) {	
		case ExprType::INT : {
			auto ans = std::static_pointer_cast<IntegerValue>(res);
			EXPECT_EQ(ans->value() , 15);
			break;
		}
		default : 
			std::cerr << "STR" << res->ToString(0) << "\n";
			EXPECT_EQ(res->ToString(0) , 
				"1:1 binary_op:'+'\n"
				"lhs :\n"
				"    1:1 variable:Aval\n"
				"rhs :\n"
				"    1:1 12");
			break;
		}
}
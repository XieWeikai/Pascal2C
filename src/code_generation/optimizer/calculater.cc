#include "calculater.h"

namespace pascal2c::code_generation {
namespace Optimizer {

Calculator::Calculator(const ExprPtr& cur) {	
	auto [ptr , is_val_] = calc(cur);
	is_val = is_val_;

	if (is_val) 
			res_ptr = convertValueExpr(cur , cur);
	else 	res_ptr = ptr;


}

ExprPtr Calculator::convertValueExpr(const ExprPtr& cur , const ExprPtr& origin) {
	auto type_info = analysiser::GetExprType(origin).type();

	switch (type_info) {
	case symbol_table::INT :
		return std::make_shared<ast::IntegerValue>(
			static_cast<int>(
				std::static_pointer_cast<ast::RealValue>(cur)->value()));
	
	case symbol_table::CHAR :
		return std::make_shared<ast::CharValue>(
			static_cast<char>(
				std::static_pointer_cast<ast::RealValue>(cur)->value()));

	default:
		return cur;
	}

}

// template<typename T , typename V>
// auto test_op(int op , T a , V b) {
// 	switch (op) {
// 		case '+' : return a + b;
// 		case '-' : return a - b;
// 		case '*' : return a * b;
// 		case '/' : return a / b;
// 	};
// }
ResOfExpr Calculator::calcBinaryExpr(const std::shared_ptr<ast::BinaryExpr>& cur) {
	auto [l_ptr , l_is_val] = calc(cur->lhs());
	auto [r_ptr , r_is_val] = calc(cur->rhs());
	
	if ( !(l_is_val && r_is_val) ) {
		if (l_is_val) l_ptr = convertValueExpr(l_ptr , cur->lhs());
		if (r_is_val) r_ptr = convertValueExpr(r_ptr , cur->rhs());
		return {
			std::make_shared<ast::BinaryExpr>(cur->op() , l_ptr , r_ptr) , false};
	}
	
	double l_val = std::static_pointer_cast<ast::RealValue>(l_ptr)->value();
	double r_val = std::static_pointer_cast<ast::RealValue>(r_ptr)->value();

	switch (cur->op()) {
	case '+' : return {std::make_shared<ast::RealValue>(l_val + r_val) , true};
	case '-' : return {std::make_shared<ast::RealValue>(l_val - r_val) , true};
	case '*' : return {std::make_shared<ast::RealValue>(l_val * r_val) , true};
	case '/' : return {std::make_shared<ast::RealValue>(l_val / r_val) , true};
	};

	return {cur , false};
}

ResOfExpr Calculator::calcUnaryExpr(const std::shared_ptr<ast::UnaryExpr>& cur ) {
	auto [ptr  , is_val] = calc(cur->factor());

	if ( !is_val ) return {ptr  , false};

	double val = std::static_pointer_cast<ast::RealValue>(ptr)->value();

	switch (cur->op()) {
	case '-' : return {std::make_shared<ast::RealValue>(-1 * val) , true};
	}
	
	return {nullptr, false};
}


ResOfExpr Calculator::calc(const ExprPtr& cur) {

	switch(cur->GetType()) {
	case ast::ExprType::INT : 
		return {std::make_shared<ast::RealValue>(
			std::static_pointer_cast<ast::IntegerValue>(cur)->value()) , true};

	case ast::ExprType::CHAR : 
		return {std::make_shared<ast::RealValue>(
			std::static_pointer_cast<ast::CharValue>(cur)->ch()) , true};	

	case ast::ExprType::BOOLEAN :
		return {std::make_shared<ast::RealValue>(
			std::static_pointer_cast<ast::BooleanValue>(cur)->value()) , true};

	case ast::ExprType::REAL :
		return {cur , true};

	case ast::ExprType::BINARY :
		return calcBinaryExpr(std::static_pointer_cast<ast::BinaryExpr>(cur));

	case ast::ExprType::UNARY :
		return calcUnaryExpr(std::static_pointer_cast<ast::UnaryExpr>(cur));

	default :
		return {cur , false};
	}
}


} // End namespace
} // End namespace

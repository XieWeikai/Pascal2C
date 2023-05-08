#pragma once
#include <variant>

#include "opti_common.h"

namespace pascal2c::code_generation {
namespace Optimizer {

using ExprPtr = std::shared_ptr<ast::Expression>;
using ResData = std::variant<char , int , double>;
using ResOfExpr = std::tuple<ExprPtr , bool>;
using NodePtr = std::shared_ptr<ASTNode>;

class Calculator {
public :
	explicit Calculator(const ExprPtr& cur);
	
	ExprPtr getResultExpr() const {return res_ptr;}

	bool isConstVal() const {return is_val;}

private :
	ResOfExpr calc(const ExprPtr& cur);
	ResOfExpr calcBinaryExpr(const std::shared_ptr<ast::BinaryExpr>& cur);
	ResOfExpr calcUnaryExpr(const std::shared_ptr<ast::UnaryExpr>& cur);
	ExprPtr	  convertValueExpr(const ExprPtr& cur , const ExprPtr& origin);


	bool is_val = false;
	ExprPtr res_ptr = nullptr;
	NodePtr node_ptr = nullptr;
	symbol_table::ItemType res_type;

};

} // End Namespace
} // End Namespace
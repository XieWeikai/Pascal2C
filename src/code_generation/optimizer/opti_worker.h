#pragma once
#include <memory>


#include "ast/ast.h"
#include "ast/expr.h"
#include "ast/program.h"
#include "ast/statement.h"

namespace pascal2c::code_generation {

namespace Optimizer {

using NodePtr = std::shared_ptr<ast::Ast>;

class OptimizerWorker {

public :
	bool rotateProgram(NodePtr root);
	
private :
	void dispatch(NodePtr node);
	void pass(NodePtr cur);


};

} // End namespace

} // End namespace
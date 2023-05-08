#include "transformer.h"
#include "calculater.h"

namespace pascal2c::code_generation {

using ::std::shared_ptr;
using ::std::make_shared;


/**
 *  TOK_INTEGER_TYPE 	297
 *  TOK_REAL_TYPE 		298
 *  TOK_BOOLEAN_TYPE 	299
 *  TOK_CHAR_TYPE 		300
 *  TOK_STRING_TYPE 	301
*/
static const string TOKToCString(int tk) {
	switch (tk) {
	
	case '=' : return "=";
	case '+' : return "+";
	case '-' : return "-";
	case '*' : return "*";
	case '/' : return "/";
	case 297 : return "int";
	case 298 : return "double";
	case 299 : return "bool";
	case 300 : return "char";
	case 301 : return "string";
	}
	
	std::cerr << "Err tk : " << tk << "->" << char(tk)<<"\n";
	throw std::runtime_error{"[Transformer] unknown type"}; 
	return "unkown-type";
}

template<typename T>
static auto make_shared_token(const TokenType type , const string value = "" , const bool is_reference = false) {
	if constexpr (std::is_same_v<T , Var>) {
		return make_shared<T>(make_shared<Token>(type , value) , is_reference);
	
	} else {
		return make_shared<T>(make_shared<Token>(type , value));
	}
}

static string las_func_line;
static string las_func_name;
static string now_func_line;
static string now_func_name;


static std::tuple<bool , bool , bool , bool>
checkIdType(const string& id) {
	std::cerr << "start check " << now_func_line << "\n";
	std::shared_ptr<symbol_table::SymbolTableBlock> sym_block;
	auto table = analysiser::GetTable();
	table->Query(now_func_line , sym_block);
	std::cerr << "end check " << id << "\n";

	symbol_table::SymbolTableItem var_checker{
		symbol_table::ERROR , 
		id , true , false , 
		std::vector<symbol_table::SymbolTablePara>()};

	auto ans = sym_block->Query(var_checker);

	auto is_var  = var_checker.is_var();
	auto is_func = var_checker.is_func();
	bool is_ref  = false;
	bool is_ret  = var_checker.is_var() && var_checker.is_func();
	printf("This : Func %s ,Check Id %s:  [%d , %d , %d , %d]\n" ,
		now_func_name.c_str() ,id.c_str() , is_var , is_func , is_ref , is_ret);

	return {is_var , is_func , is_ref , is_ret};
}


// extern std::string nowblockName;

Transformer::Transformer(shared_ptr<ast::Ast> root) {
	auto program_handle = std::dynamic_pointer_cast<ast::Program>(root);
	if (program_handle == nullptr) {
		throw std::runtime_error{"[Transformer] bad root pointer"};
	}
	// if (nowblockName.size() == 0) {
	// 	throw std::runtime_error{"[Transformer] analysis::init() has not been executed."};
	// }

	ast_root = transProgram(program_handle);
}

shared_ptr<Program> Transformer::transProgram(shared_ptr<ast::Program> cur) {
	las_func_line = now_func_line;
	las_func_name = now_func_name;
	now_func_line = std::to_string(cur->program_head()->line());
	now_func_name = cur->program_head()->id();
	auto ret = make_shared<Program>(
		cur->program_head()->id() ,
		transBody<ast::ProgramBody>(cur->program_body())
	);
	now_func_line = las_func_line;
	now_func_name = las_func_name;

	return ret;
}

shared_ptr<ASTNode>
Transformer::transSubprogram(shared_ptr<ast::Subprogram> cur) {
	now_func_line = std::to_string(cur->subprogram_head()->line());
	now_func_name = cur->subprogram_head()->id();

	std::cerr << "Subpro@" << cur->line() <<" : " << cur->column() <<"\n";

	auto params = cur->subprogram_head()->parameters();
	vector<shared_ptr<Argument>> args;

	for(const auto& elem : params) {
		const auto& list = elem->id_list();
		for(size_t i = 0 ; i < list->Size() ; i++) {
			
			args.push_back(std::move(make_shared<Argument>(
				make_shared_token<Var> (TokenType::IDENTIFIER , (*list)[i]) ,
				make_shared_token<Type>(TokenType::RESERVED	  , TOKToCString(elem->type())),
				!elem->is_var() //TODO : need_test
			)));
		}
	}

	shared_ptr<ASTNode> ret;
	if (cur->subprogram_head()->is_function()) { // function
		std::cerr << "Function@" << cur->line() <<" : " << cur->column() <<"\n";

		ret = make_shared<Function>(
			cur->subprogram_head()->id() ,
			make_shared_token<Type>(
				TokenType::RESERVED , 
				TOKToCString(cur->subprogram_head()->return_type())
			) , args ,
			transBody<ast::SubprogramBody>(cur->subprogram_body())
		);

	} else { // procedure
		std::cerr << "Proce@" << cur->line() <<" : " << cur->column() <<"\n";

		ret = make_shared<Subprogram>(
			cur->subprogram_head()->id() , args ,
			transBody<ast::SubprogramBody>(cur->subprogram_body())
		);
	}

	now_func_line = las_func_line;
	now_func_name = las_func_name;
	return ret;
}

template<typename T>
shared_ptr<Block> 
Transformer::transBody(shared_ptr<T> body) {
	static_assert(std::is_same_v<T , ast::ProgramBody> ||
				  std::is_same_v<T , ast::SubprogramBody> ,
				"[Transformer] bad body type");
	std::cerr << "Body@" << body->line() <<" : " << body->column() <<"\n";


	vector<std::shared_ptr<ASTNode>> child;
	if constexpr (std::is_same_v<T , ast::ProgramBody>) {
		child.push_back(std::move(transStatement(body->statements())));
	} else {
		child.push_back(std::move(transStatement(body->statement_list())));
	}


	return make_shared<Block>(
		transDeclaration<T>(body) ,
		make_shared<Compound>(child)
	);
}


template<typename T>
shared_ptr<Declaration>
Transformer::transDeclaration(shared_ptr<T> body) {
	static_assert(std::is_same_v<T , ast::ProgramBody> ||
				  std::is_same_v<T , ast::SubprogramBody> ,
				"[Transformer] bad body type");
	std::cerr << "body@" << body->line() <<" : " << body->column() <<"\n";

	vector<shared_ptr<ASTNode>> decls;

	auto const_decl = body->const_declarations();
	auto var_decl   = body->var_declarations();


	for(const auto& elem : const_decl) {
		decls.push_back(std::move(transConstDeclaration(elem)));
	}

	for(const auto& elem : var_decl) {
		auto var_ret = transVarDeclaration(elem);
		decls.insert(decls.end() ,var_ret.begin() ,var_ret.end() );
	}

	// >>> check >>>
	for(const auto& elem : decls) {
		if (elem == nullptr) {
			throw std::runtime_error{"[Transformer] bad decl nullptr"};
		}
	}
	return make_shared<Declaration>(decls);

}


shared_ptr<Assignment>
Transformer::transConstDeclaration(shared_ptr<ast::ConstDeclaration> cur) {
	return make_shared<Assignment>(
		make_shared_token<Var>(TokenType::IDENTIFIER , cur->id() ) ,
		transExpression(cur->const_value())
	);
}


vector<shared_ptr<ASTNode>> 
Transformer::transVarDeclaration(shared_ptr<ast::VarDeclaration> cur) { 
	const auto& list = cur->id_list();

	vector<shared_ptr<ASTNode>> res; res.reserve(list->Size());
	
	if (cur->type()->is_array()) { // array decl

		for (size_t i = 0 ; i < list->Size() ; i++) {
			const auto& arr_type = cur->type();
			vector<std::pair<int , int>> perd; perd.reserve(arr_type->periods().size());

			for (const auto& elem : arr_type->periods()) {
				perd.push_back({elem.digits_1 , elem.digits_2});
			}

			res.push_back(std::move(make_shared<ArrayDeclaration>(
				make_shared<Array>(
					make_shared<Var>((*list)[i]) 
				) ,
				make_shared<ArrayType>(
					make_shared_token<Type>(
						TokenType::RESERVED , 
						TOKToCString(cur->type()->basic_type())
					) ,
					perd
				)
			)));
		}


	} else { // basic type decl

		for (size_t i = 0 ; i < list->Size() ; i++) {
			res.push_back(std::move(make_shared<VarDeclaration>(
				make_shared<Var>((*list)[i]) ,
				make_shared_token<Type>(
					TokenType::RESERVED , 
					TOKToCString(cur->type()->basic_type())
				)
			)));
		}
	}

	return res;

}

// void Transformer::DoSubprogram(ast::Subprogram x);
// symbol_table::SymbolTableItem DoSubprogramHead(ast::SubprogramHead x);
// void Transformer::DoSubprogramBody(ast::SubprogramBody x);

/**
 *  INT = 0,
	REAL = 1,
	CHAR = 2,
	BOOLEAN = 3,
	CALL_OR_VAR = 4,
	VARIABLE = 5,
	CALL = 6,
	BINARY = 7,
	UNARY = 8,
	STRING = 9,
*/
static shared_ptr<ASTNode> 
passExpr(shared_ptr<ast::Expression> cur) {

	switch (cur->GetType()) {
	case ast::ExprType::INT :
		return make_shared_token<Num>(
			TokenType::NUMBER ,
			std::to_string(
				std::static_pointer_cast<ast::IntegerValue>(cur)->value()
			)
		);

	case ast::ExprType::REAL :
		return make_shared_token<Real>(
			TokenType::NUMBER ,
			std::to_string(
				std::static_pointer_cast<ast::RealValue>(cur)->value()
			)
		);

	case ast::ExprType::CHAR : 
		return make_shared_token<Char>(
				TokenType::NUMBER ,
				std::to_string(
					std::static_pointer_cast<ast::CharValue>(cur)->ch()
				)
		);

	case ast::ExprType::BOOLEAN : 
		return make_shared_token<String>(
				TokenType::NUMBER ,
				std::static_pointer_cast<ast::BooleanValue>(cur)->value() ?
				"true" : "false"
		);

	case ast::ExprType::STRING : 
		return make_shared_token<String>(
				TokenType::STRING ,
				std::static_pointer_cast<ast::StringValue>(cur)->value()
		);
	
	
	case ast::ExprType::VARIABLE : {
		auto var = std::static_pointer_cast<ast::Variable>(cur);

		auto [_1 , _2 , is_ref , _4] = checkIdType(var->id());

		if (var->expr_list().size() == 0) { // basic type
			return make_shared<Var>( var->id() , is_ref);

		} else { // array type
			vector<shared_ptr<ASTNode>> indices; 
			indices.reserve(var->expr_list().size()); 

			for (const auto& elem : var->expr_list()) {
				indices.push_back(std::move(passExpr(elem)));
			}

			return make_shared<ArrayAccess>(
				make_shared<Array>(
					make_shared<Var>( var->id())
				),
				indices
			);
		}
	}


	case ast::ExprType::UNARY : {
		vector<shared_ptr<ASTNode>> child {
			make_shared_token<Oper> (TokenType::OPERATOR , "-"),
			passExpr(
				std::static_pointer_cast<ast::UnaryExpr>(cur)->factor()
			)
		};

		return make_shared<Compound>(child);
	}


	case ast::ExprType::BINARY : {
		auto bin_expr = std::static_pointer_cast<ast::BinaryExpr>(cur);

		return make_shared<BinaryOperation>(
			passExpr(bin_expr->lhs()) ,
			make_shared_token<Oper>(TokenType::OPERATOR , TOKToCString(bin_expr->op())) ,
			passExpr(bin_expr->rhs())
		);
	}

	case ast::ExprType::CALL : {
		auto callee = std::static_pointer_cast<ast::CallValue>(cur);
		vector<shared_ptr<ASTNode>> param; param.reserve(callee->params().size());

		for (const auto& elem : callee->params()) {
			param.push_back(std::move(passExpr(elem)));
		}

		return make_shared<FunctionCall>( callee->id() , param );
	}

	case ast::ExprType::CALL_OR_VAR : {
		auto callval = std::static_pointer_cast<ast::CallOrVar>(cur);
		auto [is_var , is_func , _ , is_ret] = checkIdType(callval->id());
		if (is_var || is_ret) { // include 'return' -> func := expr;
			return passExpr(make_shared<ast::Variable>(callval->line() , callval->column() , callval->id()));
		} else if (is_func) {
			return passExpr(make_shared<ast::CallValue>(
				callval->line() , callval->column() , callval->id()));
		}
	}

	}

	throw std::runtime_error{"[Transformer] unknown expr type"};
	return nullptr;
}



shared_ptr<ASTNode>
Transformer::transExpression(shared_ptr<ast::Expression> cur) {
	// Optimizer::Calculator calc{cur};
	// auto res = calc.getResultExpr();
	std::cerr << "Cur@" << cur->line() <<" : " << cur->column() <<"\n";

	return passExpr(cur);
}


/**
 *  enum StatementType
    {
        ASSIGN_STATEMENT 	= 1,
        CALL_STATEMENT 		= 2,
        COMPOUND_STATEMENT 	= 3,
        IF_STATEMENT 		= 4,
        FOR_STATEMENT 		= 5,
    };
*/
shared_ptr<ASTNode>
Transformer::transStatement(shared_ptr<ast::Statement> cur) {
	std::cerr << "Cur@" << cur->line() <<" : " << cur->column() <<"\n";
	switch (cur->GetType()) {

	case ast::StatementType::ASSIGN_STATEMENT :
		return transAssignStatement(std::static_pointer_cast<ast::AssignStatement>(cur));

	case ast::StatementType::CALL_STATEMENT : 
		return transCallStatement(std::static_pointer_cast<ast::CallStatement>(cur));

	case ast::StatementType::COMPOUND_STATEMENT :
		return transCompoundStatement(std::static_pointer_cast<ast::CompoundStatement>(cur));
	
	case ast::StatementType::IF_STATEMENT :
		return transIfStatement(std::static_pointer_cast<ast::IfStatement>(cur));
	
	case ast::StatementType::FOR_STATEMENT :
		return transForStatement(std::static_pointer_cast<ast::ForStatement>(cur));
	}

	throw std::runtime_error{"[Transformer] unknown statement\n"};
	return nullptr;
}

shared_ptr<Assignment> 
Transformer::transAssignStatement(shared_ptr<ast::AssignStatement> cur) {
	return make_shared<Assignment>(
		transExpression(cur->var()) ,
		transExpression(cur->expr())
	);
}

shared_ptr<Statement>
Transformer::transCallStatement(shared_ptr<ast::CallStatement> cur) {
	vector<shared_ptr<ASTNode>> param; param.reserve(cur->expr_list().size());

	for (const auto& elem : cur->expr_list()) {
		param.push_back(std::move(passExpr(elem)));
	}

	return make_shared<Statement>(
		make_shared<FunctionCall>( cur->name() , param )
	);
}

shared_ptr<Compound>
Transformer::transCompoundStatement(shared_ptr<ast::CompoundStatement> cur) {
	vector<shared_ptr<ASTNode>> states; states.reserve(cur->statements().size());

	for(const auto& elem : cur->statements()) {
		states.push_back(std::move(transStatement(elem)));
	}
	
	return make_shared<Compound>(states);
}

shared_ptr<IfStatement>
Transformer::transIfStatement(shared_ptr<ast::IfStatement> cur) {
	auto cond = transExpression(cur->condition());
	vector<shared_ptr<ASTNode>> then_branch_ {std::move(transStatement(cur->then()))};

	if (cur->else_part() == nullptr) {
		return make_shared<IfStatement>(cond , 
		make_shared<Compound>(then_branch_)
	 );
	}

    vector<shared_ptr<ASTNode>> else_branch_ {std::move(transStatement(cur->else_part()))};
	return make_shared<IfStatement>(cond , 
		make_shared<Compound>(then_branch_) ,
		make_shared<Compound>(else_branch_)
	 );
}

shared_ptr<ForStatement>
Transformer::transForStatement(shared_ptr<ast::ForStatement> cur) {
	vector<shared_ptr<ASTNode>> body {std::move(transStatement(cur->statement()))};
	auto from = transExpression(cur->from());
	auto to   = transExpression(cur->to()  );

	return make_shared<ForStatement>(
		make_shared_token<Var>(TokenType::IDENTIFIER , cur->id()),
		from , to ,
		make_shared<Compound>(body)
	);
}

} // End Namespace
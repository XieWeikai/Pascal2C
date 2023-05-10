#include "transformer.h"
#include "calculater.h"


namespace pascal2c::code_generation {

using ::std::shared_ptr;
using ::std::make_shared;
using ::std::optional;
using ::std::pair;
/**
 * 	TOK_AND				262
 *  TOK_DIV				267
 * 	TOK_MOD				279
 * 	TOK_OR 				283
 *  TOK_INTEGER_TYPE 	297
 *  TOK_REAL_TYPE 		298
 *  TOK_BOOLEAN_TYPE 	299
 *  TOK_CHAR_TYPE 		300
 *  TOK_STRING_TYPE 	301
 *  TOK_NEQOP 			304
 *	TOK_LEOP 			305
 *	TOK_GEOP 			306
*/
static const string ToCString(int tk) {
	switch (tk) {
	
	case '=' : return "=";
	case '+' : return "+";
	case '-' : return "-";
	case '*' : return "*";
	
    case '<' : return "<";
	case '>' : return ">";

	case 262 : return "&&";
	case 267 : return "div";
	case '/' : return "/";

	case 279 : return "%";
	case 283 : return "||";

	case symbol_table::ItemType::INT :
	case 297 : return "int";

	case symbol_table::ItemType::REAL :
	case 298 : return "double";

	case symbol_table::ItemType::BOOL :
	case 299 : return "bool";

	case symbol_table::ItemType::CHAR :
	case 300 : return "char";

	case symbol_table::ItemType::STRING :
	case 301 : return "string";

	case 304 : return "!=";
	case 305 : return "<=";
	case 306 : return ">=";

	case symbol_table::ItemType::VOID :
	return "void";
	}
	
	std::cerr << "Err token got : " << tk << "->" << char(tk)<<"\n";
	throw std::runtime_error{"[Transformer] unknown type"}; 
	return "unkown-type";
}

template<typename T>
static auto make_shared_token(
	const TokenType type , 
	const string value = "" , 
	const bool is_reference = false ,
	const bool is_return_val= false ,
	const VarType var_type = VarType::UNDEFINED) {
	if constexpr (std::is_same_v<T , Var>) {
		return make_shared<T>(make_shared<Token>(type , value) , is_reference , is_return_val , var_type);
	
	} else {
		return make_shared<T>(make_shared<Token>(type , value));
	}
}

static struct {
	string func_line;
	string func_name;
	shared_ptr<ast::Subprogram> func_node;
} las , now;

static std::unordered_map <string , // Scope Line Number, Name
		shared_ptr<ast::SubprogramHead>> func_name_table;


static auto getParamRefs(const string& func_name) {
	// TODO : override checker and cache
	std::bitset<k_max_parameters> is_refs; is_refs.reset();
	size_t idx = 0;
	auto func = func_name_table[func_name];
	if (func == nullptr) return is_refs;
	
	for (const auto& elem : func->parameters()) {
		const auto& list = elem->id_list();
		for (size_t i = 0 ; i < list->Size() ; i++) {
			if (elem->is_var()) is_refs.set(idx , true); idx++;
		}
	}
	return is_refs;
}

static size_t getParamIndex(shared_ptr<ast::SubprogramHead> node ,const string& id) {
	auto id_list = node->parameters();

	size_t idx = -1;
	for(const auto& elem : id_list) 
	for(size_t j = 0 ; j < elem->id_list()->Size() ; j++) {
		idx++;
		if ((*elem->id_list())[j] == id)
			return idx;
	}
	return -1ULL;
}

bool Transformer::checkIdTypeIfRef(const string& id) {
	if (now.func_node == nullptr) return false;

	auto func_checker = analysiser::SubprogramToItem(*now.func_node->subprogram_head());
	table->Query("1" , sym_block);
	auto func = sym_block->Query(func_checker);

	const auto& para = func_checker.para();
	if (para.size() == 0) return false;

	auto idx  = getParamIndex(now.func_node->subprogram_head() , id);
	return idx == -1ULL ? false : para[idx].is_var();
}
/**
 * @return is_val | is_func | is_ref | is_ret | is_const
*/
std::tuple<bool , bool , bool , bool , bool , symbol_table::ItemType>
Transformer::checkIdType(const string& id) {
	table->Query(now.func_line , sym_block);

	symbol_table::SymbolTableItem checker{
		symbol_table::ERROR , 
		id , true , true , 
		std::vector<symbol_table::SymbolTablePara>()};

	auto ret 	 = sym_block->Query(checker);
	bool is_var  = checker.is_var() && !checker.is_func();
	bool is_func =!checker.is_var() &&  checker.is_func();
	bool is_ret  = checker.is_var() &&  checker.is_func();
	bool is_const=!checker.is_var() && !checker.is_func();

	if (ret != saERRORS::NO_ERROR) {
		symbol_table::SymbolTableItem func_checker{
			symbol_table::ERROR , 
			id , false , true , 
			std::vector<symbol_table::SymbolTablePara>()};

		is_func = sym_block->Query(func_checker) == saERRORS::NO_ERROR;
		is_ret = false;
	}
	bool is_ref  = checkIdTypeIfRef(id); // ATTENTION : will change nametable!
	
	// ::fprintf(stderr , "I'm here : Func %s ,Check Id %s: [%d , %d , %d , %d , %d]\n Msg : %s\n" ,
	// 		now.func_name.c_str() ,id.c_str() ,
	// 		is_var , is_func , is_ref , is_ret , is_const , 
	// 		saERRORS::toString(ret).c_str());

	return {is_var , is_func , is_ref , is_ret , is_const , checker.type().type()};
}

optional<pair<symbol_table::ItemType , vector<pair<int , int>>>>
Transformer::checkArrayType(const string& id) {
	table->Query(now.func_line , sym_block);

	symbol_table::SymbolTableItem checker{
		symbol_table::MegaType(), 
		id , true , true , 
		std::vector<symbol_table::SymbolTablePara>()};
	auto ret 	 =  sym_block->Query(checker);

	if (checker.type().pointer().size() == 0) return std::nullopt;

	static string pattern{"\\d"};
	static std::regex rgx{pattern};
	static std::smatch results;

	vector<pair<int , int>> bounds;

	for (const auto& elem : checker.type().pointer()) {
		auto begin_ = elem.begin();
		auto end_	= elem.end();
		std::regex_search(begin_ , end_ , results , rgx);

		int l = std::stoi(results.str());

		begin_ = results[0].second;
		std::regex_search(begin_ , end_ , results , rgx);

		int r = std::stoi(results.str());

		bounds.push_back({l , r});
	}
	return make_pair(checker.type().type() ,  bounds);
}

Transformer::Transformer(shared_ptr<ast::Ast> root) {
	auto program_handle = std::dynamic_pointer_cast<ast::Program>(root);
	if (program_handle == nullptr) {
		throw std::runtime_error{"[Transformer] bad root pointer"};
	}
	type_kit = make_shared<TypeToolKit>();
	table = analysiser::GetTable();
	ast_root = transProgram(program_handle);
}

shared_ptr<Program> Transformer::transProgram(shared_ptr<ast::Program> cur) {
	now = {
		std::to_string(cur->program_head()->line()) ,
		cur->program_head()->id() ,
		nullptr
	};
	return make_shared<Program>(
		cur->program_head()->id() ,
		transBody<ast::ProgramBody>(cur->program_body())
	);
}

shared_ptr<ASTNode>
Transformer::transSubprogram(shared_ptr<ast::Subprogram> cur) {
	las = now;
	now = {
		std::to_string(cur->subprogram_head()->line()) ,
		cur->subprogram_head()->id() ,
		cur
	};
	func_name_table[now.func_name] = (cur->subprogram_head());

	// std::cerr << "Subpro@" << cur->line() <<" : " << cur->column() <<"\n";

	auto params = cur->subprogram_head()->parameters();
	vector<shared_ptr<Argument>> args;

	for(const auto& elem : params) {
		const auto& list = elem->id_list();
		for(size_t i = 0 ; i < list->Size() ; i++) {
			
			args.push_back(std::move(make_shared<Argument>(
				make_shared_token<Var> (TokenType::IDENTIFIER , (*list)[i]) ,
				make_shared_token<Type>(TokenType::RESERVED	  , ToCString(elem->type())),
				elem->is_var() //TODO : need_test
			)));
		}
	}

	shared_ptr<ASTNode> ret;
	if (cur->subprogram_head()->is_function()) { // function
		// std::cerr << "Function@" << cur->line() <<" : " << cur->column() <<"\n";

		ret = make_shared<Function>(
			cur->subprogram_head()->id() ,
			make_shared_token<Type>(
				TokenType::RESERVED , 
				ToCString(cur->subprogram_head()->return_type())
			) , args ,
			transBody<ast::SubprogramBody>(cur->subprogram_body())
		);

	} else { // procedure
		// std::cerr << "Proce@" << cur->line() <<" : " << cur->column() <<"\n";

		ret = make_shared<Subprogram>(
			cur->subprogram_head()->id() , args ,
			transBody<ast::SubprogramBody>(cur->subprogram_body())
		);
	}

	now = las;
	return ret;
}

template<typename T>
shared_ptr<Block> 
Transformer::transBody(shared_ptr<T> body) {
	static_assert(std::is_same_v<T , ast::ProgramBody> ||
				  std::is_same_v<T , ast::SubprogramBody> ,
				"[Transformer] bad body type");
	// std::cerr << "Body@" << body->line() <<" : " << body->column() <<"\n";

	auto decls = transDeclaration<T>(body); // high priority

	vector<std::shared_ptr<ASTNode>> child;
	if constexpr (std::is_same_v<T , ast::ProgramBody>) {
		child.push_back(std::move(transStatement(body->statements())));
	} else {
		child.push_back(std::move(transStatement(body->statement_list())));
	}


	return make_shared<Block>( decls , make_shared<Compound>(child) );
}


template<typename T>
shared_ptr<Declaration>
Transformer::transDeclaration(shared_ptr<T> body) {
	static_assert(std::is_same_v<T , ast::ProgramBody> ||
				  std::is_same_v<T , ast::SubprogramBody> ,
				"[Transformer] bad body type");
	// std::cerr << "body@" << body->line() <<" : " << body->column() <<"\n";

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
	if constexpr(std::is_same_v<T , ast::ProgramBody>) {
		auto func_decl = body->subprogram_declarations();
		for(const auto& elem : func_decl) {
			decls.push_back(std::move(transSubprogram(elem)));
		}
	}

	return make_shared<Declaration>(decls);

}


shared_ptr<ConstDeclaration>
Transformer::transConstDeclaration(shared_ptr<ast::ConstDeclaration> cur) {
	return make_shared<ConstDeclaration>(
		make_shared_token<Var>(TokenType::IDENTIFIER , cur->id() ) ,
		make_shared_token<ConstType>(
			TokenType::RESERVED , ToCString(analysiser::GetExprType(cur->const_value()).type())
		) ,
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
			vector<pair<int , int>> perd; perd.reserve(arr_type->periods().size());

			for (const auto& elem : arr_type->periods()) {
				perd.push_back({elem.digits_1 , elem.digits_2});
			}

			res.push_back(std::move(make_shared<ArrayDeclaration>(
				make_shared<Array>(
					make_shared<Var>((*list)[i]) ,
					perd ,
					type_kit->StringToVarType(ToCString(cur->type()->basic_type()))
				) ,
				make_shared<ArrayType>(
					make_shared_token<Type>(
						TokenType::RESERVED , 
						ToCString(cur->type()->basic_type())
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
					ToCString(cur->type()->basic_type())
				)
			)));
		}
	}

	return res;

}
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
pair<shared_ptr<ASTNode> , VarType>
Transformer::passExpr(shared_ptr<ast::Expression> cur) {

	switch (cur->GetType()) {
	case ast::ExprType::INT :
		return {
			make_shared_token<Num>(
				TokenType::NUMBER ,
				std::to_string(
					std::static_pointer_cast<ast::IntegerValue>(cur)->value()
			)) ,
			VarType::INT
		};

	case ast::ExprType::REAL :
		return {
			make_shared_token<Real>(
				TokenType::NUMBER ,
				std::to_string(
					std::static_pointer_cast<ast::RealValue>(cur)->value()
				)
			) ,
			VarType::REAL
		};

	case ast::ExprType::CHAR : 
		return {
			make_shared_token<Char>(
				TokenType::NUMBER ,
				std::to_string(
					std::static_pointer_cast<ast::CharValue>(cur)->ch()
				)
			) ,
			VarType::CHAR
		};

	case ast::ExprType::BOOLEAN : 
		return {
			make_shared_token<Bool>(
				TokenType::NUMBER ,
				std::static_pointer_cast<ast::BooleanValue>(cur)->value() ?
				"1" : "0"
			),
			VarType::BOOL
		};

	case ast::ExprType::STRING : 
		return {
			make_shared_token<String>(
				TokenType::STRING ,
				std::static_pointer_cast<ast::StringValue>(cur)->value()
			) ,
			VarType::STRING
		};
		
	case ast::ExprType::VARIABLE : {
		auto var = std::static_pointer_cast<ast::Variable>(cur);

		auto [_1 , _2 , is_ref , is_ret , _5 , _type] = checkIdType(var->id());

		if (var->expr_list().size() == 0) { // basic type
			auto var_type  = type_kit->StringToVarType(
				ToCString(type_kit->StringToVarType(ToCString(_type))));

			return {
				make_shared<Var>( 
					var->id() , is_ref , is_ret ,
					var_type
				) ,
				var_type
			};

		} else { // array type
			vector<shared_ptr<ASTNode>> indices; 
			indices.reserve(var->expr_list().size()); 

			for (const auto& elem : var->expr_list()) {
				indices.push_back(std::move(passExpr(elem).first));
			}

			auto type_info = checkArrayType(var->id()).value();
			auto var_type  = type_kit->StringToVarType(ToCString(type_info.first));
			return {
				make_shared<ArrayAccess>(
					make_shared<Array>(
						make_shared<Var>( var->id() , is_ref ) ,
						std::move(type_info.second)
					),
					indices ,
					var_type
				) ,
				var_type
			};
	}
	}

	case ast::ExprType::UNARY : {
		auto expr = std::static_pointer_cast<ast::UnaryExpr>(cur);
		auto factor = passExpr( expr->factor() );
		return {
			make_shared<UnaryOperation>(
				make_shared_token<Oper> (TokenType::OPERATOR , ToCString(expr->op())) ,
				factor.first ,
				VarType::UNDEFINED
			) ,
			factor.second
		};
	}

	case ast::ExprType::BINARY : {
		auto bin_expr = std::static_pointer_cast<ast::BinaryExpr>(cur);

		// std::cerr << "Get Type : " << checkExprType(bin_expr) << "\n";
		auto l_expr = passExpr(bin_expr->lhs());
		auto r_expr = passExpr(bin_expr->rhs());
		// auto expr_type = Ma
		auto expr_type = type_kit->MergeType(l_expr.second , r_expr.second , 
							ToCString(bin_expr->op()));

		return {
			make_shared<BinaryOperation>(
				l_expr.first ,
				make_shared_token<Oper>(TokenType::OPERATOR , ToCString(bin_expr->op())) ,
				r_expr.first ,
				expr_type
			) ,
			expr_type
		};
	}

	case ast::ExprType::CALL : {
		auto callee = std::static_pointer_cast<ast::CallValue>(cur);
		vector<shared_ptr<ASTNode>> param; param.reserve(callee->params().size());

		auto ret_type = type_kit->StringToVarType(ToCString(
				func_name_table[callee->id()]->return_type()));

		for (const auto& elem : callee->params()) {
			param.push_back(std::move(passExpr(elem).first));
		}

		return {
			make_shared<FunctionCall>( callee->id() , param , 
				getParamRefs(callee->id()) , 
				ret_type) ,
			ret_type
		};
	}

	case ast::ExprType::CALL_OR_VAR : {
		auto callval = std::static_pointer_cast<ast::CallOrVar>(cur);
		auto [is_var , is_func , _ , is_ret , is_const , _6] = checkIdType(callval->id());
		
		if (is_func) {
			return passExpr(make_shared<ast::CallValue>(
				callval->line() , callval->column() , callval->id()));
		} else if (is_var || is_ret || is_const) { // const and include 'return' -> func := expr;
			return passExpr(make_shared<ast::Variable>(
				callval->line() , callval->column() , callval->id()));
		}
	}

	}

	throw std::runtime_error{"[Transformer] unknown expr type"};
	return {nullptr , VarType::UNDEFINED};
}




shared_ptr<ASTNode>
Transformer::transExpression(shared_ptr<ast::Expression> cur) {
	// Optimizer::Calculator calc{cur};
	// auto res = calc.getResultExpr();
	// std::cerr << "Cur@" << cur->line() <<" : " << cur->column() <<"\n";

	return passExpr(cur).first;
}


/**
 *  enum StatementType
    {
        ASSIGN_STATEMENT 	= 1,
        CALL_STATEMENT 		= 2,
        COMPOUND_STATEMENT 	= 3,
        IF_STATEMENT 		= 4,
        FOR_STATEMENT 		= 5,
		EXIT_STATEMENT 		= 6,
        WHILE_STATEMENT 	= 7
    };
*/
shared_ptr<ASTNode>
Transformer::transStatement(shared_ptr<ast::Statement> cur) {
	// std::cerr << "Cur@" << cur->line() <<" : " << cur->column() <<"\n";
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

	case ast::StatementType::EXIT_STATEMENT :
		return transExitStatement(std::static_pointer_cast<ast::ExitStatement>(cur));

	case ast::StatementType::WHILE_STATEMENT : 
		return transWhileStatement(std::static_pointer_cast<ast::WhileStatement>(cur));
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
		param.push_back(std::move(transExpression(elem)));
	}

	return make_shared<Statement>(
		make_shared<FunctionCall>( cur->name() , param , getParamRefs(cur->name()))
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

shared_ptr<ExitStatement>
Transformer::transExitStatement(shared_ptr<ast::ExitStatement> cur) {
	auto return_type = now.func_node->subprogram_head()->return_type();

	if (return_type == -1) // void type
		return make_shared<ExitStatement>();
	else
		return make_shared<ExitStatement>(now.func_name);
}

shared_ptr<WhileStatement>
Transformer::transWhileStatement(shared_ptr<ast::WhileStatement> cur) {
	return make_shared<WhileStatement>(
		transExpression(cur->condition()) ,
		make_shared<Compound>(
			vector<shared_ptr<ASTNode>>
			{transStatement( cur->statement() )}
		)
	);
}

} // End Namespace
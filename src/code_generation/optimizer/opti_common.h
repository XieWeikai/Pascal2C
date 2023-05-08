#pragma once 
#include <memory>

#include "ast/ast.h"
#include "ast/expr.h"
#include "ast/program.h"
#include "ast/statement.h"

#include "semantic_analysis/semantic_analysis.h"
#include "semantic_analysis/symbol_table.h"
#include "semantic_analysis/errors.h"

#include "code_generation/ast_adapter.h"
#include "code_generation/symbol_table_adapter.h"
#include "code_generation/symbol_item.h"
#include "code_generation/abstract_symbol_table_adapter.h"
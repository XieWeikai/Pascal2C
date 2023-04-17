file(GLOB AST "*.cc" "*.h")
add_library( ast STATIC ${AST} )

target_link_libraries( ast PUBLIC
	lexer
)
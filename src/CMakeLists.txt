include_directories(common)

add_subdirectory(lexer)
add_subdirectory(syntax)
add_subdirectory(ast)

add_executable( ${PROJECT_NAME}
	main.cc
	common/args_parser.cc
)

target_link_libraries( ${PROJECT_NAME} PUBLIC 
	lexer
	syntax	
)
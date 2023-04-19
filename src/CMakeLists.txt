# Enable modules to include each other's files
include_directories(.)


add_subdirectory(common)
add_subdirectory(lexer)
add_subdirectory(syntax)
add_subdirectory(ast)

add_executable( ${PROJECT_NAME}
	main.cc
)

target_link_libraries( ${PROJECT_NAME} PUBLIC 
	common
	syntax
	ast
)
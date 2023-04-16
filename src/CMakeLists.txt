include_directories(common)

add_subdirectory(lexer)

add_executable( ${PROJECT_NAME}
	main.cc
	common/args_parser.cc
)
include_directories(.)

add_library( syntax STATIC
	syntax.cc
	expr.cc
	program.cc
	statement.cc
)

target_link_libraries( syntax
    gtest_main
    gmock_main
	lexer
)
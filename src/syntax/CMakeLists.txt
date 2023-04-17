file(GLOB SYNTAX "*.cc" ".h")

add_library( syntax STATIC ${SYNTAX} )

target_link_libraries( syntax PUBLIC
    gtest_main
    gmock_main
	lexer
	ast
)
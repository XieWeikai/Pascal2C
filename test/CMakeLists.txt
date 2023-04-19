# Test Case Generation

# # >>> lexer test >>>
# add_executable(lexer_test
#         lexer/lexer_test.cc
#         src/lexer/utils.c
#         ${FLEX_Scanner_OUTPUTS}
#         ${OUTPUT_HEADER}
#         )
# target_include_directories(lexer_test PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
# target_link_libraries(
#         lexer_test
#		  lexer
#         gtest_main
#         gmock_main
# )
# <<< lexer test <<<



# # >>> ast test >>>
# file(GLOB AST "src/ast/*.h" "src/ast/*.cc")
# file(GLOB AST_TEST "test/ast/*.cc")

# add_executable(
#         ast_test
#         ${AST_TEST}
#         ${AST}
# )

# target_include_directories(ast_test PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# target_link_libraries(
#         ast_test
#         gtest_main
#         gmock_main
# )

# # gtest_discover_tests(ast_test)

# # <<< ast test <<<

# # >>> parser test >>>
# file(GLOB PARSER "src/parser/*.h" "src/parser/expr.cc" "src/parser/statement.cc" "src/parser/parser.cc")
# file(GLOB PARSER_TEST "test/parser/*.cc")
# add_executable(
#         parser_test
#         ${PARSER_TEST}
#         ${PARSER}
#         ${AST}
#         ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.c
#         src/lexer/utils.c
# )
# target_include_directories(parser_test PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# target_link_libraries(
#         parser_test
#         gtest_main
#         gmock_main
# )

# # <<< parser test <<<



# >>> common util test >>>



# <<< common util test <<<
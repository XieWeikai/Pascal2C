cmake_minimum_required(VERSION 3.10)

# Define project name and version
project(Pascal2C VERSION 1.0)

# Specify C++ version to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQURIED True)

# Enable/Disable system debug Logger
add_definitions(-DUSE_SYS_LOGGER)

# Add Project Infomation
add_definitions(-D__P2C_VERSION__="${PROJECT_VERSION}")
add_definitions(-D__GIT_URL__="https://github.com/XieWeikai/Pascal2C")


# Add Source Code Dictionary.
add_subdirectory(src)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
        cmake_policy(SET CMP0135 NEW)
endif()




# Download and install GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

# Prevent Google Test from overloading our Compiler/Linker options
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Include GoogleTest into CTest
include(GoogleTest)

if(NOT DEFINED $ENV{CMAKE_EXPORT_COMPILE_COMMANDS})
    message("** ENV(CMAKE_EXPORT_COMPILE_COMMANDS) not defined")
    message("** defined ENV(CMAKE_EXPORT_COMPILE_COMMANDS) for you!")
    set($ENV{CMAKE_EXPORT_COMPILE_COMMANDS} true)
endif()


# # >>> lexer test >>>
# add_executable(lexer_test
#         test/lexer/lexer_test.cc
#         src/lexer/utils.c
#         ${FLEX_Scanner_OUTPUTS}
#         ${OUTPUT_HEADER}
#         )
# target_include_directories(lexer_test PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
# target_link_libraries(
#         lexer_test
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
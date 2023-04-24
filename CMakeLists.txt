cmake_minimum_required(VERSION 3.10)

# Define project name and version
project(Pascal2C VERSION 1.0)

# Specify C++ version to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQURIED True)
set(ARG_DOWNLOAD_EXTRACT_TIMESTAMP TRUE)

include_directories("src/")

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

# >>> lexer test >>>
find_package(FLEX REQUIRED)
find_package(Python3 COMPONENTS Interpreter REQUIRED)
set(SCRIPT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/lexer/token_generator.py")
set(OUTPUT_HEADER "${CMAKE_CURRENT_BINARY_DIR}/lexer.h")
add_custom_command(
        OUTPUT ${OUTPUT_HEADER}
        COMMAND ${Python3_EXECUTABLE} ${SCRIPT_PATH} ${OUTPUT_HEADER}
        DEPENDS ${SCRIPT_PATH}
        COMMENT "Generating lexer.h"
)
FLEX_TARGET(Scanner src/lexer/lexer.lex ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.c)

add_executable(lexer_exe
        src/lexer/main.c
        src/lexer/utils.c
        ${FLEX_Scanner_OUTPUTS}
        ${OUTPUT_HEADER}
)
target_include_directories(lexer_exe PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

add_executable(lexer_test
        test/lexer/lexer_test.cc
        src/lexer/utils.c
        ${FLEX_Scanner_OUTPUTS}
        ${OUTPUT_HEADER}
)
target_include_directories(lexer_test PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(
        lexer_test
        gtest_main
        gmock_main
)

# <<< lexer test <<<

# >>> ast test >>>
file(GLOB AST "src/ast/*.h" "src/ast/*.cc")
file(GLOB AST_TEST "test/ast/*.cc")

add_executable(
        ast_test
        ${AST_TEST}
        ${AST}
)

target_include_directories(ast_test PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(
        ast_test
        gtest_main
        gmock_main
)

# gtest_discover_tests(ast_test)

# <<< ast test <<<

# >>> parser test >>>
file(GLOB PARSER "src/parser/*.h" "src/parser/expr.cc" "src/parser/statement.cc" "src/parser/parser.cc" "src/parser/program.cc")
file(GLOB PARSER_TEST "test/parser/*.cc")
add_executable(
        parser_test
        ${PARSER_TEST}
        ${PARSER}
        ${AST}
        ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.c
        src/lexer/utils.c
)
target_include_directories(parser_test PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(
        parser_test
        gtest_main
        gmock_main
)

# <<< parser test <<<

# >>> generator test >>>
enable_testing()

file(GLOB GENERATOR "src/code_generation/*.cc")
file(GLOB GENERATOR_TEST "test/code_generation/*.cc")

add_executable(
        generator_test
        ${GENERATOR_TEST}
)

add_library(LibGenerator ${GENERATOR})

# target_include_directories(generator_test PRIVATE "src/code_generation" "test/code_generation")
target_link_libraries(
        generator_test
        LibGenerator
        GTest::gtest_main
        GTest::gmock_main
)

include(GoogleTest)

gtest_discover_tests(generator_test)

# add_test(NAME GeneratorTest COMMAND generator_test)

# <<< generator test <<<

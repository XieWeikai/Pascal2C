cmake_minimum_required(VERSION 3.10)

# Define project name and version
project(Pascal2C VERSION 1.0)

# Specify C++ version to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQURIED True)

# use flex
#find_package(FLEX)
#FLEX_TARGET(LEXER src/lexer/lexer.lex ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc)
#add_custom_command(
#        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lexer.h
#        COMMAND  python src/lexer/token_generator.py > ${CMAKE_CURRENT_BINARY_DIR}/lexer.h
#)
#include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Include source files in src/ directory
file(GLOB SOURCES CONFIGURE_DEPENDS
    "src/ast/*.cc" "src/ast/*.h"
    "src/code_generation/*.cc" "src/code_generation/*.h"
    "src/parser/*.cc" "src/parser/*.h"
    "src/lexer/*.cc" "src/lexer/*.h")
add_executable(Pascal2C ${SOURCES})

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

# Define test binary file
file(GLOB TEST_SOURCES "test/code_generation/*.cc")
add_executable(Pascal2CTests ${TEST_SOURCES} ${SOURCES})

# Link our project library with GoogleTest and GoogleMock
target_link_libraries(
    Pascal2CTests
    gtest_main
    gmock_main
    )

# Include GoogleTest into CTest
include(GoogleTest)
gtest_discover_tests(Pascal2CTests)

if(NOT DEFINED $ENV{CMAKE_EXPORT_COMPILE_COMMANDS})
    message("** ENV(CMAKE_EXPORT_COMPILE_COMMANDS) not defined")
    message("** defined ENV(CMAKE_EXPORT_COMPILE_COMMANDS) for you!")
    set($ENV{CMAKE_EXPORT_COMPILE_COMMANDS} true)
endif()



aux_source_directory(src/ast AST)
aux_source_directory(test/ast AST_TEST)
aux_source_directory(src/parser PARSER)
aux_source_directory(test/parser PARSER_TEST)
add_executable(
        parser_test
        ${PARSER_TEST}
        ${PARSER}
        ${AST}
        src/lexer/lex.yy.c
)
add_executable(
        ast_test
        ${AST_TEST}
        ${AST}
)

target_link_libraries(
        ast_test
        gtest_main
        gmock_main
)

target_link_libraries(
        parser_test
        gtest_main
        gmock_main
)

gtest_discover_tests(ast_test)
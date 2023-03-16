cmake_minimum_required(VERSION 3.10)

# Define project name and version
project(Pascal2C VERSION 1.0)

# Specify C++ version to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQURIED True)

# Include source files in src/ directory
file(GLOB SOURCES "src/*.cc" "src/*.h")
add_executable(Pascal2C ${SOURCES})

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
file(GLOB TEST_SOURCES "test/*.cc")
add_executable(Pascal2CTests ${TEST_SOURCES})

# Link our project library with GoogleTest and GoogleMock
target_link_libraries(
    Pascal2C
    gtest_main
    gmock_main
    )

# Include GoogleTest into CTest
include(GoogleTest)
gtest_discover_tests(Pascal2CTests)

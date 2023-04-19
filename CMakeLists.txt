Cmake_Minimum_Required(VERSION 3.11)

# Define project name and version
Project(Pascal2C VERSION 1.0)

# Specify C++ version to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQURIED True)

# Enable/Disable system debug Logger
Add_Definitions(-DUSE_SYS_LOGGER)

# Add Project Infomation
Add_Definitions(-D__P2C_VERSION__="${PROJECT_VERSION}")
Add_Definitions(-D__GIT_URL__="https://github.com/XieWeikai/Pascal2C")


# Add Source Code Dictionary.
Add_Subdirectory(src)

# Add Test Case Code Dictionary
Add_Subdirectory(test)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
        Cmake_Policy(SET CMP0135 NEW)
endif()




# Download and install GoogleTest
Include(FetchContent)
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
    Message("** ENV(CMAKE_EXPORT_COMPILE_COMMANDS) not defined")
    Message("** defined ENV(CMAKE_EXPORT_COMPILE_COMMANDS) for you!")
    Set($ENV{CMAKE_EXPORT_COMPILE_COMMANDS} true)
endif()


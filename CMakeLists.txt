cmake_minimum_required(VERSION 3.20)
project(Interpreter-SQL)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(APPLE)
    set(BISON_EXECUTABLE "/opt/homebrew/opt/bison/bin/bison")
    set(FLEX_EXECUTABLE "/opt/homebrew/opt/flex/bin/flex")
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/bison")
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/flex")
endif()

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

BISON_TARGET(MyParser grammar/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(MyScanner grammar/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

include(FetchContent)

set(JUST_INSTALL_CEREAL ON CACHE BOOL "" FORCE)
set(SKIP_PERFORMANCE_COMPARISON ON CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "Disable Cereal tests" FORCE)
set(BUILD_SANDBOX OFF CACHE BOOL "Disable Cereal sandbox" FORCE)

FetchContent_Declare(
    cereal
    GIT_REPOSITORY https://github.com/USCiLab/cereal.git
    GIT_TAG        v1.3.2
)

FetchContent_MakeAvailable(cereal)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(include)

file(GLOB_RECURSE ALL_SOURCES "src/*.cpp")

add_executable(Interpreter-SQL
        ${ALL_SOURCES}
        ${BISON_MyParser_OUTPUTS}
        ${FLEX_MyScanner_OUTPUTS}
)

target_link_libraries(Interpreter-SQL PRIVATE cereal)

cmake_minimum_required(VERSION 3.20)
project(Interpreter-SQL)

set(CMAKE_CXX_STANDARD 17)

if(APPLE)
    # Wyłączamy szukanie w systemowych ścieżkach /usr/bin, żeby nie brał starego Bisona 2.3
    set(BISON_EXECUTABLE "/opt/homebrew/opt/bison/bin/bison")
    set(FLEX_EXECUTABLE "/opt/homebrew/opt/flex/bin/flex")

    # Dodatkowe ścieżki dla CMake
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/bison")
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/flex")
endif()

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

# generowanie plików
BISON_TARGET(MyParser grammar/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(MyScanner grammar/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(include)

add_executable(Interpreter-SQL
        src/main.cpp
        ${BISON_MyParser_OUTPUTS}
        ${FLEX_MyScanner_OUTPUTS}
)
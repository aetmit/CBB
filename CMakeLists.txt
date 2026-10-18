cmake_minimum_required(VERSION 3.20)

project(cbb LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(cbb
    src/main.cpp
    src/driver.cpp

    src/core/span.cpp
    src/core/fs.cpp
    src/core/error.cpp
    src/core/diagnostics.cpp 

    src/core/structures/arena.cpp

    src/lex/lexer.cpp
    src/lex/token.cpp

    src/parse/ast.cpp 
    src/parse/parser.cpp 

    src/sema/hir.cpp
    src/sema/ids.cpp
    src/sema/analysis.cpp
)

target_compile_options(cbb PRIVATE
    -Wall
    -Wextra
)

target_include_directories(cbb PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
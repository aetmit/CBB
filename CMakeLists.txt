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
    src/cbb.cpp

    src/core/file.cpp
    src/core/span.cpp
    src/core/arena.cpp
    src/core/diagnostics.cpp

    src/lexer/lexer.cpp 
    src/lexer/token.cpp 

    src/parser/ast.cpp
    src/parser/parser.cpp

    src/analysis/resolution.cpp
    src/analysis/ids.cpp
    src/analysis/analysis.cpp
    src/analysis/context.cpp
    src/analysis/type_checker.cpp
    src/analysis/type.cpp
    src/analysis/hir.cpp
)

target_compile_options(cbb PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

target_include_directories(cbb PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
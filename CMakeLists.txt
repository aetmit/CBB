cmake_minimum_required(VERSION 3.20)

project(cbb LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(cbb
    src/main.c

    src/core/fs.c
    src/core/io.c 
    src/core/span.c
    src/core/str.c

    src/lexer/lexer.c
)

target_compile_options(cbb PRIVATE
    -Wall
    -Wextra
)

target_include_directories(cbb PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
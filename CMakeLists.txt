cmake_minimum_required(VERSION 3.20)

project(brase C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(brase
    src/main.c
)

target_compile_options(brase PRIVATE
    -Wall
    -Wextra
)

target_include_directories(brase PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
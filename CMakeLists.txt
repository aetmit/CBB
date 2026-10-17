cmake_minimum_required(VERSION 3.20)

project(cbb LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(cbb
    src/main.cpp
)

target_compile_options(cbb PRIVATE
    -Wall
    -Wextra
)

target_include_directories(cbb PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
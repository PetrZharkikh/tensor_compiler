cmake_minimum_required(VERSION 3.16)
project(tensor_compiler CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(fe
    frontend/src/main.cpp
    frontend/src/graph/node.cpp
)

target_include_directories(fe PRIVATE
    frontend/include
)
cmake_minimum_required(VERSION 3.16)
project(tensor_compiler CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Protobuf CONFIG REQUIRED)
find_package(absl CONFIG REQUIRED)

add_executable(fe
    frontend/src/main.cpp

    frontend/src/graph/node.cpp
    frontend/src/graph/graph.cpp
    frontend/src/graph/dot.cpp
    frontend/src/graph/algorithms.cpp

    frontend/src/onnx/onnx_loader.cpp

    third_party/onnx_generated/onnx.pb.cc
)

target_include_directories(fe PRIVATE
    frontend/include
    third_party/onnx_generated
    ${Protobuf_INCLUDE_DIRS}
)

target_link_libraries(fe PRIVATE
    protobuf::libprotobuf

    absl::log
    absl::check
    absl::base
    absl::strings
)
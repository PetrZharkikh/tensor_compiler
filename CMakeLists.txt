cmake_minimum_required(VERSION 3.16)
project(tensor_compiler CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(BUILD_TESTS "Build tests" OFF)
option(ENABLE_MLIR "Enable MLIR support when toolchain is available" ON)
option(ENABLE_LLVM "Enable LLVM support when toolchain is available" ON)

find_package(Protobuf CONFIG REQUIRED)
find_package(absl CONFIG REQUIRED)

set(FRONTEND_SOURCES
    frontend/src/graph/node.cpp
    frontend/src/graph/graph.cpp
    frontend/src/graph/dot.cpp
    frontend/src/graph/algorithms.cpp
    frontend/src/onnx/onnx_loader.cpp
    third_party/onnx_generated/onnx.pb.cc
)

add_library(frontend_lib STATIC ${FRONTEND_SOURCES})
target_include_directories(frontend_lib PUBLIC
    frontend/include
    third_party/onnx_generated
    ${Protobuf_INCLUDE_DIRS}
)

target_link_libraries(frontend_lib PUBLIC
    protobuf::libprotobuf
    absl::log
    absl::check
    absl::base
    absl::strings
)

set(TC_HAS_MLIR OFF)
if(ENABLE_MLIR)
    find_package(MLIR QUIET CONFIG)
    if(MLIR_FOUND)
        set(TC_HAS_MLIR ON)
        message(STATUS "MLIR toolchain found: enabling MLIR path")
    else()
        message(WARNING "ENABLE_MLIR=ON but MLIR was not found. Building with stub midend implementation.")
    endif()
endif()

set(TC_HAS_LLVM OFF)
if(ENABLE_LLVM)
    find_package(LLVM QUIET CONFIG)
    if(LLVM_FOUND)
        set(TC_HAS_LLVM ON)
        message(STATUS "LLVM toolchain found: enabling LLVM path")
    else()
        message(WARNING "ENABLE_LLVM=ON but LLVM was not found. Building with stub backend implementation.")
    endif()
endif()

add_library(midend_mlir_lib STATIC
    midend/mlir/lowering_stub.cpp
)
target_include_directories(midend_mlir_lib PUBLIC
    frontend/include
)
target_compile_definitions(midend_mlir_lib PUBLIC
    TC_HAS_MLIR=$<BOOL:${TC_HAS_MLIR}>
)
target_link_libraries(midend_mlir_lib PUBLIC
    frontend_lib
)

add_library(backend_llvm_lib STATIC
    backend/llvm/codegen_stub.cpp
)
target_include_directories(backend_llvm_lib PUBLIC
    backend/include
)
target_compile_definitions(backend_llvm_lib PUBLIC
    TC_HAS_LLVM=$<BOOL:${TC_HAS_LLVM}>
)
target_link_libraries(backend_llvm_lib PUBLIC
    midend_mlir_lib
)

add_executable(driver
    driver/main.cpp
)
target_link_libraries(driver PRIVATE
    backend_llvm_lib
)

if(BUILD_TESTS)
    enable_testing()

    add_executable(graph_tests
        tests/test_graph.cpp
    )

    target_include_directories(graph_tests PRIVATE
        frontend/include
    )
    target_link_libraries(graph_tests PRIVATE frontend_lib)

    add_test(NAME graph_tests COMMAND graph_tests)

    add_executable(onnx_loader_tests
        tests/test_onnx_loader.cpp
    )

    target_include_directories(onnx_loader_tests PRIVATE
        frontend/include
        third_party/onnx_generated
        ${Protobuf_INCLUDE_DIRS}
    )

    target_link_libraries(onnx_loader_tests PRIVATE
        frontend_lib
    )

    add_test(NAME onnx_loader_tests COMMAND onnx_loader_tests)

    add_executable(frontend_pipeline_tests
        tests/test_frontend_pipeline.cpp
    )

    target_include_directories(frontend_pipeline_tests PRIVATE
        frontend/include
        third_party/onnx_generated
        ${Protobuf_INCLUDE_DIRS}
    )

    target_link_libraries(frontend_pipeline_tests PRIVATE
        frontend_lib
    )

    target_compile_definitions(frontend_pipeline_tests PRIVATE
        TC_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
    )

    add_test(NAME frontend_pipeline_tests COMMAND frontend_pipeline_tests)
endif()
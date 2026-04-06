#include <iostream>
#include <string>

#include "codegen/codegen.h"
#include "graph/graph.h"
#include "ir/lowering.h"
#include "onnx/onnx_loader.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: driver <model.onnx>\n";
        return 1;
    }

    const std::string model_path = argv[1];
    Graph graph = OnnxLoader::load(model_path);
    MlirModuleHandle mlir_module = lowerGraphToMlir(graph);
    std::string llvm_ir = emitLlvmIr(mlir_module);
    std::string asm_text = emitAsm(llvm_ir, "native");

    std::cout << "Loaded graph nodes: " << graph.size() << "\n";
    std::cout << mlir_module.text;
    std::cout << llvm_ir;
    std::cout << asm_text;
    return 0;
}

#include "ir/lowering.h"

MlirModuleHandle lowerGraphToMlir(const Graph& graph) {
    MlirModuleHandle handle;
    handle.text = "// stub-mlir module, nodes=" + std::to_string(graph.size()) + "\n";
#if TC_HAS_MLIR
    handle.text += "// MLIR toolchain detected\n";
#else
    handle.text += "// MLIR toolchain not detected, using stub lowering\n";
#endif
    return handle;
}

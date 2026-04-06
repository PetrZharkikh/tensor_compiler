#pragma once

#include <string>

#include "graph/graph.h"

// Minimal handle for a future MLIR module representation.
struct MlirModuleHandle {
    std::string text;
};

MlirModuleHandle lowerGraphToMlir(const Graph& graph);

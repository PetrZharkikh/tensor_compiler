#include <iostream>

#include "graph/graph.h"
#include "graph/dot.h"
#include "onnx/onnx_loader.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: fe <model.onnx>\n";
        return 1;
    }

    try {
        Graph g = OnnxLoader::load(argv[1]);
        dump_dot(g, "graph.dot");
        std::cout << "Loaded ONNX. Nodes=" << g.nodes().size() << "\n";
        std::cout << "Wrote graph.dot\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 2;
    }   
}
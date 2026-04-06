#include <cassert>
#include <fstream>
#include <string>
#include <unordered_set>

#include "graph/algorithms.h"
#include "graph/dot.h"
#include "onnx/onnx_loader.h"

#ifndef TC_SOURCE_DIR
#define TC_SOURCE_DIR "."
#endif

static void test_pipeline_from_ready_onnx_model() {
    const std::string model_path = std::string(TC_SOURCE_DIR) +
        "/third_party/onnx/onnx/backend/test/data/pytorch-converted/test_ReLU/model.onnx";

    Graph g = OnnxLoader::load(model_path);

    assert(g.size() >= 1);
    assert(g.validate());

    auto topo = topological_sort(g);
    assert(topo.size() == g.size());

    std::unordered_set<Node*> unique_nodes;
    for (Node* n : topo) {
        assert(n != nullptr);
        unique_nodes.insert(n);
    }
    assert(unique_nodes.size() == g.size());

    const std::string dot_path = "/tmp/tensor_compiler_frontend_pipeline.dot";
    dump_dot(g, dot_path);

    std::ifstream in(dot_path);
    assert(in && "DOT file was not created");
    std::string dot((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    assert(dot.find("digraph G") != std::string::npos);
    assert(dot.find("Relu") != std::string::npos);
}

int main() {
    test_pipeline_from_ready_onnx_model();
    return 0;
}

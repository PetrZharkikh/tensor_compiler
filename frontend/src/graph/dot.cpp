#include "graph/dot.h"

#include <fstream>
#include <stdexcept>

static const char* op_to_cstr(OpType op) {
    switch(op) {
        case OpType::Add: return "Add";
        case OpType::Mul: return "Mul";
        case OpType::Conv: return "Conv";
        case OpType::Relu: return "Relu";
        case OpType::MatMul: return "MatMul";
        case OpType::Gemm: return "Gemm";
        default: return "Unknown"; 
    }
}

void dump_dot(const Graph& g, const std::string& path) {
    std::ofstream out(path);
    if (!out) {
        throw std::runtime_error("dump_dot: cannot open output file: " + path);
    }

    out << "digraph G {\n";
    out << "rankdir=LR;\n";
    out << "node [shape=box];\n";

    for (const auto& uptr : g.nodes()) {
        const Node* n = uptr.get();
        out << " \"" << n->name << "\""
            << " [label=\"" << n->name << "\\n" << op_to_cstr(n->op) << "\"];\n";
    }

    for (const auto& uptr : g.nodes()) {
        const Node* from = uptr.get();
        for (const Node* to : from->outputs) {
            out << "  \"" << from->name << "\" -> \"" << to->name << "\";\n";
        }
    }

    out << "}\n";
}
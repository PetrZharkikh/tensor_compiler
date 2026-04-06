#include <iostream>
#include "graph/graph.h"
#include "graph/algorithms.h"

int main() {
    Graph g;
    Node* a = g.add_node("a", OpType::Add);
    Node* r = g.add_node("r", OpType::Relu);
    Node* m = g.add_node("m", OpType::Gemm);

    std::cout << g.add_edge(a, r) << "\n"; 
    std::cout << g.add_edge(r, m) << "\n";
    std::cout << g.add_edge(m, a) << "\n"; 

    std::cout << "valid=" << g.validate() << "\n";

    auto topo = topological_sort(g);
    std::cout << "topo:";
    for (auto* n : topo) std::cout << " " << n->name;
    std::cout << "\n";
}
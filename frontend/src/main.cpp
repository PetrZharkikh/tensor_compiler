#include <iostream>
#include "graph/graph.h"

int main() {
    Graph g;

    Node* a = g.add_node("Add", OpType::Add);
    Node* r = g.add_node("Relu", OpType::Relu);
    g.add_edge(a, r);

    std::cout << "Graph nodes: " << g.nodes().size() << "\n";
    std::cout << "A outputs: " << a->outputs.size() << "\n";
    std::cout << "Relu inputs: " << r->inputs.size() << "\n";
    
    return 0;
}
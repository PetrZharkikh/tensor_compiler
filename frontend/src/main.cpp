#include <iostream>
#include "graph/graph.h"
#include "graph/dot.h"

int main() {
    Graph g;

    Node* a = g.add_node("Add", OpType::Add);
    Node* r = g.add_node("Relu", OpType::Relu);
    Node* m = g.add_node("MatMul", OpType::MatMul);

    g.add_edge(a, r);
    g.add_edge(r, m);

    dump_dot(g, "graph.dot");

    std::cout << "Wrote graph.dot (nodes=" << g.nodes().size() << ")\n";
    return 0;
}
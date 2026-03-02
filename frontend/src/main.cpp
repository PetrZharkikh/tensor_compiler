#include <iostream>
#include "graph/node.h"

int main() {
    Node a("Add", OpType::Add);
    Node r("Relu", OpType::Relu);

    a.outputs.push_back(&r);
    r.inputs.push_back(&a);

    std::cout << "Node A outputs: " << a.outputs.size() << "\n";
    std::cout << "Node Relu inputs: " << r.inputs.size() << "\n";

    return 0;
}
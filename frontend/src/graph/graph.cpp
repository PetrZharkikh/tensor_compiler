#include "graph/graph.h"

Node* Graph::add_node(const std::string& name, OpType op) {
    nodes_.push_back(std::make_unique<Node>(name, op));
    return nodes_.back().get();
}

void Graph::add_edge(Node* from, Node* to) {
    if (!from || !to) return;
    from->outputs.push_back(to);
    to->inputs.push_back(from);
}

const std::vector<std::unique_ptr<Node>>& Graph::nodes() const {
    return nodes_;
}
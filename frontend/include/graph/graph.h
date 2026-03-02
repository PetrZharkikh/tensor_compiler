#pragma once

#include <memory>
#include <vector>
#include <string>

#include "graph/node.h"

class Graph {
public:
    Node* add_node(const std::string& name, OpType op);

    void add_edge(Node* from, Node* to);

    const std::vector<std::unique_ptr<Node>>& nodes() const;

private:
    std::vector<std::unique_ptr<Node>> nodes_;
};
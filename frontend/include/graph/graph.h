#pragma once

#include <memory>
#include <vector>
#include <string>

#include "graph/node.h"

class Graph {
public:
    Node* add_node(const std::string& name, OpType op);

    // DAG only
    bool add_edge(Node* from, Node* to);

    bool remove_edge(Node* from, Node* to);
    bool remove_node(Node* n);

    Node* replace_node_keep_edges(Node* old_node, const std::string& new_name, OpType new_op);

    // Утилиты
    std::vector<Node*> roots() const; // inputs.size()==0
    std::vector<Node*> sinks() const; // outputs.size()==0
    bool validate() const;            // checks symmetry between inputs/outputs

    const std::vector<std::unique_ptr<Node>>& nodes() const;
    size_t size() const { return nodes_.size(); }

private:
    bool would_create_cycle(Node* from, Node* to) const;

    std::vector<std::unique_ptr<Node>> nodes_;
};
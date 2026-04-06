#include "graph/graph.h"

#include <algorithm>
#include <unordered_set>

static bool contains_ptr(const std::vector<Node*>& v, const Node* p) {
    return std::find(v.begin(), v.end(), p) != v.end();
}

static void erase_ptr(std::vector<Node*>& v, const Node* p) {
    v.erase(std::remove(v.begin(), v.end(), p), v.end());
}

// достижим ли target из start по outputs
static bool reachable(const Node* start, const Node* target) {
    if (!start || !target) return false;
    if (start == target) return true;

    std::unordered_set<const Node*> vis;
    std::vector<const Node*> st{start};

    while (!st.empty()) {
        const Node* cur = st.back();
        st.pop_back();

        if (!vis.insert(cur).second) continue;

        for (const Node* nxt : cur->outputs) {
            if (!nxt) continue;
            if (nxt == target) return true;
            st.push_back(nxt);
        }
    }
    return false;
}

Node* Graph::add_node(const std::string& name, OpType op) {
    nodes_.push_back(std::make_unique<Node>(name, op));
    return nodes_.back().get();
}

bool Graph::would_create_cycle(Node* from, Node* to) const {
    // добавление from->to создаёт цикл, если уже есть путь to -> from
    return reachable(to, from);
}

bool Graph::add_edge(Node* from, Node* to) {
    if (!from || !to) return false;
    if (would_create_cycle(from, to)) return false;

    if (!contains_ptr(from->outputs, to)) from->outputs.push_back(to);
    if (!contains_ptr(to->inputs, from))  to->inputs.push_back(from);
    return true;
}

bool Graph::remove_edge(Node* from, Node* to) {
    if (!from || !to) return false;
    erase_ptr(from->outputs, to);
    erase_ptr(to->inputs, from);
    return true;
}

bool Graph::remove_node(Node* n) {
    if (!n) return false;

    // отсоединяем от соседей
    for (Node* in : n->inputs)  erase_ptr(in->outputs, n);
    for (Node* out : n->outputs) erase_ptr(out->inputs, n);

    n->inputs.clear();
    n->outputs.clear();

    auto it = std::find_if(nodes_.begin(), nodes_.end(),
                           [&](const std::unique_ptr<Node>& up) { return up.get() == n; });
    if (it == nodes_.end()) return false;

    nodes_.erase(it);
    return true;
}

Node* Graph::replace_node_keep_edges(Node* old_node, const std::string& new_name, OpType new_op) {
    if (!old_node) return nullptr;

    auto old_inputs = old_node->inputs;
    auto old_outputs = old_node->outputs;

    Node* neu = add_node(new_name, new_op);
    neu->attributes = old_node->attributes;

    for (Node* in : old_inputs) {
        remove_edge(in, old_node);
        if (!add_edge(in, neu)) return nullptr; // DAG check
    }
    for (Node* out : old_outputs) {
        remove_edge(old_node, out);
        if (!add_edge(neu, out)) return nullptr;
    }

    remove_node(old_node);
    return neu;
}

std::vector<Node*> Graph::roots() const {
    std::vector<Node*> out;
    for (const auto& up : nodes_) {
        if (up->inputs.empty()) out.push_back(up.get());
    }
    return out;
}

std::vector<Node*> Graph::sinks() const {
    std::vector<Node*> out;
    for (const auto& up : nodes_) {
        if (up->outputs.empty()) out.push_back(up.get());
    }
    return out;
}

bool Graph::validate() const {
    auto contains = [](const std::vector<Node*>& v, Node* x) {
        return std::find(v.begin(), v.end(), x) != v.end();
    };

    for (const auto& up : nodes_) {
        Node* n = up.get();
        for (Node* out : n->outputs) {
            if (!out) return false;
            if (!contains(out->inputs, n)) return false;
        }
        for (Node* in : n->inputs) {
            if (!in) return false;
            if (!contains(in->outputs, n)) return false;
        }
    }
    return true;
}

const std::vector<std::unique_ptr<Node>>& Graph::nodes() const {
    return nodes_;
}


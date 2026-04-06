#include "graph/algorithms.h"

#include <queue>
#include <unordered_map>
#include <unordered_set>

std::vector<Node*> topological_sort(const Graph& g) {
    std::vector<Node*> all;
    all.reserve(g.nodes().size());
    for (const auto& up : g.nodes()) all.push_back(up.get());

    std::unordered_map<Node*, int> indeg;
    indeg.reserve(all.size());
    for (Node* n : all) indeg[n] = 0;

    for (Node* n : all) {
        for (Node* out : n->outputs) {
            if (out) indeg[out]++;
        }
    }

    std::queue<Node*> q;
    for (auto& [n, d] : indeg) {
        if (d == 0) q.push(n);
    }

    std::vector<Node*> topo;
    topo.reserve(all.size());

    while (!q.empty()) {
        Node* cur = q.front(); q.pop();
        topo.push_back(cur);

        for (Node* out : cur->outputs) {
            if (!out) continue;
            if (--indeg[out] == 0) q.push(out);
        }
    }

    return topo;
}

std::vector<Node*> dfs_from(Node* start) {
    std::vector<Node*> order;
    if (!start) return order;

    std::unordered_set<Node*> vis;
    std::vector<Node*> st{start};

    while (!st.empty()) {
        Node* cur = st.back();
        st.pop_back();
        if (!cur) continue;
        if (!vis.insert(cur).second) continue;

        order.push_back(cur);

        for (auto it = cur->outputs.rbegin(); it != cur->outputs.rend(); ++it) {
            if (*it) st.push_back(*it);
        }
    }
    return order;
}

std::vector<Node*> bfs_from(Node* start) {
    std::vector<Node*> order;
    if (!start) return order;

    std::unordered_set<Node*> vis;
    std::queue<Node*> q;

    vis.insert(start);
    q.push(start);

    while (!q.empty()) {
        Node* cur = q.front(); q.pop();
        order.push_back(cur);

        for (Node* nxt : cur->outputs) {
            if (!nxt) continue;
            if (vis.insert(nxt).second) q.push(nxt);
        }
    }
    return order;
}
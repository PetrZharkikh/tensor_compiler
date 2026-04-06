#include <cassert>
#include <vector>

#include "graph/algorithms.h"
#include "graph/graph.h"

static void test_cycle_detection() {
    Graph g;
    Node* a = g.add_node("a", OpType::Add);
    Node* b = g.add_node("b", OpType::Relu);
    Node* c = g.add_node("c", OpType::Gemm);

    assert(g.add_edge(a, b));
    assert(g.add_edge(b, c));
    assert(!g.add_edge(c, a));  // must reject cycle
    assert(g.validate());
}

static void test_topological_sort() {
    Graph g;
    Node* a = g.add_node("a", OpType::Add);
    Node* b = g.add_node("b", OpType::Mul);
    Node* c = g.add_node("c", OpType::Relu);

    assert(g.add_edge(a, b));
    assert(g.add_edge(b, c));

    std::vector<Node*> topo = topological_sort(g);
    assert(topo.size() == 3);
    assert(topo[0] == a);
    assert(topo[1] == b);
    assert(topo[2] == c);
}

static void test_remove_and_replace_node() {
    Graph g;
    Node* a = g.add_node("a", OpType::Add);
    Node* b = g.add_node("b", OpType::Relu);
    Node* c = g.add_node("c", OpType::Gemm);

    assert(g.add_edge(a, b));
    assert(g.add_edge(b, c));
    assert(g.validate());

    Node* new_b = g.replace_node_keep_edges(b, "b2", OpType::Mul);
    assert(new_b != nullptr);
    assert(new_b->name == "b2");
    assert(g.validate());

    assert(new_b->inputs.size() == 1);
    assert(new_b->outputs.size() == 1);
    assert(new_b->inputs[0] == a);
    assert(new_b->outputs[0] == c);

    assert(g.remove_node(new_b));
    assert(g.validate());
    assert(g.size() == 2);
}

int main() {
    test_cycle_detection();
    test_topological_sort();
    test_remove_and_replace_node();
    return 0;
}

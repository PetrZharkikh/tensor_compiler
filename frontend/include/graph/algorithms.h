#pragma once

#include <vector>
#include "graph/graph.h"

std::vector<Node*> topological_sort(const Graph& g); // Kahn
std::vector<Node*> dfs_from(Node* start);           
std::vector<Node*> bfs_from(Node* start);           
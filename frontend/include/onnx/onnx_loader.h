#pragma once

#include <string>
#include "graph/graph.h"

class OnnxLoader {
public:
    static Graph load(const std::string& path);
};
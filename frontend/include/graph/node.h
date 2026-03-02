#pragma once

#include <string>
#include <vector>
#include <unordered_map>

enum class OpType {
    Add,
    Mul,
    Conv,
    Relu,
    MatMul,
    Gemm
};

struct Attribute {              //universal structure for ints, floats and strings
    std::string name;
    std::vector<int64_t> ints;
    std::vector<float> floats;
    std:: string str;
};

class Node {
public:
    Node(const std::string& name, OpType op);

    std::string name; //ONNX node name
    OpType op;

    std::vector<Node*> inputs;
    std::vector<Node*> outputs;

    std::unordered_map<std::string, Attribute> attributes;
};
#include "onnx/onnx_loader.h"

#include <fstream>
#include <stdexcept>
#include <unordered_map>

#include <google/protobuf/stubs/common.h>
#include <onnx.pb.h>

static OpType convert_op(const std::string& s) {
    if (s == "Add") return OpType::Add;
    if (s == "Mul") return OpType::Mul;
    if (s == "Conv") return OpType::Conv;
    if (s == "Relu") return OpType::Relu;
    if (s == "MatMul") return OpType::MatMul;
    if (s == "Gemm") return OpType::Gemm;
    throw std::runtime_error("Unsupported op: " + s);
}

static Attribute parse_attribute(const onnx::AttributeProto& attr) {
    Attribute a;
    a.name = attr.name();

    switch (attr.type()) {
        case onnx::AttributeProto::INT:
            a.ints.push_back(attr.i());
            break;
        case onnx::AttributeProto::FLOAT:
            a.floats.push_back(attr.f());
            break;
        case onnx::AttributeProto::STRING:
            a.str = attr.s();
            break;
        case onnx::AttributeProto::INTS:
            for (auto v : attr.ints()) a.ints.push_back(v);
            break;
        case onnx::AttributeProto::FLOATS:
            for (auto v : attr.floats()) a.floats.push_back(v);
            break;
        case onnx::AttributeProto::STRINGS:
            if (attr.strings_size() > 0) a.str = attr.strings(0);
            break;
        default:
            break;
    }
    return a;
}

Graph OnnxLoader::load(const std::string& path) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    std::ifstream input(path, std::ios::binary);
    if (!input) {
        throw std::runtime_error("Cannot open ONNX file: " + path);
    }

    onnx::ModelProto model;
    if (!model.ParseFromIstream(&input)) {
        throw std::runtime_error("Failed to parse ONNX model: " + path);
    }

    const auto& og = model.graph();

    Graph g;

    std::unordered_map<std::string, Node*> value_to_node;

    for (const auto& node : og.node()) {
        OpType op = convert_op(node.op_type());

        std::string node_name = node.name().empty() ? node.op_type() : node.name();
        Node* cur = g.add_node(node_name, op);

        for (const auto& attr : node.attribute()) {
            cur->attributes[attr.name()] = parse_attribute(attr);
        }

        for (const auto& in_name : node.input()) {
            auto it = value_to_node.find(in_name);
            if (it != value_to_node.end()) {
                g.add_edge(it->second, cur);
            }
        }

        for (const auto& out_name : node.output()) {
            value_to_node[out_name] = cur;
        }
    }

    return g;
}
#include <cassert>
#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <string>

#include "../third_party/onnx_generated/onnx.pb.h"

#include "onnx/onnx_loader.h"

static std::string make_temp_path(const std::string& filename) {
    return "/tmp/" + filename;
}

static void write_model_to_file(const onnx::ModelProto& model, const std::string& path) {
    std::ofstream out(path, std::ios::binary);
    assert(out && "Cannot open temp file for ONNX model");
    assert(model.SerializeToOstream(&out) && "Cannot serialize ONNX model");
}

static void test_load_simple_graph_and_attributes() {
    onnx::ModelProto model;
    auto* graph = model.mutable_graph();
    graph->set_name("g");

    auto* n1 = graph->add_node();
    n1->set_name("n_add");
    n1->set_op_type("Add");
    n1->add_input("x");
    n1->add_input("y");
    n1->add_output("z");

    auto* a_int = n1->add_attribute();
    a_int->set_name("alpha");
    a_int->set_type(onnx::AttributeProto::INT);
    a_int->set_i(7);

    auto* a_float = n1->add_attribute();
    a_float->set_name("beta");
    a_float->set_type(onnx::AttributeProto::FLOAT);
    a_float->set_f(0.5f);

    auto* a_str = n1->add_attribute();
    a_str->set_name("tag");
    a_str->set_type(onnx::AttributeProto::STRING);
    a_str->set_s("demo");

    auto* a_ints = n1->add_attribute();
    a_ints->set_name("axes");
    a_ints->set_type(onnx::AttributeProto::INTS);
    a_ints->add_ints(1);
    a_ints->add_ints(2);

    auto* n2 = graph->add_node();
    n2->set_name("n_relu");
    n2->set_op_type("Relu");
    n2->add_input("z");
    n2->add_output("out");

    const std::string path = make_temp_path("tensor_compiler_test_ok.onnx");
    write_model_to_file(model, path);

    Graph g = OnnxLoader::load(path);
    std::remove(path.c_str());

    assert(g.size() == 2);
    assert(g.validate());
    assert(g.roots().size() == 1);
    assert(g.sinks().size() == 1);

    Node* add_node = nullptr;
    Node* relu_node = nullptr;
    for (const auto& up : g.nodes()) {
        if (up->name == "n_add") add_node = up.get();
        if (up->name == "n_relu") relu_node = up.get();
    }

    assert(add_node != nullptr);
    assert(relu_node != nullptr);
    assert(add_node->outputs.size() == 1 && add_node->outputs[0] == relu_node);
    assert(relu_node->inputs.size() == 1 && relu_node->inputs[0] == add_node);

    assert(add_node->attributes.count("alpha") == 1);
    assert(add_node->attributes.at("alpha").ints.size() == 1);
    assert(add_node->attributes.at("alpha").ints[0] == 7);

    assert(add_node->attributes.count("beta") == 1);
    assert(add_node->attributes.at("beta").floats.size() == 1);
    assert(add_node->attributes.at("beta").floats[0] == 0.5f);

    assert(add_node->attributes.count("tag") == 1);
    assert(add_node->attributes.at("tag").str == "demo");

    assert(add_node->attributes.count("axes") == 1);
    assert(add_node->attributes.at("axes").ints.size() == 2);
}

static void test_unsupported_op_throws() {
    onnx::ModelProto model;
    auto* graph = model.mutable_graph();

    auto* n = graph->add_node();
    n->set_name("bad");
    n->set_op_type("UnsupportedOp");
    n->add_output("z");

    const std::string path = make_temp_path("tensor_compiler_test_bad_op.onnx");
    write_model_to_file(model, path);

    bool thrown = false;
    try {
        (void)OnnxLoader::load(path);
    } catch (const std::runtime_error&) {
        thrown = true;
    }
    std::remove(path.c_str());

    assert(thrown);
}

static void test_missing_file_throws() {
    bool thrown = false;
    try {
        (void)OnnxLoader::load("/tmp/definitely_missing_tensor_compiler_model.onnx");
    } catch (const std::runtime_error&) {
        thrown = true;
    }
    assert(thrown);
}

int main() {
    test_load_simple_graph_and_attributes();
    test_unsupported_op_throws();
    test_missing_file_throws();
    return 0;
}

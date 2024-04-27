#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <tuple>
#include <graph_transfer.hpp>
namespace py = pybind11;

PYBIND11_MODULE(graph_transfer_cpp, m) {
    m.doc() = "Fast graph_transfer CPP Plugin"; // Optional module docstring

    py::class_<GraphTransfer>(m, "GraphTransfer")
        .def(py::init<const std::string &>())
        .def("add_vertex_if_not_exists", &GraphTransfer::add_vertex_if_not_exists_internal)
        .def("add_edge", &GraphTransfer::add_edge_internal)
        .def("show_graph_details", &GraphTransfer::show_graph_details_internal)
        .def("print_all_data", &GraphTransfer::print_all_data_internal)
        .def("save_graph", &GraphTransfer::save_graph_internal)
        .def("load_graph", &GraphTransfer::load_graph_internal)
        ;
   py::class_<VertexProperties>(m, "VertexProperties")
        .def(py::init([](vector<float> _point, vector<float> config) { 
            return new VertexProperties(_point , config); }))
        .def_readwrite("point", &VertexProperties::point)
        .def_readwrite("config", &VertexProperties::config)
        ;
    py::class_<EdgeProperties>(m, "EdgeProperties")
        .def(py::init([](float weight, list<pair<int, int>> q_list, bool connected) { return new EdgeProperties(weight, q_list, connected); }))
        .def_readwrite("weight", &EdgeProperties::weight)
        .def_readwrite("connected", &EdgeProperties::connected)
        .def_readwrite("q_list", &EdgeProperties::q_list)
        ;
}


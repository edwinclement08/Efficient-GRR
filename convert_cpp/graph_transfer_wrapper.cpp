#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <graph_transfer.hpp>
namespace py = pybind11;

PYBIND11_MODULE(graph_transfer_cpp, m) {
    m.doc() = "Fast graph_transfer CPP Plugin"; // Optional module docstring

    py::class_<GraphTransfer>(m, "GraphTransfer")
        .def(py::init<const std::string &>())
        .def("add_vertex_if_not_exists", &GraphTransfer::add_vertex_if_not_exists)
        .def("add_edge", &GraphTransfer::add_edge)
        .def("show_graph_details", &GraphTransfer::show_graph_details)
        .def("save_graph", &GraphTransfer::save_graph)
        .def("load_graph", &GraphTransfer::load_graph)
        ;
   py::class_<VertexProperties>(m, "VertexProperties")
        .def(py::init([](tuple<float, float, float>_point, list<float> config) { 
            return new VertexProperties(_point, config); }))
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


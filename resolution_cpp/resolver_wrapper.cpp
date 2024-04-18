#include <pybind11/pybind11.h>
#include <resolver.hpp>
namespace py = pybind11;

PYBIND11_MODULE(resolver_cpp, m) {
    m.doc() = "Fast Resolver CPP Plugin"; // Optional module docstring

    py::class_<NetConvert>(m, "NetConvert")
        .def(py::init<const std::string &>())
        .def("add_vertex_if_not_exists", &NetConvert::add_vertex_if_not_exists)
        .def("add_edge", &NetConvert::add_edge)
        .def("show_graph_details", &NetConvert::show_graph_details)
        .def("save_graph", &NetConvert::save_graph)
        ;
}


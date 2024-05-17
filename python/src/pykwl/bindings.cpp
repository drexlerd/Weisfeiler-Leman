#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "wl/wl.hpp"

using namespace wl;
namespace py = pybind11;

/**
 * Bindings
 */
void init_kwl(py::module_& m)
{
    py::class_<Graph>(m, "Graph")  //
        .def(py::init<bool>())
        .def("add_node", &Graph::add_node, py::arg("label") = 0)
        .def("add_edge", &Graph::add_edge, py::arg("src_node"), py::arg("dst_node"), py::arg("label") = 0);

    py::class_<WeisfeilerLeman>(m, "WeisfeilerLeman")  //
        .def(py::init<int>())
        .def(py::init<int, bool>())
        .def("get_k", &WeisfeilerLeman::get_k)
        .def("get_ignore_counting", &WeisfeilerLeman::get_ignore_counting)
        .def("compute_coloring", &WeisfeilerLeman::compute_coloring, py::arg("graph"), py::arg("max_num_iterations") = std::numeric_limits<size_t>::max())
        .def("get_coloring_function_size", &WeisfeilerLeman::get_coloring_function_size);
}

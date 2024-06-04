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
    py::class_<EdgeColoredGraph>(m, "EdgeColoredGraph")  //
        .def(py::init<bool>())
        .def("__str__", &EdgeColoredGraph::to_string)
        .def("add_node", &EdgeColoredGraph::add_node, py::arg("label") = 0)
        .def("add_edge", &EdgeColoredGraph::add_edge, py::arg("src_node"), py::arg("dst_node"), py::arg("label") = 0);

    py::class_<GraphColoring>(m, "GraphColoring")  //
        .def("get_frequencies", &GraphColoring::get_frequencies)
        .def("is_identical_to", &GraphColoring::is_identical_to);

    py::class_<CanonicalColorRefinement>(m, "CanonicalColorRefinement")  //
        .def(py::init<bool>())
        .def("calculate", &CanonicalColorRefinement::calculate, py::arg("graph"), py::arg("factor_matrix") = false)
        .def("get_coloring", &CanonicalColorRefinement::get_coloring)
        .def("get_quotient_matrix", &CanonicalColorRefinement::get_quotient_matrix)
        .def_static("coloring_to_histogram", &CanonicalColorRefinement::coloring_to_histogram);

    py::class_<WeisfeilerLeman>(m, "WeisfeilerLeman")  //
        .def(py::init<int>())
        .def(py::init<int, bool>())
        .def("get_k", &WeisfeilerLeman::get_k)
        .def("get_ignore_counting", &WeisfeilerLeman::get_ignore_counting)
        .def("compute_coloring", &WeisfeilerLeman::compute_coloring, py::arg("graph"), py::arg("max_num_iterations") = std::numeric_limits<size_t>::max())
        .def("compute_initial_coloring", &WeisfeilerLeman::compute_initial_coloring)
        .def("compute_next_coloring", &WeisfeilerLeman::compute_next_coloring)
        .def("get_coloring_function_size", &WeisfeilerLeman::get_coloring_function_size);
}

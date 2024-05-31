#include "wl/details/weisfeiler_leman.hpp"

#include <cstddef>
#include <cstdlib>
#include <stdexcept>
#include <tuple>
#include <vector>

namespace wl
{

/*
 * TODO: Refactor the interface to use runtime polymorphism instead. This class is currently unnecessary.
 */

WeisfeilerLeman::WeisfeilerLeman(int k) : WeisfeilerLeman(k, false) {}

WeisfeilerLeman::WeisfeilerLeman(int k, bool ignore_counting) : m_k(k), m_1wl(ignore_counting), m_2wl(ignore_counting)
{
    if (k < 1 || k > 2)
    {
        throw std::invalid_argument("k must be either 1 or 2");
    }
}

int WeisfeilerLeman::get_k() const { return m_k; }

bool WeisfeilerLeman::get_ignore_counting() const
{
    if (get_k() == 1)
    {
        return m_1wl.get_ignore_counting();
    }

    if (get_k() == 2)
    {
        return m_2wl.get_ignore_counting();
    }

    throw std::runtime_error("internal error");
}

size_t WeisfeilerLeman::get_coloring_function_size() const
{
    if (get_k() == 1)
    {
        return m_1wl.get_coloring_function_size();
    }

    if (get_k() == 2)
    {
        return m_2wl.get_coloring_function_size();
    }

    throw std::runtime_error("internal error");
}

std::tuple<bool, size_t, std::vector<int>, std::vector<int>> WeisfeilerLeman::compute_coloring(const EdgeColoredGraph& graph, size_t max_num_iterations)
{
    if (get_k() == 1)
    {
        return m_1wl.compute_coloring(graph, max_num_iterations);
    }

    if (get_k() == 2)
    {
        return m_2wl.compute_coloring(graph, max_num_iterations);
    }

    throw std::runtime_error("internal error");
}

GraphColoring WeisfeilerLeman::compute_initial_coloring(const EdgeColoredGraph& graph)
{
    if (get_k() == 1)
    {
        return m_1wl.compute_initial_coloring(graph);
    }

    if (get_k() == 2)
    {
        return m_2wl.compute_initial_coloring(graph);
    }

    throw std::runtime_error("internal error");
}

bool WeisfeilerLeman::compute_next_coloring(const EdgeColoredGraph& graph, const GraphColoring& current_coloring, GraphColoring& ref_next_coloring)
{
    if (get_k() == 1)
    {
        return m_1wl.compute_next_coloring(graph, current_coloring, ref_next_coloring);
    }

    if (get_k() == 2)
    {
        return m_2wl.compute_next_coloring(graph, current_coloring, ref_next_coloring);
    }

    throw std::runtime_error("internal error");
}

}

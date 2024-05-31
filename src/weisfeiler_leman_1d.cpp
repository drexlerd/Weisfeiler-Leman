#include "wl/details/weisfeiler_leman_1d.hpp"

#include "wl/details/utils.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <tuple>
#include <utility>
#include <vector>

namespace wl
{

WeisfeilerLeman1D::WeisfeilerLeman1D() : WeisfeilerLeman1D(false) {}

WeisfeilerLeman1D::WeisfeilerLeman1D(bool ignore_counting) : m_color_function(), m_ignore_counting(ignore_counting) {}

bool WeisfeilerLeman1D::get_ignore_counting() const { return m_ignore_counting; }

size_t WeisfeilerLeman1D::get_coloring_function_size() const { return m_color_function.size(); }

std::vector<AdjacentColor> WeisfeilerLeman1D::get_colors_pairs(const std::vector<Color>& node_colors,
                                                               const std::vector<int>& node_indices,
                                                               const std::vector<Color>& edge_colors,
                                                               const std::vector<int>& edge_indices)
{
    assert(node_indices.size() == edge_indices.size());

    std::vector<AdjacentColor> adjacent_colors;

    for (size_t index = 0; index < node_indices.size(); ++index)
    {
        adjacent_colors.emplace_back(node_colors[node_indices[index]], edge_colors[edge_indices[index]]);
    }

    return adjacent_colors;
}

Color WeisfeilerLeman1D::get_new_color(NodeColorContext&& node_color_context)
{
    auto& first_colors = std::get<1>(node_color_context);
    auto& second_colors = std::get<2>(node_color_context);

    std::sort(first_colors.begin(), first_colors.end());
    std::sort(second_colors.begin(), second_colors.end());

    if (m_ignore_counting)
    {
        auto first_last = std::unique(first_colors.begin(), first_colors.end());
        first_colors.erase(first_last, first_colors.end());

        auto second_last = std::unique(second_colors.begin(), second_colors.end());
        second_colors.erase(second_last, second_colors.end());
    }

    auto it = m_color_function.find(node_color_context);

    if (it == m_color_function.end())
    {
        int color = static_cast<int>(m_color_function.size());
        m_color_function.emplace(std::move(node_color_context), color);
        return color;
    }

    return it->second;
}

bool WeisfeilerLeman1D::compute_next_coloring(const EdgeColoredGraph& graph, const GraphColoring& current_coloring, GraphColoring& ref_next_coloring)
{
    for (int node = 0; node < graph.get_num_nodes(); ++node)
    {
        auto outgoing_colors =
            get_colors_pairs(current_coloring.colorings, graph.get_outbound_adjacent(node), graph.get_edge_labels(), graph.get_outbound_edges(node));

        auto ingoing_colors =
            graph.is_directed() ?
                get_colors_pairs(current_coloring.colorings, graph.get_inbound_adjacent(node), graph.get_edge_labels(), graph.get_inbound_edges(node)) :
                std::vector<AdjacentColor>();

        ref_next_coloring.colorings[node] = get_new_color({ current_coloring.colorings[node], std::move(outgoing_colors), std::move(ingoing_colors) });
    }

    return current_coloring.is_identical_to(ref_next_coloring);
}

std::tuple<bool, size_t, std::vector<int>, std::vector<int>> WeisfeilerLeman1D::compute_coloring(const EdgeColoredGraph& graph, size_t max_num_iterations)
{
    auto num_nodes = graph.get_num_nodes();

    auto current_coloring = compute_initial_coloring(graph);
    auto next_coloring = GraphColoring { std::vector<int>(num_nodes) };

    size_t num_iterations = 0;
    bool is_stable = false;

    while (true)
    {
        ++num_iterations;

        bool is_stable_i = compute_next_coloring(graph, current_coloring, next_coloring);

        std::swap(current_coloring, next_coloring);

        if (is_stable_i)
        {
            is_stable = true;
            break;
        }

        if (num_iterations == max_num_iterations)
        {
            break;
        }
    }

    auto [unique, counts] = current_coloring.get_frequencies();
    lexical_sort(unique, counts);
    return { is_stable, num_iterations, std::move(unique), std::move(counts) };
}

GraphColoring WeisfeilerLeman1D::compute_initial_coloring(const EdgeColoredGraph& graph)
{
    auto num_nodes = graph.get_num_nodes();
    auto current_coloring = std::vector<int>(num_nodes);

    for (int node = 0; node < num_nodes; ++node)
    {
        // Both graph labels and colors are natural numbers.
        // We make the graph labels negative so that they are not confused with colors.

        auto node_label = -graph.get_node_label(node) - 1;
        current_coloring[node] = get_new_color({ node_label, {}, {} });
    }

    return GraphColoring { std::move(current_coloring) };
}

}

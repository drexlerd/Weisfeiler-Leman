#include "wl/details/weisfeiler_leman_2d.hpp"

#include "wl/details/utils.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <tuple>
#include <utility>
#include <vector>

namespace wl
{

WeisfeilerLeman2D::WeisfeilerLeman2D() : WeisfeilerLeman2D(false) {}

WeisfeilerLeman2D::WeisfeilerLeman2D(bool ignore_counting) : m_color_function(), m_ignore_counting(ignore_counting) {}

bool WeisfeilerLeman2D::get_ignore_counting() const { return m_ignore_counting; }

size_t WeisfeilerLeman2D::get_coloring_function_size() const { return m_color_function.size(); }

std::vector<Color> WeisfeilerLeman2D::get_colors(const std::vector<Color>& colors, const std::vector<int>& indices)
{
    auto result = std::vector<int>(indices.size());

    for (const auto& i : indices)
    {
        result.emplace_back(colors[i]);
    }

    return result;
}

Color WeisfeilerLeman2D::get_new_color(NodeColorContext&& node_color_context)
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

int WeisfeilerLeman2D::get_subgraph_color(int src_node, int dst_node, const EdgeColoredGraph& graph)
{
    const auto& node_labels = graph.get_node_labels();
    const auto& edge_labels = graph.get_edge_labels();

    auto src_label = node_labels[src_node];
    auto dst_label = node_labels[dst_node];

    auto forward_edge_labels = get_colors(edge_labels, graph.get_edges(src_node, dst_node));
    auto self_src_edge_labels = get_colors(edge_labels, graph.get_edges(src_node, src_node));
    auto backward_edge_labels = get_colors(edge_labels, graph.get_edges(dst_node, src_node));
    auto self_dst_edge_labels = get_colors(edge_labels, graph.get_edges(dst_node, dst_node));

    // Encode the subgraph as a vector of the colors of the nodes and the edges.
    // We use a pairing function to give them a unique color to distinguish between self-loops and normal edges.

    auto forward_colors = std::vector<AdjacentColor>();
    auto backward_colors = std::vector<AdjacentColor>();

    for (const auto& label : forward_edge_labels)
    {
        forward_colors.emplace_back(pairing_function(0, label), dst_label);
    }

    for (const auto& label : self_src_edge_labels)
    {
        forward_colors.emplace_back(pairing_function(1, label), src_label);
    }

    for (const auto& label : backward_edge_labels)
    {
        backward_colors.emplace_back(pairing_function(0, label), src_label);
    }

    for (const auto& label : self_dst_edge_labels)
    {
        backward_colors.emplace_back(pairing_function(1, label), dst_label);
    }

    // Both graph labels and colors are natural numbers.
    // We make the graph labels negative so that they are not confused with colors.

    return get_new_color({ -pairing_function(src_label, dst_label) - 1, std::move(forward_colors), std::move(backward_colors) });
}

inline static int index_of_pair(int first_node, int second_node, int num_nodes) { return first_node * num_nodes + second_node; }

bool WeisfeilerLeman2D::compute_next_coloring(const EdgeColoredGraph& graph, const GraphColoring& current_coloring, GraphColoring& ref_next_coloring)
{
    const auto num_nodes = graph.get_num_nodes();

    for (int i = 0; i < num_nodes; ++i)
    {
        for (int j = 0; j < num_nodes; ++j)
        {
            auto compositions = std::vector<AdjacentColor>(num_nodes);

            const auto ij_index = index_of_pair(i, j, num_nodes);
            const auto ij_color = current_coloring.colorings[ij_index];

            for (int k = 0; k < num_nodes; ++k)
            {
                const auto ik_index = index_of_pair(i, k, num_nodes);
                const auto kj_index = index_of_pair(k, j, num_nodes);

                const auto ik_color = current_coloring.colorings[ik_index];
                const auto kj_color = current_coloring.colorings[kj_index];

                compositions[k] = { ik_color, kj_color };
            }

            ref_next_coloring.colorings[ij_index] = get_new_color({ ij_color, std::move(compositions), {} });
        }
    }

    return current_coloring.is_identical_to(ref_next_coloring);
}

std::tuple<bool, size_t, std::vector<int>, std::vector<int>> WeisfeilerLeman2D::compute_coloring(const EdgeColoredGraph& graph, size_t max_num_iterations)
{
    const auto num_nodes = graph.get_num_nodes();

    auto current_coloring = compute_initial_coloring(graph);
    auto next_coloring = GraphColoring { std::vector<int>(num_nodes * num_nodes) };

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

GraphColoring WeisfeilerLeman2D::compute_initial_coloring(const EdgeColoredGraph& graph)
{
    const auto num_nodes = graph.get_num_nodes();
    auto current_coloring = std::vector<int>(num_nodes * num_nodes);

    for (int first_node = 0; first_node < num_nodes; ++first_node)
    {
        for (int second_node = 0; second_node < num_nodes; ++second_node)
        {
            const auto pair_index = index_of_pair(first_node, second_node, num_nodes);
            current_coloring[pair_index] = get_subgraph_color(first_node, second_node, graph);
        }
    }

    return GraphColoring { std::move(current_coloring) };
}

}

#include "wl/details/weisfeiler_leman.hpp"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <stdexcept>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace wl
{

inline static int pairing_function(int x, int y)
{
    // Szudzik's pairing function
    return std::abs(x >= y ? x * x + x + y : y * y + x);
}

inline static std::vector<int> get_colors_from_indices(const std::vector<int>& colors, const std::vector<int>& indices)
{
    auto result = std::vector<int>(indices.size());

    for (const auto& i : indices)
    {
        result.emplace_back(colors[i]);
    }

    return result;
}

inline static void lexical_sort(std::vector<int>& items1, std::vector<int>& items2)
{
    assert(items1.size() == items2.size());

    std::vector<std::pair<int, int>> paired_items(items1.size());
    for (size_t i = 0; i < items1.size(); ++i)
    {
        paired_items[i] = { items1[i], items2[i] };
    }

    std::sort(paired_items.begin(), paired_items.end());

    for (size_t i = 0; i < items1.size(); ++i)
    {
        items1[i] = paired_items[i].first;
        items2[i] = paired_items[i].second;
    }
}

inline static std::pair<std::vector<int>, std::vector<int>> get_frequencies(const std::vector<int>& items)
{
    std::unordered_map<int, int> frequencies;

    for (int item : items)
    {
        frequencies[item]++;
    }

    std::vector<int> unique;
    std::vector<int> counts;

    for (const auto& [item, count] : frequencies)
    {
        unique.push_back(item);
        counts.push_back(count);
    }

    return { unique, counts };
}

inline static bool test_fixpoint(const std::vector<int>& current_coloring, const std::vector<int>& next_coloring)
{
    bool fixpoint = true;

    auto coloring_difference = next_coloring[0] - current_coloring[0];
    for (size_t i = 0; i < next_coloring.size(); ++i)
    {
        if ((current_coloring[i] + coloring_difference) != next_coloring[i])
        {
            fixpoint = false;
            break;
        }
    }

    return fixpoint;
}

WeisfeilerLeman::WeisfeilerLeman(int k) : m_color_function(), m_k(k)
{
    if (k < 1 || k > 2)
    {
        throw std::invalid_argument("k must be either 1 or 2");
    }
}

int WeisfeilerLeman::get_color(ColorMultiset&& color_multiset)
{
    auto it = m_color_function.find(color_multiset);

    if (it == m_color_function.end())
    {
        int color = static_cast<int>(m_color_function.size());
        m_color_function.emplace(std::move(color_multiset), color);
        return color;
    }

    return it->second;
}

std::tuple<int, std::vector<int>, std::vector<int>> WeisfeilerLeman::k1_fwl(const Graph& graph)
{
    auto num_nodes = graph.get_num_nodes();
    auto current_coloring = std::vector<int>(num_nodes);
    auto next_coloring = std::vector<int>(num_nodes);

    for (int node = 0; node < num_nodes; ++node)
    {
        // Graph labels must be non-negative and colors will always be positive.
        // We make the graph labels negative so that they are not confused with colors.

        auto node_label = -graph.get_node_label(node) - 1;
        current_coloring[node] = get_color({ node_label, {}, {}, {}, {} });
    }

    int num_iterations = 0;

    while (true)
    {
        ++num_iterations;

        for (int node = 0; node < num_nodes; ++node)
        {
            auto ingoing_edge_colors = get_colors_from_indices(graph.get_edge_labels(), graph.get_inbound_edges(node));
            auto outgoing_edge_colors = get_colors_from_indices(graph.get_edge_labels(), graph.get_outbound_edges(node));

            auto ingoing_node_colors = get_colors_from_indices(current_coloring, graph.get_inbound_adjacent(node));
            auto outgoing_node_colors = get_colors_from_indices(current_coloring, graph.get_outbound_adjacent(node));

            // Sort the vectors using lexical sorting to ensure that the actual order of retrieval is irrelevant.

            lexical_sort(ingoing_node_colors, ingoing_edge_colors);
            lexical_sort(outgoing_node_colors, outgoing_edge_colors);

            next_coloring[node] = get_color({ current_coloring[node],
                                              std::move(ingoing_edge_colors),
                                              std::move(outgoing_edge_colors),
                                              std::move(ingoing_node_colors),
                                              std::move(outgoing_node_colors) });
        }

        if (test_fixpoint(current_coloring, next_coloring))
        {
            break;
        }
        else
        {
            std::swap(current_coloring, next_coloring);
        }
    }

    auto [unique, counts] = get_frequencies(current_coloring);
    lexical_sort(unique, counts);
    return { num_iterations, std::move(unique), std::move(counts) };
}

inline static int index_of_pair(int first_node, int second_node, int num_nodes) { return first_node * num_nodes + second_node; }

int WeisfeilerLeman::get_subgraph_color(int src_node, int dst_node, const Graph& graph)
{
    const auto& node_labels = graph.get_node_labels();
    const auto& edge_labels = graph.get_edge_labels();

    auto src_label = node_labels[src_node];
    auto dst_label = node_labels[dst_node];

    auto forward_edge_labels = get_colors_from_indices(edge_labels, graph.get_edges(src_node, dst_node));
    auto backward_edge_labels = get_colors_from_indices(edge_labels, graph.get_edges(dst_node, src_node));
    auto self_src_edge_labels = get_colors_from_indices(edge_labels, graph.get_edges(src_node, src_node));
    auto self_dst_edge_labels = get_colors_from_indices(edge_labels, graph.get_edges(dst_node, dst_node));

    std::sort(forward_edge_labels.begin(), forward_edge_labels.end());
    std::sort(backward_edge_labels.begin(), backward_edge_labels.end());
    std::sort(self_src_edge_labels.begin(), self_src_edge_labels.end());
    std::sort(self_dst_edge_labels.begin(), self_dst_edge_labels.end());

    // Graph labels must be non-negative and colors will always be positive.
    // We make the graph labels negative so that they are not confused with colors.

    return get_color({ -pairing_function(src_label, dst_label) - 1,
                       std::move(forward_edge_labels),
                       std::move(backward_edge_labels),
                       std::move(self_src_edge_labels),
                       std::move(self_dst_edge_labels) });
}

std::tuple<int, std::vector<int>, std::vector<int>> WeisfeilerLeman::k2_fwl(const Graph& graph)
{
    const auto num_nodes = graph.get_num_nodes();
    auto current_coloring = std::vector<int>(num_nodes * num_nodes);
    auto next_coloring = std::vector<int>(num_nodes * num_nodes);

    for (int first_node = 0; first_node < num_nodes; ++first_node)
    {
        for (int second_node = 0; second_node < num_nodes; ++second_node)
        {
            const auto pair_index = index_of_pair(first_node, second_node, num_nodes);
            current_coloring[pair_index] = get_subgraph_color(first_node, second_node, graph);
        }
    }

    int num_iterations = 0;

    while (true)
    {
        ++num_iterations;

        for (int i = 0; i < num_nodes; ++i)
        {
            for (int j = 0; j < num_nodes; ++j)
            {
                auto compositions = std::vector<int>(num_nodes);

                const auto ij_index = index_of_pair(i, j, num_nodes);
                const auto ij_color = current_coloring[ij_index];

                for (int k = 0; k < num_nodes; ++k)
                {
                    const auto ik_index = index_of_pair(i, k, num_nodes);
                    const auto kj_index = index_of_pair(k, j, num_nodes);

                    const auto ik_color = current_coloring[ik_index];
                    const auto kj_color = current_coloring[kj_index];

                    compositions[k] = pairing_function(ik_color, kj_color);
                }

                std::sort(compositions.begin(), compositions.end());
                next_coloring[ij_index] = get_color({ ij_color, std::move(compositions), {}, {}, {} });
            }
        }

        if (test_fixpoint(current_coloring, next_coloring))
        {
            break;
        }
        else
        {
            std::swap(current_coloring, next_coloring);
        }
    }

    auto [unique, counts] = get_frequencies(current_coloring);
    lexical_sort(unique, counts);
    return { num_iterations, std::move(unique), std::move(counts) };
}

std::tuple<int, std::vector<int>, std::vector<int>> WeisfeilerLeman::compute_coloring(const Graph& graph)
{
    if (m_k == 1)
    {
        return k1_fwl(graph);
    }

    if (m_k == 2)
    {
        return k2_fwl(graph);
    }

    throw std::invalid_argument("k must be either 1 or 2");
}

}

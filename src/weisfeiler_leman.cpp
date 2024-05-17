#include "wl/details/weisfeiler_leman.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <stdexcept>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
#include <limits>
#include <climits>

namespace wl
{

inline static int safe_multiply(int x, int y) {
    if (x != 0 && (y > INT_MAX / x || y < INT_MIN / x)) {
        throw std::overflow_error("Overflow detected in multiplication");
    }
    return x * y;
}

inline static int pairing_function(int x, int y) {
    int x_sq, y_sq;
    if (x >= y) {
        x_sq = safe_multiply(x, x);  // Safe multiplication
        int x_sq_plus_x = x_sq + x;  // Add x
        if (x_sq_plus_x < x_sq) throw std::overflow_error("Overflow in addition");
        return x_sq_plus_x + y;  // Add y
    } else {
        y_sq = safe_multiply(y, y);  // Safe multiplication
        if (y_sq < y) throw std::overflow_error("Overflow in addition");
        return y_sq + x;  // Add x
    }
}


//inline static int pairing_function(int x, int y)
//{
//    // Szudzik's pairing function
//    return std::abs(x >= y ? x * x + x + y : y * y + x);
//}

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

WeisfeilerLeman::WeisfeilerLeman(int k) : WeisfeilerLeman(k, false) {}

WeisfeilerLeman::WeisfeilerLeman(int k, bool ignore_counting) : m_color_function(), m_k(k), m_ignore_counting(ignore_counting)
{
    if (k < 1 || k > 2)
    {
        throw std::invalid_argument("k must be either 1 or 2");
    }
}

int WeisfeilerLeman::get_k() const { return m_k; }

bool WeisfeilerLeman::get_ignore_counting() const { return m_ignore_counting; }

std::vector<Color> WeisfeilerLeman::get_colors(const std::vector<Color>& colors, const std::vector<int>& indices)
{
    auto result = std::vector<int>(indices.size());

    for (const auto& i : indices)
    {
        result.emplace_back(colors[i]);
    }

    return result;
}

std::vector<AdjacentColor> WeisfeilerLeman::get_colors_pairs(const std::vector<Color>& node_colors,
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

Color WeisfeilerLeman::get_new_color(NodeColorContext&& node_color_context)
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

std::tuple<bool, size_t, std::vector<int>, std::vector<int>> WeisfeilerLeman::k1_fwl(const Graph& graph, size_t max_num_iterations)
{
    auto num_nodes = graph.get_num_nodes();
    auto current_coloring = std::vector<int>(num_nodes);
    auto next_coloring = std::vector<int>(num_nodes);

    for (int node = 0; node < num_nodes; ++node)
    {
        // Graph labels must be non-negative and colors will always be positive.
        // We make the graph labels negative so that they are not confused with colors.

        auto node_label = -graph.get_node_label(node) - 1;
        current_coloring[node] = get_new_color({ node_label, {}, {} });
    }

    size_t num_iterations = 0;
    bool is_stable = false;

    while (true)
    {
        ++num_iterations;

        for (int node = 0; node < num_nodes; ++node)
        {
            auto outgoing_colors =
                get_colors_pairs(current_coloring, graph.get_outbound_adjacent(node), graph.get_edge_labels(), graph.get_outbound_edges(node));

            auto ingoing_colors =
                graph.is_directed() ?
                    get_colors_pairs(current_coloring, graph.get_inbound_adjacent(node), graph.get_edge_labels(), graph.get_inbound_edges(node)) :
                    std::vector<AdjacentColor>();

            next_coloring[node] = get_new_color({ current_coloring[node], std::move(outgoing_colors), std::move(ingoing_colors) });
        }

        if (test_fixpoint(current_coloring, next_coloring))
        {
            is_stable = true;
            break;
        }
        else
        {
            std::swap(current_coloring, next_coloring);
        }

        if (num_iterations == max_num_iterations) {
            break;
        }
    }

    auto [unique, counts] = get_frequencies(current_coloring);
    lexical_sort(unique, counts);
    return { is_stable, num_iterations, std::move(unique), std::move(counts) };
}

inline static int index_of_pair(int first_node, int second_node, int num_nodes) { return first_node * num_nodes + second_node; }

int WeisfeilerLeman::get_subgraph_color(int src_node, int dst_node, const Graph& graph)
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

    // Graph labels must be non-negative and colors will always be positive.
    // We make the graph labels negative so that they are not confused with colors.

    return get_new_color({ -pairing_function(src_label, dst_label) - 1, std::move(forward_colors), std::move(backward_colors) });
}

std::tuple<bool, size_t, std::vector<int>, std::vector<int>> WeisfeilerLeman::k2_fwl(const Graph& graph, size_t max_num_iterations)
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

    size_t num_iterations = 0;
    bool is_stable = false;

    while (true)
    {
        ++num_iterations;

        for (int i = 0; i < num_nodes; ++i)
        {
            for (int j = 0; j < num_nodes; ++j)
            {
                auto compositions = std::vector<AdjacentColor>(num_nodes);

                const auto ij_index = index_of_pair(i, j, num_nodes);
                const auto ij_color = current_coloring[ij_index];

                for (int k = 0; k < num_nodes; ++k)
                {
                    const auto ik_index = index_of_pair(i, k, num_nodes);
                    const auto kj_index = index_of_pair(k, j, num_nodes);

                    const auto ik_color = current_coloring[ik_index];
                    const auto kj_color = current_coloring[kj_index];

                    compositions[k] = { ik_color, kj_color };
                }

                next_coloring[ij_index] = get_new_color({ ij_color, std::move(compositions), {} });
            }
        }

        if (test_fixpoint(current_coloring, next_coloring))
        {
            is_stable = true;
            break;
        }
        else
        {
            std::swap(current_coloring, next_coloring);
        }

        if (num_iterations == max_num_iterations) {
            break;
        }
    }

    auto [unique, counts] = get_frequencies(current_coloring);
    lexical_sort(unique, counts);
    return { is_stable, num_iterations, std::move(unique), std::move(counts) };
}

std::tuple<bool, size_t, std::vector<int>, std::vector<int>> WeisfeilerLeman::compute_coloring(const Graph& graph, size_t max_num_iterations)
{
    if (m_k == 1)
    {
        return k1_fwl(graph, max_num_iterations);
    }

    if (m_k == 2)
    {
        return k2_fwl(graph, max_num_iterations);
    }

    throw std::invalid_argument("k must be either 1 or 2");
}

size_t WeisfeilerLeman::get_coloring_function_size() const { return m_color_function.size(); }

}

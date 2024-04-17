#include "wl/details/weisfeiler_leman.hpp"

#include <cassert>
#include <exception>
#include <numeric>
#include <stdexcept>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace wl
{

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

    std::vector<size_t> indices(items1.size());
    std::iota(indices.begin(), indices.end(), 0);

    // Sort the indices based on corresponding values in items1 and items2
    std::sort(indices.begin(), indices.end(), [&](size_t i, size_t j) { return std::tie(items1[i], items2[i]) < std::tie(items1[j], items2[j]); });

    // Reorder items1 and items2 according to the sorted indices in-place
    for (size_t i = 0; i < indices.size(); ++i)
    {
        while (i != indices[i])
        {
            size_t next = indices[i];
            std::swap(items1[i], items1[next]);
            std::swap(items2[i], items2[next]);
            std::swap(indices[i], indices[next]);
        }
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

int WeisfeilerLeman::get_color_k1(ColorMultiset&& key)
{
    auto it = m_color_function.find(key);

    if (it == m_color_function.end())
    {
        int color = static_cast<int>(m_color_function.size());
        m_color_function.emplace(std::move(key), color);
        return color;
    }

    return it->second;
}

std::tuple<int, std::vector<int>, std::vector<int>> WeisfeilerLeman::compute_coloring_k1(const Graph& graph)
{
    auto num_nodes = graph.get_num_nodes();
    auto current_coloring = std::vector<int>(num_nodes);
    auto next_coloring = std::vector<int>(num_nodes);

    for (int node = 0; node < num_nodes; ++node)
    {
        // Graph labels must be non-negative and colors will always be positive.
        // We make the graph labels negative so that they are not confused with colors.

        auto node_label = -graph.get_node_label(node) - 1;
        current_coloring[node] = get_color_k1(ColorMultiset(node_label, {}, {}, {}, {}));
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

            next_coloring[node] = get_color_k1(ColorMultiset(current_coloring[node],
                                                             std::move(ingoing_edge_colors),
                                                             std::move(outgoing_edge_colors),
                                                             std::move(ingoing_node_colors),
                                                             std::move(outgoing_node_colors)));
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

std::tuple<int, std::vector<int>, std::vector<int>> WeisfeilerLeman::compute_coloring_k2(const Graph& graph) { throw std::exception(); }

std::tuple<int, std::vector<int>, std::vector<int>> WeisfeilerLeman::compute_coloring(const Graph& graph)
{
    if (m_k == 1)
    {
        return compute_coloring_k1(graph);
    }

    if (m_k == 2)
    {
        return compute_coloring_k2(graph);
    }

    throw std::invalid_argument("k must be either 1 or 2");
}

}

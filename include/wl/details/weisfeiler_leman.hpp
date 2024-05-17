#ifndef WL_DETAILS_WEISFEILER_LEMAN_HPP_
#define WL_DETAILS_WEISFEILER_LEMAN_HPP_

#include "wl/details/graph.hpp"

#include <map>
#include <tuple>
#include <vector>
#include <limits>

namespace wl
{

using Color = int;
using AdjacentColor = std::pair<Color, Color>;
using NodeColorContext = std::tuple<Color, std::vector<AdjacentColor>, std::vector<AdjacentColor>>;

class WeisfeilerLeman
{
private:
    std::map<NodeColorContext, Color> m_color_function;
    int m_k;
    bool m_ignore_counting;

    std::vector<Color> get_colors(const std::vector<Color>& colors, const std::vector<int>& indices);

    std::vector<AdjacentColor> get_colors_pairs(const std::vector<Color>& node_colors,
                                                   const std::vector<int>& node_indices,
                                                   const std::vector<Color>& edge_colors,
                                                   const std::vector<int>& edge_indices);

    Color get_new_color(NodeColorContext&& color_multiset);

    Color get_subgraph_color(int src_node, int dst_node, const Graph& graph);

    std::tuple<int, std::vector<int>, std::vector<int>> k1_fwl(const Graph& graph, size_t max_num_iterations);

    std::tuple<int, std::vector<int>, std::vector<int>> k2_fwl(const Graph& graph, size_t max_num_iterations);

public:
    explicit WeisfeilerLeman(int k);

    explicit WeisfeilerLeman(int k, bool ignore_counting);

    int get_k() const;

    bool get_ignore_counting() const;

    std::tuple<int, std::vector<int>, std::vector<int>> compute_coloring(const Graph& graph, size_t max_num_iterations = std::numeric_limits<size_t>::max());

    size_t get_coloring_function_size() const;
};

}

#endif

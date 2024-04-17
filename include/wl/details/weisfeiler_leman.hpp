#ifndef WL_DETAILS_WEISFEILER_LEMAN_HPP_
#define WL_DETAILS_WEISFEILER_LEMAN_HPP_

#include "wl/details/graph.hpp"

#include <map>
#include <tuple>
#include <vector>

namespace wl
{

class WeisfeilerLeman
{
private:
    using ColorMultiset = std::tuple<int, std::vector<int>, std::vector<int>, std::vector<int>, std::vector<int>>;

    std::map<ColorMultiset, int> m_color_function;
    int m_k;

    int get_color(ColorMultiset&& key);

    int get_subgraph_color(int src_node, int dst_node, const Graph& graph);

    std::tuple<int, std::vector<int>, std::vector<int>> k1_fwl(const Graph& graph);

    std::tuple<int, std::vector<int>, std::vector<int>> k2_fwl(const Graph& graph);

public:
    explicit WeisfeilerLeman(int k);

    std::tuple<int, std::vector<int>, std::vector<int>> compute_coloring(const Graph& graph);
};

}

#endif

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

    int get_color_k1(ColorMultiset&& key);

    std::tuple<int, std::vector<int>, std::vector<int>> compute_coloring_k1(const Graph& graph);

    std::tuple<int, std::vector<int>, std::vector<int>> compute_coloring_k2(const Graph& graph);

public:
    explicit WeisfeilerLeman(int k);

    std::tuple<int, std::vector<int>, std::vector<int>> compute_coloring(const Graph& graph);
};

}

#endif

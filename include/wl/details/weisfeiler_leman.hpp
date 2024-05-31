#ifndef WL_DETAILS_WEISFEILER_LEMAN_HPP_
#define WL_DETAILS_WEISFEILER_LEMAN_HPP_

#include "wl/details/edge_colored_graph.hpp"
#include "wl/details/weisfeiler_leman_1d.hpp"
#include "wl/details/weisfeiler_leman_2d.hpp"

#include <limits>
#include <tuple>
#include <vector>

namespace wl
{

using Color = int;
using AdjacentColor = std::pair<Color, Color>;
using NodeColorContext = std::tuple<Color, std::vector<AdjacentColor>, std::vector<AdjacentColor>>;

class WeisfeilerLeman
{
private:
    int m_k;
    WeisfeilerLeman1D m_1wl;
    WeisfeilerLeman2D m_2wl;

public:
    explicit WeisfeilerLeman(int k);

    explicit WeisfeilerLeman(int k, bool ignore_counting);

    /* Getters */

    int get_k() const;

    bool get_ignore_counting() const;

    size_t get_coloring_function_size() const;

    /* Simple interface to run k-WL for at most max_num_iterations or until convergence. */

    std::tuple<bool, size_t, std::vector<int>, std::vector<int>> compute_coloring(const EdgeColoredGraph& graph,
                                                                                  size_t max_num_iterations = std::numeric_limits<size_t>::max());

    /* Expert interface with more control over the execution */

    GraphColoring compute_initial_coloring(const EdgeColoredGraph& graph);

    bool compute_next_coloring(const EdgeColoredGraph& graph, const GraphColoring& current_coloring, GraphColoring& ref_next_coloring);
};

}

#endif

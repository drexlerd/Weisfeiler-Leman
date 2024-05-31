#ifndef WL_DETAILS_WEISFEILER_LEMAN_1D_HPP_
#define WL_DETAILS_WEISFEILER_LEMAN_1D_HPP_

#include "wl/details/edge_colored_graph.hpp"

#include <limits>
#include <map>
#include <tuple>
#include <vector>

namespace wl
{

using Color = int;
using AdjacentColor = std::pair<Color, Color>;
using NodeColorContext = std::tuple<Color, std::vector<AdjacentColor>, std::vector<AdjacentColor>>;

class WeisfeilerLeman1D
{
private:
    std::map<NodeColorContext, Color> m_color_function;
    bool m_ignore_counting;

    std::vector<AdjacentColor> get_colors_pairs(const std::vector<Color>& node_colors,
                                                const std::vector<int>& node_indices,
                                                const std::vector<Color>& edge_colors,
                                                const std::vector<int>& edge_indices);

    Color get_new_color(NodeColorContext&& color_multiset);

public:
    explicit WeisfeilerLeman1D();

    explicit WeisfeilerLeman1D(bool ignore_counting);

    /* Getters */

    size_t get_coloring_function_size() const;

    bool get_ignore_counting() const;

    /* Simple interface to run k-WL for at most max_num_iterations or until convergence. */

    std::tuple<bool, size_t, std::vector<int>, std::vector<int>> compute_coloring(const EdgeColoredGraph& graph,
                                                                                  size_t max_num_iterations = std::numeric_limits<size_t>::max());

    /* Expert interface with more control over the execution */

    /// @brief Compute the initial coloring of the graph based on the node labels.
    /// Returns a GraphColoring object.
    GraphColoring compute_initial_coloring(const EdgeColoredGraph& graph);

    /// @brief One step of updating the 1-WL coloring.
    /// Return true iff the coloring has stabilized.
    bool compute_next_coloring(const EdgeColoredGraph& graph, const GraphColoring& current_coloring, GraphColoring& ref_next_coloring);
};

}

#endif

/**
 * An implementation of Berkholz, Bonsma, and Grohe's O((m+n)log n) algorithm for computing a canonical coloring.
 *
 * Credits to Blai Bonet: https://github.com/bonetblai/canonical-coloring
 */

#ifndef WL_DETAILS_CANONICAL_COLOR_REFINEMENT_HPP_
#define WL_DETAILS_CANONICAL_COLOR_REFINEMENT_HPP_

#include "wl/details/edge_colored_graph.hpp"
#include "wl/details/printer.hpp"

#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <tuple>
#include <vector>

namespace wl
{

class CanonicalColorRefinement
{
protected:
    int debug_;
    bool use_stack_;

    std::vector<std::set<int>> C_;     // Indexed by color. Partition. C[c] is set of vertices with color c
    std::vector<std::vector<int>> A_;  // Indexed by color. A[c] is vertices of color c adjacent to vertices of color r
    std::vector<int> colour_;          // Indexed by vertex. colour[v] is color of vertex v
    std::vector<int> cdeg_;            // Indexed by vertex. Color degree d^+_r(v) = |N^+(v) \cap C_r| (number of out-neighbors of vertex v of color r)
    std::vector<int> maxcdeg_;         // Indexed by color. maxcdeg[c] = max { d^+_r(v) : v is of color c }
    std::vector<int> mincdeg_;         // Indexed by color. mincdeg[c] = min { d^+_r(v) : v is of color c }

    bool valid_QM_;                     // Whether the factor matrix was computed
    std::vector<std::vector<int>> QM_;  // Factor matrix

    int k_;
    std::deque<int> s_refine_;
    std::vector<bool> in_s_refine_;

    void split_up_color(int s);

    void calculate_quotient_matrix(const EdgeColoredGraph& graph);

public:
    CanonicalColorRefinement(int debug = 0, bool use_stack = false) : debug_(debug), use_stack_(use_stack), valid_QM_(false) {}
    ~CanonicalColorRefinement() {}

    /// @brief Calculate the canonical equitable partition of a vertex colored graph.
    /// @param graph
    /// @param factor_matrix
    void calculate(const EdgeColoredGraph& graph, bool calculate_qm = false);

    /**
     * Getters
     */

    const std::vector<std::set<int>>& get_coloring() const;
    const std::vector<std::vector<int>>& get_quotient_matrix() const;

    /**
     * Setters
     */

    void set_debug(int debug);
    void set_use_stack(bool use_stack);

    /**
     * Translators
     */

    static int check_coloring(const std::vector<int>& alpha, bool verbose = true);
    static std::vector<int> coloring_to_histogram(const std::vector<std::set<int>>& partition);
};

}

#endif

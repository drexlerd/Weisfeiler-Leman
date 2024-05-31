#ifndef WL_DETAILS_CANONICAL_COLOR_REFINEMENT_HPP_
#define WL_DETAILS_CANONICAL_COLOR_REFINEMENT_HPP_

#include "wl/details/edge_colored_graph.hpp"
#include "wl/details/printer.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <tuple>
#include <vector>

namespace wl
{
template<typename T>
class Stack
{
protected:
    std::vector<T> stack_;

public:
    Stack() {}
    ~Stack() {}

    void initialize(int n)
    {
        stack_.resize(0);
        stack_.reserve(n);
    }

    bool empty() const { return stack_.empty(); }
    const T& top() const
    {
        if (!stack_.empty())
            return stack_.back();
        else
            throw std::runtime_error("Attempt top() on empty stack");
    }
    T pop()
    {
        if (!stack_.empty())
        {
            T item = stack_.back();
            stack_.pop_back();
            return item;
        }
        else
        {
            throw std::runtime_error("Attempt pop() on empty stack");
        }
    }
    int push(const T item)
    {
        stack_.push_back(item);
        return stack_.back();
    }

    void print(std::ostream& os) const { os << stack_; }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Stack<T>& stack)
{
    stack.print(os);
    return os;
}

using ColorHistogram = std::vector<int>;

class CanonicalColorRefinement
{
protected:
    const int debug_;

    std::vector<std::set<int>> C_;     // Indexed by color. Partition. C[c] is set of vertices with color c
    std::vector<std::vector<int>> A_;  // Indexed by color. A[c] is vertices of color c adjacent to vertices of color r
    std::vector<int> colour_;          // Indexed by vertex. colour[v] is color of vertex v
    std::vector<int> cdeg_;            // Indexed by vertex. Color degree d^+_r(v) = |N^+(v) \cap C_r| (number of out-neighbors of vertex v of color r)
    std::vector<int> maxcdeg_;         // Indexed by color. maxcdeg[c] = max { d^+_r(v) : v is of color c }
    std::vector<int> mincdeg_;         // Indexed by color. mincdeg[c] = min { d^+_r(v) : v is of color c }

    int k_;
    Stack<int> s_refine_;
    std::vector<bool> in_s_refine_;

    void split_up_color(int s);

public:
    CanonicalColorRefinement(int debug = 0) : debug_(debug) {}
    ~CanonicalColorRefinement() {}

    std::vector<std::set<int>> calculate(const EdgeColoredGraph& graph);

    static ColorHistogram histogram(const std::vector<std::set<int>>& partition)
    {
        ColorHistogram hist;
        for (size_t i = 0; i < partition.size(); ++i)
            hist.push_back(partition[i].size());
        return hist;
    }

    int get_coloring_function_size() const { return k_; }
};

}

#endif

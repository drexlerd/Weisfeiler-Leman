#ifndef WL_DETAILS_UTILS_HPP_
#define WL_DETAILS_UTILS_HPP_

#include <algorithm>
#include <cassert>
#include <climits>
#include <stdexcept>
#include <vector>

namespace wl
{

inline int safe_multiply(int x, int y)
{
    if (x != 0 && (y > INT_MAX / x || y < INT_MIN / x))
    {
        throw std::overflow_error("Overflow detected in multiplication");
    }
    return x * y;
}

inline int pairing_function(int x, int y)
{
    int x_sq, y_sq;
    if (x >= y)
    {
        x_sq = safe_multiply(x, x);  // Safe multiplication
        int x_sq_plus_x = x_sq + x;  // Add x
        if (x_sq_plus_x < x_sq)
            throw std::overflow_error("Overflow in addition");
        return x_sq_plus_x + y;  // Add y
    }
    else
    {
        y_sq = safe_multiply(y, y);  // Safe multiplication
        if (y_sq < y)
            throw std::overflow_error("Overflow in addition");
        return y_sq + x;  // Add x
    }
}

inline void lexical_sort(std::vector<int>& items1, std::vector<int>& items2)
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

}

#endif

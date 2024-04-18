#ifndef WL_DETAILS_GRAPH_HPP_
#define WL_DETAILS_GRAPH_HPP_

#include <unordered_map>
#include <vector>

namespace wl
{

class Graph
{
private:
    std::vector<std::vector<int>> m_outgoing_edges;
    std::vector<std::vector<int>> m_ingoing_edges;
    std::vector<std::vector<int>> m_outgoing_adjacent;
    std::vector<std::vector<int>> m_ingoing_adjacent;
    std::vector<std::unordered_map<int, std::vector<int>>> m_outgoing_edges_between;
    std::vector<std::unordered_map<int, std::vector<int>>> m_ingoing_edges_between;
    std::vector<int> m_node_labels;
    std::vector<int> m_edge_labels;
    bool m_directed;

public:
    explicit Graph(bool directed);

    int add_node(int label = 0);

    void add_edge(int src_node, int dst_node, int label = 0);

    const std::vector<int>& get_outbound_edges(int node) const;

    const std::vector<int>& get_inbound_edges(int node) const;

    const std::vector<int>& get_outbound_adjacent(int node) const;

    const std::vector<int>& get_inbound_adjacent(int node) const;

    int get_num_nodes() const;

    int get_num_edges() const;

    int get_node_label(int node) const;

    int get_edge_label(int edge) const;

    const std::vector<int>& get_node_labels() const;

    const std::vector<int>& get_edge_labels() const;

    const std::vector<int>& get_edges(int src_node, int dst_node) const;

    bool is_directed() const;
};

}

#endif

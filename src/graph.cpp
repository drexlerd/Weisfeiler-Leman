#include "wl/details/graph.hpp"

#include <stdexcept>
#include <vector>

namespace wl
{

Graph::Graph(bool directed) :
    m_outgoing_edges(),
    m_ingoing_edges(),
    m_outgoing_adjacent(),
    m_ingoing_adjacent(),
    m_outgoing_edges_between(),
    m_ingoing_edges_between(),
    m_node_labels(),
    m_edge_labels(),
    m_directed(directed)
{
}

int Graph::add_node(int label)
{
    if (label < 0)
    {
        throw std::invalid_argument("label must be non-negative");
    }

    int node = get_num_nodes();
    m_outgoing_edges.emplace_back();
    m_ingoing_edges.emplace_back();
    m_outgoing_adjacent.emplace_back();
    m_ingoing_adjacent.emplace_back();
    m_node_labels.emplace_back(label);

    auto& outgoing_adjacent_between = m_outgoing_edges_between.emplace_back();
    auto& ingoing_adjacent_between = m_ingoing_edges_between.emplace_back();

    for (int other_node = 0; other_node < node; ++other_node)
    {
        m_outgoing_edges_between.at(other_node).emplace(node, std::vector<int>());
        m_ingoing_edges_between.at(other_node).emplace(node, std::vector<int>());
        outgoing_adjacent_between.emplace(other_node, std::vector<int>());
        ingoing_adjacent_between.emplace(other_node, std::vector<int>());
    }

    outgoing_adjacent_between.emplace(node, std::vector<int>());
    ingoing_adjacent_between.emplace(node, std::vector<int>());

    return node;
}

void Graph::add_edge(int src_node, int dst_node, int label)
{
    if (label < 0)
    {
        throw std::invalid_argument("label must be non-negative");
    }

    {  // Add src_node -> dst_node
        int edge = get_num_edges();
        m_outgoing_edges.at(src_node).emplace_back(edge);
        m_ingoing_edges.at(dst_node).emplace_back(edge);
        m_outgoing_adjacent.at(src_node).emplace_back(dst_node);
        m_ingoing_adjacent.at(dst_node).emplace_back(src_node);
        m_outgoing_edges_between.at(src_node).at(dst_node).emplace_back(edge);
        m_ingoing_edges_between.at(dst_node).at(src_node).emplace_back(edge);
        m_edge_labels.emplace_back(label);
    }

    if (!m_directed)  // Add dst_node -> src_node
    {
        int edge = get_num_edges();
        m_outgoing_edges.at(dst_node).emplace_back(edge);
        m_ingoing_edges.at(src_node).emplace_back(edge);
        m_outgoing_adjacent.at(dst_node).emplace_back(src_node);
        m_ingoing_adjacent.at(src_node).emplace_back(dst_node);
        m_outgoing_edges_between.at(dst_node).at(src_node).emplace_back(edge);
        m_ingoing_edges_between.at(src_node).at(dst_node).emplace_back(edge);
        m_edge_labels.emplace_back(label);
    }
}

const std::vector<int>& Graph::get_outbound_edges(int node) const { return m_outgoing_edges.at(node); }

const std::vector<int>& Graph::get_inbound_edges(int node) const { return m_ingoing_edges.at(node); }

const std::vector<int>& Graph::get_outbound_adjacent(int node) const { return m_outgoing_adjacent.at(node); }

const std::vector<int>& Graph::get_inbound_adjacent(int node) const { return m_ingoing_adjacent.at(node); }

int Graph::get_num_nodes() const { return static_cast<int>(m_node_labels.size()); }

int Graph::get_num_edges() const { return static_cast<int>(m_edge_labels.size()); }

int Graph::get_node_label(int node) const { return m_node_labels.at(node); }

int Graph::get_edge_label(int edge) const { return m_edge_labels.at(edge); }

const std::vector<int>& Graph::get_node_labels() const { return m_node_labels; }

const std::vector<int>& Graph::get_edge_labels() const { return m_edge_labels; }

const std::vector<int>& Graph::get_edges(int src_node, int dst_node) const { return m_outgoing_edges_between.at(src_node).at(dst_node); }

bool Graph::is_directed() const { return m_directed; }
}

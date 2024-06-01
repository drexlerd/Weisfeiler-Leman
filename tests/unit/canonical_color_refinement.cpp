#include "wl/details/canonical_color_refinement.hpp"

#include <gtest/gtest.h>

namespace wl::tests
{

TEST(WLTests, Canonical)
{
    auto color_refinement = CanonicalColorRefinement(0);

    // 2024-05-31 14:28:09,314 -  > Cost: 4; State 1: [<Atom 'room(rooma)'>, <Atom 'room(roomb)'>, <Atom 'gripper(left)'>, <Atom 'gripper(right)'>, <Atom
    // 'ball(ball1)'>, <Atom 'free(left)'>, <Atom 'free(right)'>, <Atom 'at(ball1, rooma)'>, <Atom 'at-robby(roomb)'>] 2024-05-31 14:28:09,314 -  > Cost: 0;
    // State 2: [<Atom 'room(rooma)'>, <Atom 'room(roomb)'>, <Atom 'gripper(left)'>, <Atom 'gripper(right)'>, <Atom 'ball(ball1)'>, <Atom 'free(left)'>, <Atom
    // 'free(right)'>, <Atom 'at-robby(rooma)'>, <Atom 'at(ball1, roomb)'>]

    /**
     * State 1
     */
    // Num nodes: 17
    // Num edges: 28
    // Node colors: [1 1 1 1 1 2 2 3 3 4 5 5 6 7 8 9 10]
    // Undirected edges
    // 0 : [5 13]
    // 1 : [6 14 16]
    // 2 : [7 10]
    // 3 : [8 11]
    // 4 : [9 12 15]
    // 5 : []
    // 6 : []
    // 7 : []
    // 8 : []
    // 9 : []
    // 10 : []
    // 11 : []
    // 12 : [13]
    // 13 : []
    // 14 : []
    // 15 : [16]
    // 16 : []
    auto graph1 = EdgeColoredGraph(false);
    graph1.add_node(1);
    graph1.add_node(1);
    graph1.add_node(1);
    graph1.add_node(1);
    graph1.add_node(1);
    graph1.add_node(2);
    graph1.add_node(2);
    graph1.add_node(3);
    graph1.add_node(3);
    graph1.add_node(4);
    graph1.add_node(5);
    graph1.add_node(5);
    graph1.add_node(6);
    graph1.add_node(7);
    graph1.add_node(8);
    graph1.add_node(9);
    graph1.add_node(10);
    graph1.add_edge(0, 5);
    graph1.add_edge(0, 13);
    graph1.add_edge(1, 6);
    graph1.add_edge(1, 14);
    graph1.add_edge(1, 16);
    graph1.add_edge(2, 7);
    graph1.add_edge(2, 10);
    graph1.add_edge(3, 8);
    graph1.add_edge(3, 11);
    graph1.add_edge(4, 9);
    graph1.add_edge(4, 12);
    graph1.add_edge(4, 15);
    graph1.add_edge(12, 13);
    graph1.add_edge(15, 16);

    color_refinement.calculate(graph1, true);
    auto factor_matrix = color_refinement.get_factor_matrix();

    /**
     * State 2
     */
    // Num nodes: 17
    // Num edges: 28
    // Node colors: [1 1 1 1 1 2 2 3 3 4 5 5 6 7 8 9 10]
    // Undirected edges
    // 0 : [5 12]
    // 1 : [6 14 16]
    // 2 : [7 10]
    // 3 : [8 11]
    // 4 : [9 13 15]
    // 5 : []
    // 6 : []
    // 7 : []
    // 8 : []
    // 9 : []
    // 10 : []
    // 11 : []
    // 12 : []
    // 13 : [14]
    // 14 : []
    // 15 : [16]
    // 16 : []

    auto graph2 = EdgeColoredGraph(false);
    graph2.add_node(1);
    graph2.add_node(1);
    graph2.add_node(1);
    graph2.add_node(1);
    graph2.add_node(1);
    graph2.add_node(2);
    graph2.add_node(2);
    graph2.add_node(3);
    graph2.add_node(3);
    graph2.add_node(4);
    graph2.add_node(5);
    graph2.add_node(5);
    graph2.add_node(6);
    graph2.add_node(7);
    graph2.add_node(8);
    graph2.add_node(9);
    graph2.add_node(10);
    graph2.add_edge(0, 5);
    graph2.add_edge(0, 12);
    graph2.add_edge(1, 6);
    graph2.add_edge(1, 14);
    graph2.add_edge(1, 16);
    graph2.add_edge(2, 7);
    graph2.add_edge(2, 10);
    graph2.add_edge(3, 8);
    graph2.add_edge(3, 11);
    graph2.add_edge(4, 9);
    graph2.add_edge(4, 13);
    graph2.add_edge(4, 15);
    graph2.add_edge(13, 14);
    graph2.add_edge(15, 16);

    auto color_refinement2 = CanonicalColorRefinement(false);
    color_refinement2.calculate(graph2, true);
    auto factor_matrix_2 = color_refinement2.get_factor_matrix();

    EXPECT_NE(factor_matrix, factor_matrix_2);
}

}

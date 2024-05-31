#include "wl/details/canonical_color_refinement.hpp"

#include <gtest/gtest.h>

namespace wl::tests
{

TEST(WLTests, Canonical)
{
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
    // 5 : [0]
    // 6 : [1]
    // 7 : [2]
    // 8 : [3]
    // 9 : [4]
    // 10 : [2]
    // 11 : [3]
    // 12 : [4 13]
    // 13 : [0 12]
    // 14 : [1]
    // 15 : [4 16]
    // 16 : [1 15]
    auto graph1 = EdgeColoredGraph(true);
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
    graph1.add_edge(5, 0);
    graph1.add_edge(6, 1);
    graph1.add_edge(7, 2);
    graph1.add_edge(8, 3);
    graph1.add_edge(9, 4);
    graph1.add_edge(10, 2);
    graph1.add_edge(11, 3);
    graph1.add_edge(12, 4);
    graph1.add_edge(12, 13);
    graph1.add_edge(13, 0);
    graph1.add_edge(13, 12);
    graph1.add_edge(14, 1);
    graph1.add_edge(15, 4);
    graph1.add_edge(15, 16);
    graph1.add_edge(16, 1);
    graph1.add_edge(16, 15);

    auto color_refinement = CanonicalColorRefinement(false);
    auto histogram1 = CanonicalColorRefinement::histogram(color_refinement.calculate(graph1));

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
    // 5 : [0]
    // 6 : [1]
    // 7 : [2]
    // 8 : [3]
    // 9 : [4]
    // 10 : [2]
    // 11 : [3]
    // 12 : [0]
    // 13 : [4 14]
    // 14 : [1 13]
    // 15 : [4 16]
    // 16 : [1 15]

    auto graph2 = EdgeColoredGraph(true);
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
    graph2.add_edge(5, 0);
    graph2.add_edge(6, 1);
    graph2.add_edge(7, 2);
    graph2.add_edge(8, 3);
    graph2.add_edge(9, 4);
    graph2.add_edge(10, 2);
    graph2.add_edge(11, 3);
    graph2.add_edge(12, 0);
    graph2.add_edge(13, 4);
    graph2.add_edge(13, 14);
    graph2.add_edge(14, 1);
    graph2.add_edge(14, 13);
    graph2.add_edge(15, 4);
    graph2.add_edge(15, 16);
    graph2.add_edge(16, 1);
    graph2.add_edge(16, 15);

    auto color_refinement2 = CanonicalColorRefinement(false);
    auto histogram2 = CanonicalColorRefinement::histogram(color_refinement2.calculate(graph2));

    EXPECT_NE(histogram1, histogram2);
}

}

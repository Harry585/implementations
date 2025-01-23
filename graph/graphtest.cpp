#include "graph.cpp"
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <cassert>
#include <climits>
#include <tuple>
#include <algorithm>
#include <stdexcept>
#include <queue>

// Function to test max flow calculations
void test_max_flow() {
    // Test Case 1: Simple Graph
    {
        // Graph structure:
        // 0 -> 1 (capacity 10)
        // 0 -> 2 (capacity 5)
        // 1 -> 2 (capacity 15)
        // 1 -> 3 (capacity 10)
        // 2 -> 3 (capacity 10)
        WeightedGraph graph(4, {
            {0, 1, 10},
            {0, 2, 5},
            {1, 2, 15},
            {1, 3, 10},
            {2, 3, 10}
        });

        int max_flow = graph.edmondsKarp(0, 3);
        // Expected max flow: 15
        // Path 0->1->3: flow 10
        // Path 0->2->3: flow 5
        assert(max_flow == 15);
        std::cout << "Test Case 1 Passed: Simple Graph Max Flow = " << max_flow << std::endl;
    }

    // Test Case 2: Multiple Paths
    {
        // Graph structure:
        // 0 -> 1 (capacity 4)
        // 0 -> 2 (capacity 2)
        // 1 -> 2 (capacity 1)
        // 1 -> 3 (capacity 2)
        // 2 -> 4 (capacity 3)
        // 3 -> 4 (capacity 4)
        WeightedGraph graph(5, {
            {0, 1, 4},
            {0, 2, 2},
            {1, 2, 1},
            {1, 3, 2},
            {2, 4, 3},
            {3, 4, 4}
        });

        int max_flow = graph.edmondsKarp(0, 4);
        // Expected max flow: 6
        // Path 0->1->3->4: flow 2
        // Path 0->1->2->4: flow 1
        // Path 0->2->4: flow 2
        assert(max_flow == 5);
        std::cout << "Test Case 2 Passed: Multiple Paths Max Flow = " << max_flow << std::endl;
    }

    // Test Case 3: No Flow (Disconnected Source and Sink)
    {
        // Graph structure:
        // 0 -> 1 (capacity 10)
        // 2 -> 3 (capacity 10)
        WeightedGraph graph(4, {
            {0, 1, 10},
            {2, 3, 10}
        });

        int max_flow = graph.edmondsKarp(0, 3);
        // Expected max flow: 0 (no path from 0 to 3)
        assert(max_flow == 0);
        std::cout << "Test Case 3 Passed: Disconnected Graph Max Flow = " << max_flow << std::endl;
    }

    // Test Case 4: No Flow (Source equals Sink)
    {
        WeightedGraph graph(3, {
            {0, 1, 5},
            {1, 2, 5}
        });

        try {
            graph.edmondsKarp(1, 1);
            // Should throw an exception
            assert(false); // Should not reach here
        } catch (const std::invalid_argument& e) {
            std::cout << "Test Case 4 Passed: Exception caught for source equals sink." << std::endl;
        } catch (...) {
            // Catch any other exceptions
            assert(false);
        }
    }

    // Test Case 5: Complex Graph
    {
        // Graph structure based on the classic Ford-Fulkerson example
        // 0 -> 1 (capacity 16)
        // 0 -> 2 (capacity 13)
        // 1 -> 2 (capacity 10)
        // 1 -> 3 (capacity 12)
        // 2 -> 1 (capacity 4)
        // 2 -> 4 (capacity 14)
        // 3 -> 2 (capacity 9)
        // 3 -> 5 (capacity 20)
        // 4 -> 3 (capacity 7)
        // 4 -> 5 (capacity 4)
        WeightedGraph graph(6, {
            {0, 1, 16},
            {0, 2, 13},
            {1, 2, 10},
            {1, 3, 12},
            {2, 1, 4},
            {2, 4, 14},
            {3, 2, 9},
            {3, 5, 20},
            {4, 3, 7},
            {4, 5, 4}
        });

        int max_flow = graph.edmondsKarp(0, 5);
        // Expected max flow: 23
        // Paths:
        // 0->1->3->5: flow 12
        // 0->2->4->3->5: flow 11
        // Total: 23
        assert(max_flow == 23);
        std::cout << "Test Case 5 Passed: Complex Graph Max Flow = " << max_flow << std::endl;
    }

    // Test Case 6
    {
        // Initial Graph:
        // 0 -> 1 (capacity 10)
        // 0 -> 2 (capacity 10)
        // 1 -> 2 (capacity 2)
        // 1 -> 3 (capacity 4)
        // 2 -> 3 (capacity 10)
        WeightedGraph graph(4, {
            {0, 1, 10},
            {0, 2, 10},
            {1, 2, 2},
            {1, 3, 4},
            {2, 3, 10}
        });

        int max_flow_initial = graph.edmondsKarp(0, 3);
        // Expected max flow: 14
        // Paths:
        // 0->1->3: flow 4
        // 0->2->3: flow 10
        // Total: 14
        assert(max_flow_initial == 14);
        std::cout << "Test Case 6 Passed: Max Flow = " << max_flow_initial << std::endl;    }

    // Test Case 7: Adding Duplicate Edge
    {
        WeightedGraph graph(3, {
            {0, 1, 5},
            {1, 2, 5}
        });

        // Attempt to add duplicate edge 0->1
        bool added = graph.addEdge({0, 1, 5});
        assert(added == false); // Should not add duplicate
        std::cout << "Test Case 7 Passed: Duplicate Edge Not Added." << std::endl;
    }

    // Test Case 8: Invalid Edge Addition
    {
        WeightedGraph graph(3, {
            {0, 1, 5},
            {1, 2, 5}
        });

        // Attempt to add an edge with invalid node
        try {
            graph.addEdge({-1, 2, 5});
            // Should throw an exception
            assert(false); // Should not reach here
        } catch (const std::invalid_argument& e) {
            std::cout << "Test Case 8 Passed: Exception caught for invalid edge addition." << std::endl;
        } catch (...) {
            // Catch any other exceptions
            assert(false);
        }

        // Attempt to add an edge with zero weight
        try {
            graph.addEdge({0, 2, 0});
            // Should throw an exception
            assert(false); // Should not reach here
        } catch (const std::invalid_argument& e) {
            std::cout << "Test Case 8 Passed: Exception caught for zero weight edge." << std::endl;
        } catch (...) {
            // Catch any other exceptions
            assert(false);
        }
    }

    // Test Case 9: Large Graph
    {
        // Creating a larger graph to test performance and correctness
        int size = 6;
        std::initializer_list<WeightedGraph::Edge> edges = {
            {0, 1, 16}, {0, 2, 13}, {1, 2, 10}, {1, 3, 12},
            {2, 1, 4}, {2, 4, 14}, {3, 2, 9}, {3, 5, 20},
            {4, 3, 7}, {4, 5, 4}
        };
        WeightedGraph graph(size, edges);

        int max_flow = graph.edmondsKarp(0, 5);
        // Expected max flow: 23
        assert(max_flow == 23);
        std::cout << "Test Case 9 Passed: Large Graph Max Flow = " << max_flow << std::endl;
    }

    std::cout << "All Test Cases Passed Successfully!" << std::endl;
}
void test_undirected_graph() {
    std::cout << "Constructing graph from initializer list" << std::endl;
    UndirectedGraph g = UndirectedGraph(5, {{1,2},{2,3},{3,1}});
    assert(g.neighbours(1) == std::vector<int>({2,3}));
    std::cout << "Adding edges" << std::endl;
    assert(g.addEdge({4,1}));
    assert(!g.addEdge({1,4}));
    // find neighbours by getting sorted vectors
    std::cout << "Getting neighbours" << std::endl;
    assert(g.neighbours(2) == std::vector<int>({1,3}));
    assert(g.neighbours(1) == std::vector<int>({2,3,4}));
    assert(g.neighbours(0) == std::vector<int>());
    std::cout << "Removing edge" << std::endl;
    assert(g.deleteEdge({1,4}));
    assert(!g.deleteEdge({1,4}));
    assert(g.neighbours(1) == std::vector<int>({2,3}));
    std::cout << "Constructing graph test passed" << std::endl;
}   

void test_search() {
    std::cout << "Constructing graph for BFS and DFS" << std::endl;
    /* Graph looks like:
    5 - 2 - 6
        |   |   8
    0 - 1 - 4
            |
            3 - 7
    */
    UndirectedGraph g = UndirectedGraph(9, {{5,2},{2,6},{0,1},{1,4},{2,1}, 
                        {6,4},{4,3},{3,7}});
    /*
    BFS on Node 1 should go:
    (neighbours sorted in increasing order)
    1 -> 0 -> 2 -> 4 -> 5 -> 6 -> 3 -> 7

    BFS on Node 3 should go:
    4 -> 1 -> 3 -> 6 -> 0 -> 2 -> 7 -> 5
    */
    assert(g.bfs(1) == std::vector<int>({1,0,2,4,5,6,3,7}));
    assert(g.bfs(4) == std::vector<int>({4,1,3,6,0,2,7,5}));
    assert(g.bfs(8) == std::vector<int>({8}));
    std::cout << "BFS passed\n";
    /*
    DFS on Node 1 should go:
    1 -> 0 (backtrack) -> 2 -> 5 (backtrack) -> 6 -> 4 -> 3 -> 7
    On node 2:
    2 -> 1 -> 0 (bt) -> 4 -> 3 -> 7 (bt) -> 6 (bt) -> 5

    */
    assert(g.dfs(1) == std::vector<int>({1,0,2,5,6,4,3,7}));
    assert(g.dfs(2) == std::vector<int>({2,1,0,4,3,7,6,5}));
    assert(g.dfs(8) == std::vector<int>({8}));
    std::cout << "DFS passed\n";
}

void test_weighted_graph() {
    /* Graph looks like:
      9    8->    7
    0 -> 2 <-> 3 -> 4
           <-2
    1 (alone)   
    */
    WeightedGraph g = WeightedGraph(5);
    assert(g.addEdge({0, 2, 9}));
    assert(g.addEdge({2, 3, 8}));
    assert(g.addEdge({3, 2, 2}));
    assert(g.addEdge({3, 4, 7}));
    assert(g.neighbours(0) == std::vector<int>({2}));
    assert(g.neighbours(1).empty());
    assert(g.neighbours(2) == std::vector<int>({3}));
    assert(g.neighbours(3) == std::vector<int>({2,4}));
    assert(g.neighbours(4).empty());
    // Test Deleting Edge
    assert(!g.deleteEdge({0, 1, 1}));
    assert(g.deleteEdge({2, 3, 8}));
    assert(g.neighbours(2).empty());
    std::cout << "Weighted Graph Operations Passed\n";
}

int main() {
    test_undirected_graph();
    test_search();
    test_weighted_graph();
    test_max_flow();
}

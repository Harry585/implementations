/*
Implement a graph data structure and include 
implementations for BFS (iterative) and DFS (recursive)
*/

/*
Features:
- Vectors and List usage
- Initializer list constructor
*/
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <cassert>
#include <climits>
#include <tuple>
#include <algorithm>

/* Generalized Graph Interface. */
template <typename Edge>
class Graph {  
public:
    virtual bool addEdge(const Edge& edge) = 0;
    virtual bool deleteEdge(const Edge& edge) = 0;
    virtual std::vector<int> neighbours(int node) const = 0;
};

class UndirectedGraph : Graph<std::pair<int, int>> {
protected:
    using Edge = std::pair<int, int>;
    int n;
    std::vector<std::set<int>> edges;
    void dfs_recursive(int node, std::vector<int>& result) const {
        if (std::find(result.begin(), result.end(), node) != result.end()) {
            return;
        }
        result.push_back(node);
        for (int n : neighbours(node)) {
            dfs_recursive(n, result);
        }
    }
public:
    // edges(n) calls the vector constructor with n n
    // This is idiomatic, and the list<int> elements are default-initialized.
    UndirectedGraph(int n) : n(n), edges(n) {}
    // Initializer list constructor, uses constructor delegation
    // use initEdges, because parameter edges would shadow member variable edges
    UndirectedGraph(int n, std::initializer_list<Edge> initEdges) : UndirectedGraph(n) {
        for (const Edge& edge : initEdges) {
            addEdge(edge);
        }
    }
    // Inserts an edge into the graph if exists.
    // Return whether adding edge was successful
    bool addEdge(const Edge& edge) override {
        auto [x, y] = edge;
        if (x < 0 || y < 0 || x >= n || y >= n || x == y) {
            // Either std::domain_error or std::invalid_argument
            // Are acceptable
            throw std::domain_error("Invalid edge");
        }
        // Insert returns iterator position and whether insert was successful
        std::pair<std::set<int>::iterator, bool> success1 = edges[x].insert(y);
        std::pair<std::set<int>::iterator, bool> success2 = edges[y].insert(x);
        // new edge inserted if either success or success2
        bool inserted_new = success1.second | success2.second;
        return inserted_new;
    }
    // deletes an edge from the graph if it exists.
    // Return true if deletion was successful
    bool deleteEdge(const Edge& edge) override {
        auto [x, y] = edge;
        if (x < 0 || y < 0 || x >= n || y >= n) {
            throw std::domain_error("Invalid edge");
        }
        int erased = edges[y].erase(x);
        erased += edges[x].erase(y);
        return (erased) ? true : false;
    }
    // Public API to return all neighbours of a node
    // Sorted for deterministic behaviour for testing and BFS/DFS
    std::vector<int> neighbours(int node) const override {
        // Use a range constructor
        auto result = std::vector<int>(edges[node].begin(), edges[node].end());
        return result;
    }
    std::vector<int> bfs(int start) const {
        if (start < 0 || start >= n) {
            throw std::invalid_argument("Invalid start node");
        }
        std::vector<int> visited;
        std::queue<int> q;
        q.push(start);
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            // If not visited, add to visited
            if (std::find(visited.begin(), visited.end(), node) == visited.end()) {
                visited.push_back(node);
            }
            for (int n : neighbours(node)) {
                if (std::find(visited.begin(), visited.end(), n) == visited.end()) {
                    q.push(n);
                }
            }
        }
        return visited;
    }
    std::vector<int> dfs(int start) const {
        if (start < 0 || start > n) {
            throw std::invalid_argument("Invalid start node");
        }
        std::vector<int> result;
        dfs_recursive(start, result);
        return result;
    }
};

class WeightedGraph : Graph<std::tuple<int, int, int>> {
protected:
    int n;
    std::vector<std::vector<int>> matrix;
    int augmentingFlow(int source, int sink);
public:
    using Edge = std::tuple<int, int, int>;
    WeightedGraph(int size) : n(size), matrix(size, std::vector<int>(size)) {};
    // Initializer list
    WeightedGraph(int size, std::initializer_list<Edge> edges) : n(size), matrix(size, std::vector<int>(size)) {
        for (Edge e : edges) {
            addEdge(e);
        }
    }
    bool addEdge(const Edge& edge) override {
        // Each edge is <source, sink, weight>
        auto [source, dest, weight] = edge;
        if (source < 0 || source >= n || dest < 0 || dest >= n || source == dest) {
            throw std::invalid_argument("Invalid edge");
        }
        if (weight == 0) {
            throw std::invalid_argument("Weight is 0");
        }
        if (matrix[source][dest]) {
            return false;
        }
        matrix[source][dest] = weight;
        return true;
    }
    bool deleteEdge(const Edge& edge) override {
        auto [source, dest, weight] = edge;
        if (source < 0 || source >= n || dest < 0 || dest >= n) {
            throw std::invalid_argument("Invalid edge");
        }
        if (weight == 0) {
            throw std::invalid_argument("Weight is 0");
        }
        if (matrix[source][dest] == 0) {
            return false;
        }
        matrix[source][dest] = 0;
        return true;
    }
    std::vector<int> neighbours(int node) const override {
        std::vector<int> res;
        for (int i = 0; i < n; i++) {
            if (matrix[node][i] > 0) {
                res.push_back(i);
            }
        }
        return res;
    }
    int edmondsKarp(int source, int sink);
};

// Searches for an augmenting flow from source to sink
// Returns: n of augmenting flow, -1 on failure
int WeightedGraph::augmentingFlow(int source, int sink) {
    std::queue<int> queue;
    std::vector<bool> visited(n);
    std::vector<int> pred(n, -1);

    queue.push(source);
    visited[source] = true;
    int curr;
    while (!queue.empty()) {
        curr = queue.front();
        std::cout << std::format("Removing curr={}\n", curr);
        queue.pop();

        if (curr == sink) {
            break;
        }
        for (int node : neighbours(curr)) {
            if (!visited[node]) {
                queue.push(node);
                // Update "visited" at the point of pushing,
                // Even though you haven't visited, so it's only discovered
                // Avoids pushing same node more than once.
                pred[node] = curr;
                visited[node] = true;
            }
        }
    }
    if (curr != sink) {
        return 0;
    }
    // Path found
    // Find augmenting path weight
    int augmenting_flow = INT_MAX;
    int prev = pred[curr];
    while (prev != -1) {
        std::cout << std::format("Tracing back to prev={}\n", prev);
        int weight = matrix[prev][curr];
        augmenting_flow = std::min(augmenting_flow, weight);
        curr = prev;
        prev = pred[prev];
    }
    std::cout << std::format("augmenting_flow={}\n", augmenting_flow) << std::endl;

    // Update values in the residual graph
    curr = sink;
    prev = pred[curr];
    while (prev != -1) {
        matrix[prev][curr] -= augmenting_flow;
        matrix[curr][prev] += augmenting_flow;
        curr = prev;
        prev = pred[prev];
    }
    return augmenting_flow;
}

// Calculates max network flow using Edmonds-Karp algorithm
int WeightedGraph::edmondsKarp(int source, int sink) {
    if (source < 0 || source >= n || sink < 0 || sink >= n) {
        throw std::domain_error("Invalid source or sink nodes");
    } else if (source == sink) {
        throw std::invalid_argument("Source and sink nodes cannot be the same");
    }
    int maxFlow = 0;
    int flow;
    while ((flow = augmentingFlow(source, sink)) > 0) {
        maxFlow += flow;
    }
    return maxFlow;
}

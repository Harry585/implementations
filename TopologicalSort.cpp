#include <vector>
#include <queue>
#include <algorithm>
// Given an edge list representing a directed graph, 
// Return a topological sort or an empty array if no such sort exists.
class TopSort {
public:
    std::vector<int> topsort(int n, std::vector<std::vector<int>>& edges) {
        // Build adjacency list
        std::vector<std::vector<int>> adjList = std::vector<std::vector<int>>(n);
        std::vector<int> indegree(n, 0);
        for (std::vector<int>& edge : edges) {
            int to = edge[1];
            int from = edge[0];
            adjList[from].push_back(to);
            indegree[to]++;
        }
        // Conduct Kahn's algorithm
        std::vector<int> topsort;
        std::queue<int> q;
        for (int i = 0; i < n; i++) {
            if (indegree[i] == 0) {
                q.push(i);
            }
        }
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            topsort.push_back(node);
            for (int out : adjList[node]) {
                indegree[out]--;
                if (indegree[out] == 0) {
                    q.push(out);
                }
            }
        }
        return (topsort.size() == n) ? topsort : std::vector<int>();
    }
};
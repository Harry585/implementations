// Implements the union-find data structure
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>

class UnionFind {
private:
    // Stores a forest of pointers
    std::vector<int> pointers;
    // Stores corresponding heights of each tree
    std::vector<int> heights;

    // Recursive implementation
    int find(int elem) {
        if (elem == pointers[elem]) {
            return elem;
        }
        // Sets the pointers
        pointers[elem] = find(pointers[elem]);
        return pointers[elem];
    }

    void merge_roots(int r1, int r2) {
        int h1 = heights[r1];
        int h2 = heights[r2];
        if (h1 < h2) {
            // merge r1 into r2
            pointers[r1] = r2;
        } else if (h1 > h2) {
            pointers[r2] = r1;
        } else {
            // Arbitrary choice to merge r1 into r2
            pointers[r1] = r2;
            heights[r2]++;
        }
    }

public:
    UnionFind(int size) : pointers(size), heights(size, 1) {
        for (int i = 0; i < size; i++) {
            pointers[i] = i;
        }
    }
    //   returns whether these two components are in the same partition
    bool same_component(const int a, const int b) {
        return (find(a) == find(b));
    }
    // Merges two components
    // Returns true if successful, false if a,b were in the same partition
    bool merge(const int a, const int b) {
        int r1 = find(a);
        int r2 = find(b);
        if (r1 == r2) {
            return false;
        }
        merge_roots(r1, r2);
        return true;
    }

    // function for debugging
    void printState() {
        std::cout << "Tree states: ";
        for (int ptr : pointers) {
            std::cout << ptr << " ";
        }
        std::cout << std::endl;
        std::cout << "Tree heights: ";
        for (int height : heights) {
            std::cout << height << " ";
        }
        std::cout << std::endl;
    }
};

void runtests() {
    UnionFind uf = UnionFind(7);
    uf.printState();
    assert(!uf.merge(1,1));
    assert(uf.merge(1,2));
    assert(uf.same_component(1,2));
    assert(uf.merge(2,3));
    assert(uf.same_component(1,2));
    assert(uf.same_component(3,2));
    assert(uf.same_component(1,3));
    assert(uf.merge(4,5));
    assert(uf.merge(3,5));
    uf.printState();
    assert(uf.same_component(1,4));
    assert(uf.same_component(1,5));
    assert(!uf.same_component(1,6));
    uf.printState();
    UnionFind invalid = UnionFind(-1);
}

int main() {
    runtests();
}
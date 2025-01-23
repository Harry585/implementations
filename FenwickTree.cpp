#include <vector>
#include <iostream>
#include <cassert>

class FenwickTree {
private:
std::vector<int> tree;
int n;
public:
    FenwickTree(const std::vector<int>& nums) : tree(nums.size()+1), n(nums.size()) {
        for (int i = 0; i < n; i++) {
            update(i, nums[i]);
        }
    }
    // Updates the Fenwick tree to reflect a change in the original array
    void update(int i, int delta) {
        // Tree is 1-ied array
        i++;
        while (i <= n) {
            tree[i] += delta;
            i += i & -i;
        }
    }
    // Computes the prefix sum up to element i
    long query(int i) {
        i++;
        long sum = 0;
        while (i > 0) {
            sum += tree[i];
            i -= i & -i;
        }
        return sum;
    }
};


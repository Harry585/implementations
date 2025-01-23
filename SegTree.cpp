#include <vector>
#include <algorithm>
#include <iostream>

class SegTree {
private:
    std::vector<int> tree;
    std::vector<int> nums;
public:
    SegTree(std::vector<int>& nums) : nums(nums), tree(4 * nums.size(), 0) {
        // Build a segment tree
        build(0, 0, nums.size()-1);
    }
    void build(int node, int start, int end) {
        if (start == end) {
            tree[node] = nums[start];
            return;
        }
        // Recursive case:  build both children
        int mid = (start + end) / 2;
        build(node*2 + 1, start, mid);
        build(node*2 + 2, mid+1, end);
        tree[node] = tree[node*2 + 1 ] + tree[node*2 + 2];
    }
    
    void update(int index, int val) {
        // Try iterative solution
        if (index < 0 || index > nums.size()) {
            return;
        }
        int difference = val - nums[index];
        nums[index] += difference;
        // Traverse through implicit tree to binary search
        int lo = 0;
        int hi = nums.size() - 1;
        int node = 0;
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            tree[node] += difference;
            if (lo <= index && index <= mid) {
                // recurse on the left
                node = node * 2 + 1;
                hi = mid;
            } else if (mid < index && index <= hi) {
                node = node * 2 + 2;
                lo = mid + 1;
            } else {
                throw std::runtime_error("Binary search wrong");
            }
        }
        // lo == hi, update final value
        tree[node] += difference;
    }
    // Returns sum(nums[left:right+1]) in O(lgn)
    int sumRange(int left, int right) {
        return query(0, left, right, 0, nums.size() - 1);
    }
    int query(int node, int left, int right, int lo, int hi) {
        // Recursively query, split into three cases:
        // If query is completely outside node range
        if (right < lo || left > hi) {
            return 0;
        }
        // If query is completely inside node range
        if (left <= lo && hi <= right) {
            return tree[node];
        }
        // Otherwise, examine both children.
        int mid = (lo + hi) / 2;
        int result = query(2 * node + 1, left, right, lo, mid);
        result += query(2 * node + 2, left, right, mid + 1, hi);
        return result;
    }

};
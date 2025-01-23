#include <vector>
#include <iostream>
#include <cassert>

#include "FenwickTree.cpp"
// Assuming FenwickTree class is defined as above

void test_initialization_and_basic_queries() {
    std::vector<int> nums = {1, 2, 3, 4, 5};
    FenwickTree ft(nums);
    
    // Test prefix sums
    assert(ft.query(0) == 1);            // Sum up to index 0
    assert(ft.query(1) == 3);            // Sum up to index 1
    assert(ft.query(2) == 6);            // Sum up to index 2
    assert(ft.query(3) == 10);           // Sum up to index 3
    assert(ft.query(4) == 15);           // Sum up to index 4
    std::cout << "test_initialization_and_basic_queries passed.\n";
}

void test_single_element_updates() {
    std::vector<int> nums = {5, 5, 5, 5, 5};
    FenwickTree ft(nums);
    
    // Update index 2 by +3 (original value 5 -> 8)
    ft.update(2, 3);
    assert(ft.query(2) == 18); // 5 + 5 + 8 = 18 up to index 2
    assert(ft.query(4) == 28); // 5 + 5 + 8 + 5 + 5 = 28
    
    // Update index 0 by -2 (original value 5 -> 3)
    ft.update(0, -2);
    assert(ft.query(0) == 3);
    assert(ft.query(4) == 26); // 3 + 5 + 8 + 5 + 5 = 26
    std::cout << "test_single_element_updates passed.\n";
}

void test_multiple_updates() {
    std::vector<int> nums = {0, 0, 0, 0, 0, 0};
    FenwickTree ft(nums);
    
    // Perform multiple updates
    ft.update(0, 1); // [1,0,0,0,0,0]
    ft.update(1, 2); // [1,2,0,0,0,0]
    ft.update(2, 3); // [1,2,3,0,0,0]
    ft.update(3, 4); // [1,2,3,4,0,0]
    ft.update(4, 5); // [1,2,3,4,5,0]
    ft.update(5, 6); // [1,2,3,4,5,6]
    
    // Test prefix sums
    assert(ft.query(0) == 1);
    assert(ft.query(1) == 3);
    assert(ft.query(2) == 6);
    assert(ft.query(3) == 10);
    assert(ft.query(4) == 15);
    assert(ft.query(5) == 21);
    
    // Further updates
    ft.update(3, -4); // [1,2,3,0,5,6]
    assert(ft.query(3) == 6);
    assert(ft.query(5) == 17);
    
    ft.update(5, 4); // [1,2,3,0,5,10]
    assert(ft.query(5) == 21);
    
    std::cout << "test_multiple_updates passed.\n";
}

void test_empty_array() {
    std::vector<int> nums = {};
    FenwickTree ft(nums);
    assert(ft.query(0) == 0);
    std::cout << "test_empty_array passed.\n";
}

void test_single_element_array() {
    std::vector<int> nums = {42};
    FenwickTree ft(nums);
    
    assert(ft.query(0) == 42);
    
    ft.update(0, 8); // 42 +8 =50
    assert(ft.query(0) == 50);
    
    ft.update(0, -50); // 50 -50 =0
    assert(ft.query(0) == 0);
    
    std::cout << "test_single_element_array passed.\n";
}

void test_all_zeroes() {
    std::vector<int> nums(10, 0);
    FenwickTree ft(nums);
    
    // All prefix sums should be 0 initially
    for(int i=0; i<10; i++) {
        assert(ft.query(i) == 0);
    }
    
    // Perform some updates
    ft.update(3, 5);
    ft.update(7, 10);
    
    // Check prefix sums
    assert(ft.query(3) == 5);
    assert(ft.query(4) == 5);
    assert(ft.query(7) == 15);
    assert(ft.query(9) == 15);
    
    std::cout << "test_all_zeroes passed.\n";
}

void test_negative_numbers() {
    std::vector<int> nums = {5, -3, 7, -2, 4};
    FenwickTree ft(nums);
    
    // Test initial prefix sums
    assert(ft.query(0) == 5);
    assert(ft.query(1) == 2);  // 5 + (-3)
    assert(ft.query(2) == 9);  // 5 + (-3) +7
    assert(ft.query(3) == 7);  // 5 + (-3) +7 + (-2)
    assert(ft.query(4) == 11); // 5 + (-3) +7 + (-2) +4
    
    // Perform updates
    ft.update(1, 3); // nums[1] = -3 +3 =0
    assert(ft.query(1) == 5);  // 5 +0
    assert(ft.query(4) == 14); // 5 +0 +7 + (-2) +4
    
    ft.update(3, 2); // nums[3] = -2 +2 =0
    assert(ft.query(4) == 16); // 5 +0 +7 +0 +4
    
    std::cout << "test_negative_numbers passed.\n";
}

int main() {
    test_initialization_and_basic_queries();
    test_single_element_updates();
    test_multiple_updates();
    test_empty_array();
    test_single_element_array();
    test_all_zeroes();
    test_negative_numbers();
    
    std::cout << "All tests passed successfully.\n";
    return 0;
}
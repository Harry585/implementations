#include <iostream>
#include <cassert>
#include <sstream>
#include "Vector.hpp" // Ensure that Vector class is declared in Vector.hpp

// Test Utilities
void print_test_result(const std::string& test_name, bool passed) {
    if (passed) {
        std::cout << "[PASS] " << test_name << std::endl;
    } else {
        std::cout << "[FAIL] " << test_name << std::endl;
    }
}

// Test 1: Default Constructor
void test_default_constructor() {
    std::string test_name = "Default Constructor";
    Vector<int> v;
    bool passed = (v.size() == 0) && (v.capacity() >= 16) && v.empty();
    print_test_result(test_name, passed);
    assert(passed);
}

// Test 2: Size Constructor
void test_size_constructor() {
    std::string test_name = "Size Constructor";
    Vector<int> v(10, 5);
    bool passed = (v.size() == 10) && (v.capacity() >= 16);
    for(int i = 0; i < 10; ++i) {
        if(v[i] != 5) {
            passed = false;
            break;
        }
    }
    print_test_result(test_name, passed);
    assert(passed);
}

// Test 3: Negative Size Constructor (Should throw exception)
void test_negative_size_constructor() {
    std::string test_name = "Negative Size Constructor";
    bool passed = false;
    try {
        Vector<int> v(-5);
    } catch(const std::domain_error&) {
        passed = true;
    } catch(...) {
        passed = false;
    }
    print_test_result(test_name, passed);
    assert(passed);
}

// Test 4: Push Back
void test_push_back() {
    std::string test_name = "Push Back";
    Vector<int> v;
    for(int i = 0; i < 20; ++i) {
        v.push_back(i);
    }
    bool passed = (v.size() == 20);
    for(int i = 0; i < 20; ++i) {
        if(v[i] != i) {
            passed = false;
            break;
        }
    }
    print_test_result(test_name, passed);
    assert(passed);
}

// Test 5: Pop Back
void test_pop_back() {
    std::string test_name = "Pop Back";
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    int popped = v.pop_back();
    bool passed = (popped == 3) && (v.size() == 2) && (v.back() == 2);
    print_test_result(test_name, passed);
    assert(passed);
}

// Test 6: Pop Back from Empty Vector (Should throw exception)
void test_pop_back_empty() {
    std::string test_name = "Pop Back from Empty Vector";
    Vector<int> v;
    bool passed = false;
    try {
        v.pop_back();
    } catch(const std::invalid_argument&) {
        passed = true;
    } catch(...) {
        passed = false;
    }
    print_test_result(test_name, passed);
    assert(passed);
}

// Test 7: Front and Back
void test_front_back() {
    std::string test_name = "Front and Back";
    Vector<std::string> v;
    v.push_back("first");
    v.push_back("middle");
    v.push_back("last");
    bool passed = (v.front() == "first") && (v.back() == "last");
    print_test_result(test_name, passed);
    assert(passed);
}

// Test 8: Accessing Out of Range (Should throw exception)
void test_out_of_range_access() {
    std::string test_name = "Out of Range Access";
    Vector<int> v(5, 10);
    bool passed = false;
    try {
        [[maybe_unused]] int val = v[5];
    } catch(const std::invalid_argument&) {
        passed = true;
    } catch(...) {
        passed = false;
    }
    print_test_result(test_name, passed);
    assert(passed);
}

// Test 9: Resize Smaller
void test_resize_smaller() {
    std::string test_name = "Resize Smaller";
    Vector<int> v;
    for(int i = 0; i < 20; ++i) {
        v.push_back(i);
    }
    v.resize(10);
    bool passed = (v.size() == 10);
    for(int i = 0; i < 10; ++i) {
        if(v[i] != i) {
            passed = false;
            break;
        }
    }
    print_test_result(test_name, passed);
    assert(passed);
}

// Test 10: Resize Larger
void test_resize_larger() {
    std::string test_name = "Resize Larger";
    Vector<int> v(5, 1);
    v.resize(10);
    bool passed = (v.size() == 5) && (v.capacity() >= 10);
    for(int i = 0; i < 5; ++i) {
        if(v[i] != 1) {
            passed = false;
            break;
        }
    }
    print_test_result(test_name, passed);
    assert(passed);
}

// Test 11: Equality Operator
void test_equality_operator() {
    std::string test_name = "Equality Operator";
    Vector<int> v1 = Vector<int>(5, 2);
    Vector<int> v2 = Vector<int>(5, 2);
    Vector<int> v3 = Vector<int>(5, 3);
    bool passed = (v1 == v2) && !(v1 == v3);
    print_test_result(test_name, passed);
    assert(passed);
}

// Test 12: Copy Constructor
void test_copy_constructor() {
    std::string test_name = "Copy Constructor";
    Vector<int> v1;
    for(int i = 0; i < 10; ++i) v1.push_back(i);
    Vector<int> v2 = v1;
    bool passed = (v1 == v2);
    // Modify v2 and check v1 remains unchanged
    v2.push_back(10);
    passed = passed && (v1.size() == 10) && (v2.size() == 11);
    print_test_result(test_name, passed);
    assert(passed);
}

// Test 13: Copy Assignment
void test_copy_assignment() {
    std::string test_name = "Copy Assignment";
    Vector<int> v1;
    for(int i = 0; i < 10; ++i) v1.push_back(i);
    Vector<int> v2;
    v2 = v1;
    bool passed = (v1 == v2);
    // Modify v2 and check v1 remains unchanged
    v2.pop_back();
    passed = passed && (v1.size() == 10) && (v2.size() == 9);
    print_test_result(test_name, passed);
    assert(passed);
}

// Test 14: Move Constructor
void test_move_constructor() {
    std::string test_name = "Move Constructor";
    Vector<int> v1;
    for(int i = 0; i < 10; ++i) v1.push_back(i);
    Vector<int> v2 = std::move(v1);
    bool passed = (v2.size() == 10) && (v1.size() == 0);
    print_test_result(test_name, passed);
    assert(passed);
}

// Test 15: Move Assignment
void test_move_assignment() {
    std::string test_name = "Move Assignment";
    Vector<int> v1;
    for(int i = 0; i < 10; ++i) v1.push_back(i);
    Vector<int> v2;
    v2 = std::move(v1);
    bool passed = (v2.size() == 10) && (v1.size() == 0);
    print_test_result(test_name, passed);
    assert(passed);
}

// Test 16: contains Method
void test_contains() {
    std::string test_name = "contains Method";
    Vector<std::string> v;
    v.push_back("apple");
    v.push_back("banana");
    v.push_back("cherry");
    bool passed = v.contains("banana") && !v.contains("date");
    print_test_result(test_name, passed);
    assert(passed);
}

// Test 17: Empty Method
void test_empty_method() {
    std::string test_name = "Empty Method";
    Vector<int> v;
    bool passed = v.empty();
    v.push_back(1);
    passed = passed && !v.empty();
    v.pop_back();
    passed = passed && v.empty();
    print_test_result(test_name, passed);
    assert(passed);
}

// Test 18: Operator[]
void test_operator_brackets() {
    std::string test_name = "Operator[]";
    Vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    bool passed = (v[0] == 10) && (v[1] == 20) && (v[2] == 30);
    // Modify an element
    v[1] = 25;
    passed = passed && (v[1] == 25);
    print_test_result(test_name, passed);
    assert(passed);
}

// Test 19: Operator<<
void test_operator_output() {
    std::string test_name = "Operator<<";
    Vector<int> v;
    std::ostringstream oss;
    oss << v;
    bool passed = (oss.str() == "[]");
    
    v.push_back(1);
    oss.str(""); oss.clear();
    oss << v;
    passed = passed && (oss.str() == "[1]");
    
    v.push_back(2);
    v.push_back(3);
    oss.str(""); oss.clear();
    oss << v;
    passed = passed && (oss.str() == "[1, 2, 3]");
    
    print_test_result(test_name, passed);
    assert(passed);
}

// Test 20: Clear
void test_clear() {
    std::string test_name = "Clear";
    Vector<int> v;
    v.push_back(1);
    bool passed = (v.size() == 1) && (v[0] == 1);
    v.clear();
    v.push_back(2);
    passed = passed && (v.size() == 1) && (v[0] == 2);
    print_test_result(test_name, passed);
    assert(passed);
}

// Main Function to Run All Tests
int main() {
    std::cout << "Running Vector Tests..." << std::endl;
    
    test_default_constructor();
    test_size_constructor();
    test_negative_size_constructor();
    test_push_back();
    test_pop_back();
    test_pop_back_empty();
    test_front_back();
    test_out_of_range_access();
    test_resize_smaller();
    test_resize_larger();
    test_equality_operator();
    test_copy_constructor();
    test_copy_assignment();
    test_move_constructor();
    test_move_assignment();
    test_contains();
    test_empty_method();
    test_operator_brackets();
    test_operator_output();
    test_clear();
    
    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}
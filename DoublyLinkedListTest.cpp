#include <iostream>
#include <cassert>
#include "DoublyLinkedList.hpp"

int main() {
    Stack<int> myStack;
    
    // Test empty Stack
    std::cout << "Initial Stack: " << myStack << std::endl;
    std::cout << "Is empty? " << std::boolalpha << myStack.empty() << std::endl;
    
    // Push elements
    myStack.push(10);
    myStack.push(20);
    myStack.push(30);
    
    std::cout << "After pushing 10, 20, 30: " << myStack << std::endl;
    std::cout << "Size: " << myStack.size() << std::endl;
    std::cout << "Top element: " << myStack.top() << std::endl;
    
    // Pop elements
    myStack.pop();
    std::cout << "After popping: " << myStack << std::endl;
    std::cout << "Size: " << myStack.size() << std::endl;
    std::cout << "Top element: " << myStack.top() << std::endl;
    
    myStack.pop();
    myStack.pop();
    std::cout << "After popping all elements: " << myStack << std::endl;
    std::cout << "Is empty? " << std::boolalpha << myStack.empty() << std::endl;
    
    // Attempt to pop from empty Stack (should throw exception)
    try {
        myStack.pop();
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
    
    return 0;
}
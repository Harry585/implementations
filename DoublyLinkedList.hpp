/*
Doubly Linked Stack implementation
Features smart pointers and templates
Thoughts: using weak_ptr is unnecessary and introduces
additional complexities into the code
It is enough to just use either:
- unique_ptr for forward reference and dumb ptrs for backrefs, or:
- use dumb ptrs and destructors
*/

#include <iostream>
#include <algorithm>
#include <cassert>
#include <memory>

// Forward declaration of Stack class for Node
template <typename T>
class Stack;

template <typename T>
class Node {
    friend class Stack<T>;
private:
    T val;
    std::shared_ptr<Node<T>> next;
    std::weak_ptr<Node<T>> prev;
public:
    Node() = default;
    Node(T value) : val(value) {};
};

template <typename T>
class Stack {
private:
    std::shared_ptr<Node<T>> head;
    std::shared_ptr<Node<T>> tail;
    int _size;
public:
    // Idea: have sentinel values to make appends and deletes
    // Easier to implement
    Stack(): head(std::make_shared<Node<T>>()), 
            tail(std::make_shared<Node<T>>()), 
            _size(0) {
        head->next = tail;
        tail->prev = head;
    }
    void push(T val) {
        auto prevTail = tail->prev.lock();
        if (prevTail) {
            auto newNode = std::make_shared<Node<T>>(val);
            prevTail->next = newNode;
            tail->prev = newNode;

            newNode->next = tail;
            newNode->prev = prevTail;
            _size++;
        } else {
            throw std::runtime_error("Append failed from invalid weak_ptr lock");
        }
    }
    void pop() {
        // Remove last element from the Stack
        if (_size == 0) {
            throw std::runtime_error("Cannot pop back from empty Stack");
        }
        // Must I use .lock()?
        std::shared_ptr<Node<T>> newLast = tail->prev.lock();
        if (newLast) {
            newLast = newLast->prev.lock();
        }
        if (newLast) {
            newLast->next = tail;
            tail->prev = newLast;
            _size--;
            return;
        }
        throw std::runtime_error("Remove failed from invalid weak_ptr lock");
        // RAII: prev tail element automatically left out of scope.
    }
    T top() {
        auto last = tail->prev.lock();
        if (last) {
            return last->val;
        }
        throw std::runtime_error("Weak ptr failed to lock");
    }
    bool empty() {
        return (this->_size == 0);
    }
    int size() {
        return this->_size;
    }
    friend std::ostream& operator<<(std::ostream& os, const Stack<T>& stk) {
        if (stk._size == 0) {
            os << "Empty Stack";
            return os;
        }
        os << "[";
        std::shared_ptr<Node<T>> curr = stk.head->next;
        auto last = stk.tail->prev.lock();
        if (!last) {
            return os;
        }
        while (curr != last) {
            os << curr->val << ", ";
            curr = curr->next;
        }
        os << curr->val << "]";
        return os;
    }
};

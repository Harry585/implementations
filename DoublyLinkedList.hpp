/*
Doubly Linked List implementation
Features smart pointers and templates
*/

#include <iostream>
#include <algorithm>
#include <cassert>
#include <memory>

// Forward declaration of List class for Node
template <typename T>
class List;

template <typename T>
class Node {
    friend class List<T>;
private:
    T val;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;
public:
    Node() = default;
    Node(T value) : val(value), next(nullptr), prev(nullptr) {};
};

template <typename T>
class List {
private:
    std::shared_ptr<Node<T>> head;
    std::shared_ptr<Node<T>> tail;
    int size;
public:
    // Idea: have sentinel values to make appends and deletes
    // Easier to implement
    List(): head(Node<T>()), tail(Node<T>()), size(0) {
        head->next = tail;
        tail->prev = head;
    }
    void append(T val) {
        Node<T> *prevTail = tail->prev;
        prevTail->next = std::make_shared<Node<T>>(val);
        Node<T> *newNode = prevTail->next;
        newNode->next = tail;
        newNode->prev = prevTail;
        size++;
    }
    void pop() {
        // Remove last element from the list
        if (size == 0) {
            throw std::runtime_error("Cannot pop back from empty list\n");
        }
        // Must I use .lock()?
        Node<T> *newLast = tail->prev->prev;
        newLast->next = tail->prev;
        tail->prev = newLast;
        // RAII: prev tail element automatically left out of scope.
    }
    friend std::ostream& operator<<(std::ostream& os, List<T> list) {
        os << "[";
        Node<T> *curr = list.head->next;
        while (curr) {
            os << curr->val << ", ";
            curr = curr->next;
        }
        os << "]";
        return os;
    }
};

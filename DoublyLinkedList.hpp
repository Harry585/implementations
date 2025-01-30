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
    std::shared_ptr<Node<T>> next;
    std::weak_ptr<Node<T>> prev;
public:
    Node() = default;
    Node(T value) : val(value) {};
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
    List(): head(std::make_shared<Node<T>>()), 
            tail(std::make_shared<Node<T>>()), 
            size(0) {
        head->next = tail;
        tail->prev = head;
    }
    void append(T val) {
        auto prevTail = tail->prev.lock();
        if (prevTail) {
            auto newNode = std::make_shared<Node<T>>(val);
            prevTail->next = newNode;
            tail->prev = newNode;

            newNode->next = tail;
            newNode->prev = prevTail;
            size++;
        } else {
            throw std::runtime_error("Append failed from invalid weak_ptr lock");
        }
    }
    void pop() {
        // Remove last element from the list
        if (size == 0) {
            throw std::runtime_error("Cannot pop back from empty list\n");
        }
        // Must I use .lock()?
        std::shared_ptr<Node<T>> newLast = tail->prev.lock();
        if (newLast) {
            newLast = newLast->prev.lock();
        }
        if (newLast) {
            auto x = tail->prev.lock();
            if (x) {
                newLast->next = x;
                tail->prev = newLast;
                size--;
                return;
            }
        }
        throw std::runtime_error("Remove failed from invalid weak_ptr lock");
        // RAII: prev tail element automatically left out of scope.
    }
    friend std::ostream& operator<<(std::ostream& os, List<T> list) {
        if (list.size == 0) {
            os << "Empty List";
            return os;
        }
        os << "[";
        std::shared_ptr<Node<T>> curr = list.head->next;
        auto last = list.tail->prev.lock();
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

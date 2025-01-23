#include <iostream>
#include <memory>
#include <cassert>
// This linked list uses raw pointers, and includes:
// Constructor, copy constructor, copy assignment operator,
// Move constructor, move assignment operator.
// Also includes basic tests, which are not comprehensive.

bool runtests();
bool testcopy();
bool testmove();

class Node {
private:    // Default access modifier is private
    int value;
    Node *next;
public:
    Node(int v) : value(v), next(nullptr) {}
    friend class LinkedList;
};

class LinkedList {
private:
    Node *head;
    Node * _reverse(Node *node) {
        if (node->next == nullptr) {
            return node;
        }
        Node *new_head = _reverse(node->next);
        node->next->next = node;
        return new_head;
    }
    void clear() {
        Node *curr = head;
        while (curr != nullptr) {
            Node *temp = curr->next;
            delete curr;
            curr = temp;
        }
        head = nullptr;
    }
public:
    // This is the default constructor, writing for clarity
    LinkedList() : head(nullptr) {
        std::cout << "Constructor\n";
    }
    ~LinkedList() {
        Node *curr = head;
        while (curr != nullptr) {
            Node *temp = curr->next;
            delete curr;
            curr = temp;
        }
    }
    // Copy constructor, performs deep copy
    LinkedList(const LinkedList& other) {
        std::cout << "Copy constructor\n";
        if (other.head == nullptr) {
            this->head = nullptr;
            return;
        }
        this->head = new Node(other.head->value);
        Node *thisCurr = this->head;
        Node *otherCurr = other.head->next;
        while (otherCurr != nullptr) {
            std::cout << "Copying " << otherCurr->value << std::endl;
            thisCurr->next = new Node(otherCurr->value);
            thisCurr = thisCurr->next;
            otherCurr = otherCurr->next;
        }
    }
    // Copy assignment operator
    LinkedList& operator=(const LinkedList& other) {
        if (this == &other) {
            return *this;
        }
        std::cout << "Copy assignment operator\n";
        this->clear();
        head = nullptr;
        for (Node *curr = other.head; curr != nullptr; curr = curr->next) {
            this->insert(curr->value);
        }
        return *this;
    }
    // Move constructor
    LinkedList(LinkedList&& other) noexcept : head(other.head) {
        std::cout << "Move constructor\n";
        other.head = nullptr;
    }
    // Move assignment operator
    LinkedList& operator=(LinkedList&& other) noexcept {
        std::cout << "Move assignment operator\n";
        if (this == &other) {
            return *this;
        }
        if (this->head == nullptr) {
            this->head = other.head;
        } else {
            Node *tail;
            for (tail = this->head; tail->next != nullptr; tail = tail->next) {}
            tail->next = other.head;
        }
        other.head = nullptr;
        return *this;
    }
    void insert(int value) {
        if (head == nullptr) {
            head = new Node(value);
            return;
        }
        Node *curr = head;
        while (curr->next != nullptr) {
            curr = curr->next;
        }
        curr -> next = new Node(value);
    }
    void traverse() {
        if (head == nullptr) {
            std::cout << "List is empty" << std::endl;
            return;
        }
        Node *curr = head;
        while (curr->next != nullptr) {
            std::cout << curr->value << " -> ";
            curr = curr->next;
        }
        std::cout << curr->value << std::endl;
    }
    bool del(int key) {
        if (head == nullptr) {
            return false;
        }
        if (head->value == key) {
            Node *temp = head;
            head = head->next;
            delete temp;
            return true;
        }
        Node *curr = head;
        while (curr->next != nullptr) {
            if (curr->next->value == key) {
                Node *temp = curr->next->next;
                delete curr->next;
                curr->next = temp;
                return true;
            }
            curr = curr->next;
        }
        return false;
    }
    void reverse() {
        if (head == nullptr || head->next == nullptr) {
            return;
        }
        Node *prev = head;
        Node *curr = head->next;
        head->next = nullptr;
        while (curr != nullptr) {
            Node *next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        head = prev;
    }

};

int main() {
    runtests();
    testcopy();
    testmove();
    return 0;
}

bool testcopy() {
    std::cout << "TESTING COPY\n";
    LinkedList l1 = LinkedList();
    l1.insert(2);
    l1.insert(3);
    l1.insert(4);
    std::cout << "Testing copy constructor\n";
    LinkedList l2 = l1;
    std::cout << "Expected: same output\n";
    l1.traverse();
    l2.traverse();
    std:: cout << "Expected: different linked lists\n";
    l2.del(4);
    l1.insert(5);
    l1.traverse();
    l2.traverse();
    std::cout << "Testing copy assignment operator\n";
    l1 = l2;
    std::cout << "Expected: same output\n";
    l1.traverse();
    l2.traverse();
    std::cout << "Testing edge cases\n";
    std::cout << "Expected: 3 empty lists and l1\n";
    LinkedList l3 = LinkedList();
    // Testing copy constructor with empty list
    LinkedList l4 = l3;
    l4.traverse();
    // Testing copy assignment with empty list
    l3 = l4;
    l3.traverse();
    // Testing empty list self-copy
    l4 = l4;
    l4.traverse();
    // Testing non-empty list self-copy
    l1 = l1;
    l1.traverse();
    return true;
}

bool testmove() {
    std::cout << "TESTING MOVE\n";
    LinkedList l1 = LinkedList();
    l1.insert(2);
    l1.insert(3);
    l1.insert(4);
    l1.traverse();
    std::cout << "Testing move constructor\n";
    LinkedList l2 = std::move(l1);
    std::cout << "Expected: l1 elements moved to l2\n";
    l1.traverse();
    l2.traverse();
    std::cout << "Testing move assignment operator\n";
    l1.insert(1);
    l2 = std::move(l1);
    std::cout << "Expected: all elements moved back to l2\n";
    l1.traverse();
    l2.traverse();
    return true;
}

bool runtests() {
    // Declare the linked list on the stack
    LinkedList list = LinkedList();
    list.insert(5);
    list.insert(6);
    list.insert(7);
    list.traverse();
    list.reverse();
    list.traverse();
    assert(!list.del(11));
    assert(list.del(7));
    list.traverse();
    assert(list.del(5));
    list.traverse();
    assert(list.del(6));
    list.traverse();
    return true;
}
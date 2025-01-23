#include <vector>
#include <iostream>
#include <string>
#include <memory>

constexpr int NUM_CHARS = 26;

class Node {
    friend class Trie;
private:
    bool isEnd;
    std::vector<std::unique_ptr<Node>> next;
public:
    Node() : isEnd(false), next(NUM_CHARS) {}
    std::unique_ptr<Node>& getNext(char c) {
        size_t index = c - 'a';
        if (index >= NUM_CHARS) {
            throw std::invalid_argument(std::format("Invalid character {}, index={}", c, index));
        }
        return next[index];
    }
    void createIfNotExist(char c) {
        size_t index = c - 'a';
        if (index >= NUM_CHARS) {
            throw std::invalid_argument(std::format("Invalid character {}, index={}", c, index));
        }
        if (!next[index]) {
            next[index] = std::make_unique<Node>();
        }
    }
};

class Trie {
private:
    std::unique_ptr<Node> root;
public:
    Trie() : root(std::make_unique<Node>()) {}
    void insert(const std::string& word) {
        Node *curr = root.get();
        for (char c : word) {
            curr->createIfNotExist(c);
            curr = curr->getNext(c).get();
        }
        curr->isEnd = true;
    }
    bool search(const std::string& word) {
        Node *curr = root.get();
        for (char c : word) {
            curr = curr->getNext(c).get();
            if (!curr) {
                return false;
            }
        }
        return curr->isEnd;
    }
    
    bool startsWith(const std::string& prefix) {
        Node *curr = root.get();
        for (char c : prefix) {
            curr = curr->getNext(c).get();
            if (!curr) {
                return false;
            }
        }
        bool hasNext = std::any_of(curr->next.begin(), curr->next.end(), 
            [](const std::unique_ptr<Node>& ptr){
                return ptr != nullptr;
            }
        );
        return (curr->isEnd || hasNext);
    }
};
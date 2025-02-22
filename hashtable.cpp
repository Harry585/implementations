/* 
Custom implementation of a hashtable in C++
call it map<key, value>
Properties:
- needs add method: needs to overload operator[]
- needs to value-initialize the value if operator[] looked at
- .contains()

How to resize a hashtable?
allocate a space twice as large
then, reallocate the entirety of the object pool
- how to use the hash?
Bonus: make it templated
Bonus: use smart pointer
Bonus: add locks and concurrency
*/

#include <iostream>
#include <memory>
#include <vector>
#include <climits>
#include <functional>
constexpr int START_SIZE = 16;
constexpr double RESIZE_THRESHOLD = 0.7;

template<typename Key, typename Value>
class Map {
private:
    size_t size;
    size_t capacity;
    std::vector<std::pair<Key, std::optional<Value>>> table;
    // what if there is no std::has<Key>?
    std::hash<Key> hasher;
    // resizing
    void resize(int newCapacity) {
        if (newCapacity < START_SIZE) {
            throw std::invalid_argument("New size is too small");
        }
        auto new_table = std::move(table);
        table.clear();
        table.resize(newCapacity);
        for (auto [k, v] : new_table) {
            if (v.has_value()) {
                insert(k, v.value());
            }
        }
    }
    // Gets the corresponding slot that a key is located in
    size_t getSlot(const Key& k) {
        size_t hash = hasher(k) % capacity;
        // Problem: we cannot know when to stop searching
        // Because the table might be null.
        while (table[hash].first != k && table[hash].first) {
            hash = (hash + 1) % capacity;
        }
        return hash;
    }
public:
    Map() : size(0), capacity(16), table(16) {}
    void insert(const Key& k, const Value& v) {
        size_t hash = getSlot(k);
        // If I am inserting, can I use const lvalue references for k and v?
        table[hash] = {k, v};
        size++;
        double load_factor = static_cast<double>(size) / capacity;
        if (load_factor >= RESIZE_THRESHOLD) {
            resize(2 * capacity);
        }
    }
    void erase(const Key& k) {
        size_t hash = getSlot(k);
        if (!table[hash].second.has_value()) {
            throw std::invalid_argument("Key not found");
        }
        // Leaves tombstone value
        table[hash] = {k, std::nullopt};
        size--;
    }
    // TODO: how to increment size if it's an assignment?
    Value& operator[](const Key& k) {
        size_t hash = getSlot(k);
        return table[hash].second.value();
    }
};

int main() {
    Map<std::string, int> ht;
    ht.insert("dog", 1);
    ht.insert("cat", 2);
    std::cout << std::format("hashtable: dog={}\n", ht["dog"]);
    std::cout << std::format("hashtable: cat={}\n", ht["cat"]);

    return 0;
}

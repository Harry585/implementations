/*
Custom vector implementation using dynamic arrays

Simplified layout:
attributes:
._size(): number of elements
._capacity(): number of elements holdable before array must be resized
operator[]: indexing
operator=: equality checking
.front(): return v[0]
.back(): returns v[v._size() - 1]
.push_back()
.pop_back(): pops the last element
.resize(): manually resizes the array
*/

/*
Invariants:
- _size > 0
- _capacity >= _size
_size refers to number of valid elements in vector
_capacity refers to _size of underlying array
*/

#include <iostream>
#include <algorithm>
#include <iterator>
#include <stdexcept>
template <typename T>
class Vector {
private:
    // Start with default _capacity = 16, does this work?
    int _size;
    int _capacity;
    T *arr;
    static constexpr int MIN_CAPACITY = 16;
public:
    // Delegated constructors
    Vector() : Vector(0, T{}) {}
    Vector(int n) : Vector(n, T{}) {}

    Vector(int n, T value) : _size(n) {
        if (n < 0) {
            throw std::domain_error("_Size passed into vector is negative");
        }
        // Set _capacity to be a power of two that contains n
        _capacity = MIN_CAPACITY;
        while (_capacity < n) {
            _capacity <<= 1;
        }
        arr = new T[_capacity];
        for (int i = 0; i < _size; i++) {
            // Set to 0 (or nullptr? How to handle?)
            arr[i] = value;
        }
    }
    // Destructor
    ~Vector() {
        std::cout << "Deleting vector\n" << std::endl;
        delete[] arr;
    }
    // Copy constructor
    Vector(const Vector<T>& other) : _size(other._size), _capacity(other._capacity), arr(new T[_capacity]){
        // Make a deep copy of the array
        for (int i = 0; i < _size; i++) {
            arr[i] = other.arr[i];
        }
    }
    // Copy assignment operator, returns lvalue reference
    // TODO: use copy-and-swap idiom
    Vector<T>& operator=(const Vector<T>& other) {
        if (this != &other) {
            this->_size = other._size;
            if (this->_capacity != other._capacity) {
                this->_capacity = other._capacity;
                delete[] arr;
                this->arr = new T[_capacity];
            }
            // Make a deep copy of the array
            for (int i = 0; i < _size; i++) {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }
    
    // Move constructor
    Vector(Vector<T>&& other) {
        this->_size = other._size;
        this->_capacity = other._capacity;
        this->arr = other.arr;
        // Ensure other is in a valid state
        other._size = 0;
        other._capacity = 0;
        other.arr = nullptr;
    }
    // Move assignment operator
    Vector<T>& operator=(Vector<T>&& other) {
        if (this != &other) {
            this->_size = other._size;
            this->_capacity = other._capacity;
            this->arr = other.arr;
            // Ensure other is in a valid state
            other._size = 0;
            other._capacity = 0;
            other.arr = nullptr;
        }
        return *this;
    }

    // Implement operator[]
    T& operator[](int index) {
        if (index < 0) {
            throw std::invalid_argument("Cannot subscript into negative index");
        } else if (index >= _size) {
            throw std::invalid_argument("Out of range");
        }
        return arr[index];
    }
    // Implement operator<< overload as free friend function
    friend std::ostream& operator<<(std::ostream& os, Vector& v) {
        if (v._size == 0) {
            os << "[]";
            return os;
        }
        os << '[';
        for (int i = 0; i < v._size - 1; i++) {
            os << v.arr[i] << ", ";
        }
        os << v.back() << ']';
        return os;
    }
    friend bool operator==(Vector v1, Vector v2) {
        if (v1._size != v2._size) return false;
        for (int i = 0; i < v1._size; i++) {
            if (v1[i] != v2[i]) return false;
        }
        return true;
    }
    void resize(int new_size) {
        if (new_size < _capacity) {
            _capacity = new_size;
            // Ensure _size is not greater than _capacity
            _size = std::min(_size, _capacity);
        } else if (new_size > _capacity) {
            // Reallocate memory
            T* new_arr = new T[new_size];
            // Copy across all elements
            for (int i = 0; i < _size; i++) {
                new_arr[i] = arr[i];
            }
            delete[] arr;
            arr = new_arr;
            _capacity = new_size;
        }
    }
    void push_back(const T& value) {
        if (_size == _capacity) {
            // resize
            _capacity *= 2;
            resize(_capacity);
        }
        arr[_size] = value;
        _size++;
    }
    // pop returns the popped value
    T& pop_back() {
        if (_size == 0) {
            throw std::invalid_argument("Cannot pop from empty vector");
        }
        // Does not resize
        return arr[--_size];
    }
    const T& front() {
        if (_size == 0) {
            throw std::invalid_argument("Cannot access front of empty vector");
        }
        return arr[0];
    }
    const T& back() {
        if (_size == 0) {
            throw std::invalid_argument("Cannot access back of empty vector");
        }
        return arr[_size - 1];
    }
    // We don't implement iterators but return yes or no
    bool find(const T& value) {
        for (int i = 0; i < _size; i++) {
            if (arr[i] == value) {
                return true;
            }
        }
        return false;
    }
    bool empty() {
        return _size == 0;
    }
    int size() {
        return _size;
    }
    int capacity() {
        return _capacity;
    }
};

int main() {
    // Attempt manually resizing while it's still simpler

    Vector<int> v1;
    v1.push_back(5);
    std::cout << "First element: " << v1[0] << std::endl;
    Vector<char> v2(3, 'a');
    v2.push_back('b');
    std::cout << "Vector starts and ends with" << v2.front() << v2.back() << std::endl;
    std::cout << std::format("Popped element is {}\n", v2.pop_back());
    std::cout << v2 << v1 << std::endl;
    Vector<char> v3(3, 'a');
    if (v2 == v3) {
        std::cout <<  v2 << "==" << v3 << std::endl;
    }
    while (!v2.empty()) {
        std::cout << "Popping: " << v2.pop_back() << std::endl;
    }
    std::cout << "Empty vector" << v2 << std::endl;
    std::cout << std::format("v1 Size={} capacity={}\n", v1.size(), v1.capacity());
    for (int i = 0; i < 16; i++) {
        v1.push_back(i + 10);
    }
    std::cout << v1 << std::endl;
    std::cout << std::format("v1 Size={} capacity={}\n", v1.size(), v1.capacity());


}
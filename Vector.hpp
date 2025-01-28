/*
Custom vector implementation using dynamic arrays

Simplified layout:
attributes:
._size(): number of elements
._capacity(): number of elements holdable before array must be resized
operator[]: indexing
operator==: equality checking
.front(): return v[0], requires nonempty vector
.back(): returns v[v._size() - 1] requires nonempty vector
.push_back()
.pop_back(): pops the last element
.resize(): manually resizes the array
.empty(): returns whether size of vector is 0
.clear(): erases all elements in vector and resets capacity to MIN_CAPACITY
*/

/*
Invariants:
- _size >= 0
- _capacity >= _size
- _capacity >= MIN_CAPACITY
_size refers to number of valid elements in vector
_capacity refers to _size of underlying array
*/
#pragma once
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
        delete[] arr;
    }
    // Copy constructor
    Vector(const Vector<T>& other) : _size(other._size), _capacity(other._capacity), arr(new T[_capacity]){
        // Make a deep copy of the array
        for (int i = 0; i < _size; i++) {
            arr[i] = other.arr[i];
        }
    }
    // Move constructor
    // NOTE: I assume that Vector<T> other is move-constructed.
    Vector(Vector<T>&& other) : _size(other._size), _capacity(other._capacity), arr(other.arr) {
        std::cout << "Move constructor called" << std::endl;
        other._size = 0;
        other._capacity = 0;
        other.arr = nullptr;
    }
    // Copy assignment operator, returns lvalue reference
    // Copy and swap idiom: other is copied by value
    Vector<T>& operator=(Vector<T>& other) {
        Vector<T> temp(other);
        swap(*this, temp);
        return *this;
    }
    Vector<T>& operator=(Vector<T>&& other) {
        Vector<T> temp(std::move(other));
        swap(*this, temp);
        return *this;
    }
    friend void swap(Vector<T>& first, Vector<T>&second) {
        // Enable ADL
        using std::swap;
        swap(first._size, second._size);
        swap(first._capacity, second._capacity);
        swap(first.arr, second.arr);
    }

    // Implement operator[]
    T& operator[](int index) const {
        if (index < 0) {
            throw std::invalid_argument("Cannot subscript into negative index");
        } else if (index >= _size) {
            throw std::invalid_argument("Out of range");
        }
        return arr[index];
    }
    // Implement operator<< overload as free friend function
    friend std::ostream& operator<<(std::ostream& os, Vector& v) const {
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
    friend bool operator==(const Vector& v1, const Vector& v2){
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

    void clear() {
        _size = 0;
        if (_capacity != MIN_CAPACITY) {
            _capacity = MIN_CAPACITY;
            delete[] arr;
            arr = new T[MIN_CAPACITY];
        }
    }
    const T& front() const {
        if (_size == 0) {
            throw std::invalid_argument("Cannot access front of empty vector");
        }
        return arr[0];
    }
    const T& back() const {
        if (_size == 0) {
            throw std::invalid_argument("Cannot access back of empty vector");
        }
        return arr[_size - 1];
    }
    // We don't implement iterators but return yes or no
    bool contains(const T& value) const {
        for (int i = 0; i < _size; i++) {
            if (arr[i] == value) {
                return true;
            }
        }
        return false;
    }
    bool empty() const {
        return _size == 0;
    }
    int size() const {
        return _size;
    }
    int capacity() const {
        return _capacity;
    }
};
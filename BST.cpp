#include <iostream>
#include <cassert>
#include <stdexcept>
#include <memory>

/*
Binary Search Tree implementation in C++
Abstract Data Type required operations:
- insert(value): inserts value
- delete(value): deletes value if exists
- find(value): returns yes/no for finding if value in the tree
Extra operations:
- Traverse(enum: order): traverses preorder, inorder, postorder

Features:
- Use templates to enable generic programming
- Use smart pointers to reduce possible memory errors
*/

void testInsert();
void testFind();
void testDelete();
void testTraverse();

enum class TraversalOrder {
    Preorder,
    Inorder,
    Postorder
};
// Friend class declaration requires forward declaration of Tree
template <typename T> class Tree;

template <typename T>
class Node {
    friend class Tree<T>;
private:
    // Raw pointer used for parent
    // Requires that parent is never deleted before child
    T value;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    Node *parent;
    void insertNode(T key) {
        if (value == key) {
            throw std::runtime_error("Key already found");
        }
        // Making this a reference is necessary
        std::unique_ptr<Node>& target = (key < value) ? left : right;
        if (!target) {
            target = std::make_unique<Node>(key);
            // Set parent of this Node
            target->parent = this;
        } else {
            target->insertNode(key);
        }
    }
    bool deleteNode(T key) {
        std::cout << "Value examined: " << value << std::endl;
        if (value == key) {
            // Determine the child pointer of this node's parent
            std::unique_ptr<Node>& ref = (parent->right.get() == this) ? parent->right : parent->left;
            std::cout << ref << std::endl;
            // Replace value with that of successor
            // If null, then this current node should be deleted
            if (left == nullptr && right == nullptr) {
                std::cout << "No children exist\n";
                ref = nullptr;
            } else if (right == nullptr) {
                std::cout << "Left child only with value " << left->value << std::endl;
                left->parent = this->parent;
                // Move ownership of left to ref
                ref = std::move(left);
            } else if (left == nullptr) {
                std::cout << "Right child only with value " << right->value << std::endl;
                right->parent = this->parent;
                // Move ownership of right node to ref
                ref = std::move(right);
            } else {
                std::optional<T> succ = successorNode(key, true);
                std::cout << "2 children; successor is " << succ.value_or(0) << std::endl;
                if (succ) {
                    this->value = *succ;
                } else {
                    throw std::runtime_error("Successor is null but right child exists");
                }
            }
            return true;
        }
        std::unique_ptr<Node>& target = (key < value) ? left : right;
        if (!target) {
            std::cout << key << " not found\n";
            return false;
        }
        return target->deleteNode(key);
    }
    bool findNode(T key) const {
        std::cout << "Find called on " << value << std::endl;
        if (key == value) {
            return true;
        }
        if (left == nullptr && right == nullptr) {
            return false;
        }
        if (key < value) {
            return left->findNode(key);
        } else {
            return right->findNode(key);
        }
    }
    std::optional<T> successorNode(T key, bool deleteRightChild=false) {
                if (right != nullptr) {
            Node *succ = right.get();
            while (succ->left != nullptr) {
                succ = succ->left.get();
            }
            T successor_value = succ->value;
            // Delete current node
            if (deleteRightChild) {
                // If successor(x) is the right child of x
                if (succ == this->right.get()) {
                    if (succ->right) {
                        succ->right->parent = this;
                        this->right = std::move(succ->right);
                    } else {
                        this->right = nullptr;
                    }
                } else {
                    if (succ->right) {
                        // Rearrange pointers to replace succ with right child
                        succ->right->parent = succ->parent;
                        succ->parent->left = std::move(succ->right);
                    } else {
                        // Set parent pointer to null
                        succ->parent->left = nullptr;
                    }
                }
            }
            return successor_value;
        }
        // Else, traverse up to the parent
        Node *prev = this->parent;
        while (prev != nullptr) {
            // Return the first ancestor which contains us in left node
            if (prev->left.get() == this) {
                return prev->value;
            }
            prev = prev->parent;
        }
        return std::nullopt;
    }
    // Traversal functions
    std::vector<T>& inorder(std::vector<T>& result) const {
        if (left) {
            left->inorder(result);
        }
        result.push_back(value);
        if (right) {
            right->inorder(result);
        }
        return result;
    }
    std::vector<T>& preorder(std::vector<T>& result) const {
        result.push_back(value);
        if (left) {
            left->preorder(result);
        }
        if (right) {
            right->preorder(result);
        }
        return result;
    }
    std::vector<T>& postorder(std::vector<T>& result) const {
        if (left) {
            left->postorder(result);
        }
        if (right) {
            right->postorder(result);
        }
        result.push_back(value);
        return result;
    }
public:
    Node(T value) : value(value), left(nullptr), right(nullptr) {}
    // Getter methods
    T getValue() {return value;}
    // Return raw pointers which cannot violate ownership semantics
    Node<T> *getLeft() {return left.get();}
    Node<T> *getRight() {return right.get();}
};

template <typename T>
class Tree {
private:
    std::unique_ptr<Node<T>> root;
public:
    // Keep default construtor as-is
    Tree() = default;
    // Allow construction from initializer list
    Tree(std::initializer_list<T> values) : root(nullptr) {
        for (const T value : values) {
            this->insert(value);
        }
    }
    // Returns raw pointer which cannot violate ownership semantics
    Node<T> *getRoot() {return root.get();}

    void insert(T key) {
        if (!root) {
            root = std::make_unique<Node<T>>(key);
        } else {
            root->insertNode(key);
        }
    }
    /* Deletes the node with a given key. Return true on success, false on failure */
    bool del(T key) {
        if (!root) {
            return false;
        }
        if (key == root->value) {
            std::cout << "Deleting root\n";
            if (!root->left && !root->right) {
                root = nullptr;
            } else if (!root->left) {
                root = std::move(root->right);
            } else if (!root->right) {
                root = std::move(root->left);
            } else {
                std::optional<T> succ = root->successorNode(key, true);
                root->value = succ.value();
            }
            return true;
        } else {
            return root->deleteNode(key);
        }
    }
    bool find(T key) {
        if (!root) {
            return false;
        }
        return root->findNode(key);
    }

    /* Returns successor of key, null if successor is not defined */
    std::optional<T> successor(T key) {
        if (!root) {
            return std::nullopt;
        }
        return root->successorNode(key);
    }
    // Force myself to practice using Enums
    std::vector<T> traverse(TraversalOrder order) const {
        std::vector<T> result;
        if (root) {
            if (order == TraversalOrder::Inorder) {
                root->inorder(result);
            } else if (order == TraversalOrder::Preorder) {
                root->preorder(result);
            } else {
                root->postorder(result);
            }
        }
        return result;
    }
};

template <typename T>
void printVector(std::vector<T> v) {
    if (v.empty()) {
        std::cout << "Vector empty" << std::endl;
        return;
    }
    std::cout << "[";
    for (int i = 0; i < v.size() - 1; i++) {
        std::cout << v[i] << ", ";
    }
    std::cout << v.back() << "]" << std::endl;
}

int main() {
    std::cout << "Running tests\n";
    testInsert();
    testFind();
    testTraverse();
    testDelete();
}

void testInsert() {
    std::unique_ptr<Tree<int>> tree = std::make_unique<Tree<int>>();
    tree->insert(9);
    tree->insert(5);
    tree->insert(11);
    tree->insert(7);
    assert(tree->getRoot()->getValue() == 9);
    assert(tree->getRoot()->getLeft()->getValue() == 5);
    assert(tree->getRoot()->getRight()->getValue() == 11);
    assert(tree->getRoot()->getLeft()->getRight()->getValue() == 7);
    try {
        tree->insert(7);
        assert(false);
    } catch (const std::exception& e) {
        // Do nothing
    }
    std::cout << "Insert test passed" << std::endl;
}

void testFind() {
    std::unique_ptr<Tree<double>> tree = std::make_unique<Tree<double>>();
    tree->insert(9.9);
    tree->insert(5.5);
    tree->insert(11);
    tree->insert(7);
    assert(tree->find(7));
    assert(tree->find(5.5));
    assert(tree->find(11));
    assert(tree->find(9.9));
    assert(!tree->find(5.51));
    std::cout << "Find test passed" << std::endl;
}

void testDelete() {
    Tree<char> tree = {'g', 'a', 'c', 'm', 'z', 'b', 'd', 'h', 'y'};
    assert(tree.getRoot()->getValue() == 'g');
    // Left subtree
    assert(tree.getRoot()->getLeft()->getValue() == 'a');
    assert(tree.getRoot()->getLeft()->getRight()->getValue() == 'c');
    assert(tree.getRoot()->getLeft()->getRight()->getLeft()->getValue() == 'b');
    assert(tree.getRoot()->getLeft()->getRight()->getRight()->getValue() == 'd');
    // Right subtree
    assert(tree.getRoot()->getRight()->getValue() == 'm');
    assert(tree.getRoot()->getRight()->getLeft()->getValue() == 'h');
    assert(tree.getRoot()->getRight()->getRight()->getValue() == 'z');
    assert(tree.getRoot()->getRight()->getRight()->getLeft()->getValue() == 'y');
    assert(!tree.del('x'));
    printVector(tree.traverse(TraversalOrder::Inorder));
    printVector(tree.traverse(TraversalOrder::Preorder));
    // Deleting node with one child
    assert(tree.del('z'));
    assert(tree.getRoot()->getRight()->getRight()->getValue() == 'y');
    // Deleting node with no successor
    assert(tree.del('y'));
    assert(!tree.getRoot()->getRight()->getRight());
    // Deleting node with two children
    assert(tree.del('c'));
    assert(tree.getRoot()->getLeft()->getRight()->getValue() == 'd');
    assert(tree.getRoot()->getLeft()->getRight()->getLeft()->getValue() == 'b');
    // Deleting root
    assert(tree.del('g'));
    printVector(tree.traverse(TraversalOrder::Inorder));
    printVector(tree.traverse(TraversalOrder::Preorder));
    // Deleting root
    assert(tree.del('h'));
    printVector(tree.traverse(TraversalOrder::Inorder));
    printVector(tree.traverse(TraversalOrder::Preorder));
    // Deleting d, 1 left child
    assert(tree.del('d'));
    printVector(tree.traverse(TraversalOrder::Inorder));
    printVector(tree.traverse(TraversalOrder::Preorder));
    // Deleting a, 1 right child
    assert(tree.del('a'));
    printVector(tree.traverse(TraversalOrder::Inorder));
    printVector(tree.traverse(TraversalOrder::Preorder));
    // Deleting m, root with 1 left child
    assert(tree.del('m'));
    printVector(tree.traverse(TraversalOrder::Inorder));
    printVector(tree.traverse(TraversalOrder::Preorder));
    // Deleting b, root with no children
    assert(tree.del('b'));
    std::cout << "root: " << tree.getRoot() << std::endl;
    printVector(tree.traverse(TraversalOrder::Inorder));
    printVector(tree.traverse(TraversalOrder::Preorder));
    std::cout << "Deletion test passed\n";
}

void testTraverse() {
    // This results in a highly unbalanced tree
    Tree<std::string> tree = {"armadillo", "boronia", "maleficient", \
    "zoonotic", "neurotic", "phantasia", "bibliophile"};
    printVector(tree.traverse(TraversalOrder::Inorder));
    printVector(tree.traverse(TraversalOrder::Preorder));
    printVector(tree.traverse(TraversalOrder::Postorder));
}
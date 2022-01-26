/** UNIVERSIDAD DE LAS FUERZAS ARMADAS "ESPE"
*			INGENIERIA SOFTWARE
*
*AUTORES: Luis Fernando Cueva
*Leonardo de la Cadena
*Steven Achig
*Eduardo Mortensen
*ARCHIVO: binary_tree.h
* */
#pragma once
#include "tree_node.h"
#include <optional>
#include <functional>

enum class TraverseDirection {
    Inorder,
    Postorder,
    PreOrder
};

template <typename T>
class BinaryTree {
public:

    BinaryTree() = default;
    BinaryTree(TreeNode<T>* root) : root_(root) {}
    void insert(T value);
    void remove(T value);
    void traverse(std::function<void(T)> visitor, TraverseDirection direction = TraverseDirection::Inorder);
    void traverse(std::function<void(TreeNode<T>*)> visitor, TraverseDirection direction = TraverseDirection::Inorder);
    std::optional<T> find(T value);
    TreeNode<T>* root();

private:
    TreeNode<T>* insert(TreeNode<T>*& node, T value);
    TreeNode<T>* remove(TreeNode<T>*& node, T value);
    std::optional<T> find(TreeNode<T>*& node, T value);
    void traverse_inorder(TreeNode<T>* node, std::function<void(T)> visitor);
    void traverse_preorder(TreeNode<T>* node, std::function<void(T)> visitor);
    void traverse_postorder(TreeNode<T>* node, std::function<void(T)> visitor);
    void traverse_inorder(TreeNode<T>* node, std::function<void(TreeNode<T>*)> visitor);

    int height(TreeNode<T>* current);
    int calculate_height(TreeNode<T>* node);
    TreeNode<T>* root_ = nullptr;
};

template <typename T>
void BinaryTree<T>::insert(T value) {
    root_ = insert(root_, value);
}

template <typename T>
TreeNode<T>* BinaryTree<T>::insert(TreeNode<T>*& node, T value) {
    if (node == nullptr) {
        node = new TreeNode<T>(value);
    } else if (node->value() < value) {
        node->right(insert(node->right(), value));
        node->height(std::max(node->height(), node->right()->height() + 1));
    } else {
        node->left(insert(node->left(), value));
        node->height(std::max(node->height(), node->left()->height() + 1));
    }

    return node;
}

template<typename T>
inline TreeNode<T>* BinaryTree<T>::remove(TreeNode<T>*& node, T value) {
    if (node == nullptr) return node;

    if (value < node->value()) {
        node->left(remove(node->left(), value));
    } else if (value > node->value()) {
        node->right(remove(node->right(), value));
    } else {
        TreeNode<T>* r = node->right();

        if (node->right() == nullptr) {
            TreeNode<T>* l = node->left();
            delete node;
            node = l;
        } else if (node->left() == nullptr) {
            delete node;
            node = r;
        } else {
            while (r->left() != nullptr) r = r->left();
            node->value(r->value());
            node->right(remove(node->right(), r->value()));
        }
    }

    return node;
}

template<typename T>
inline std::optional<T> BinaryTree<T>::find(TreeNode<T>*& node, T value) {
    if (node == nullptr) return std::nullopt;

    T v = node->value();

    if (v == value) return { node->value() };
    if (v > value) return find(node->left(), value);
    if (v < value) return find(node->right(), value);
}

template<typename T>
inline void BinaryTree<T>::traverse_inorder(TreeNode<T>* node, std::function<void(T)> visitor) {
    if (node == nullptr) return;

    traverse_inorder(node->left(), visitor);
    visitor(node->value());
    traverse_inorder(node->right(), visitor);
}

template<typename T>
inline void BinaryTree<T>::traverse_inorder(TreeNode<T>* node, std::function<void(TreeNode<T>*)> visitor) {
    if (node == nullptr) return;

    traverse_inorder(node->left(), visitor);
    visitor(node);
    traverse_inorder(node->right(), visitor);
}

template<typename T>
inline void BinaryTree<T>::traverse_preorder(TreeNode<T>* node, std::function<void(T)> visitor) {
    if (node == nullptr) return;

    visitor(node->value());
    traverse_preorder(node->left(), visitor);
    traverse_preorder(node->right(), visitor);
}

template<typename T>
inline void BinaryTree<T>::traverse_postorder(TreeNode<T>* node, std::function<void(T)> visitor) {
    if (node == nullptr) return;

    traverse_preorder(node->left(), visitor);
    traverse_preorder(node->right(), visitor);
    visitor(node->value());
}

template<typename T>
inline int BinaryTree<T>::height(TreeNode<T>* current) {
    return current == nullptr ? -1 : current->height();
}

template<typename T>
inline int BinaryTree<T>::calculate_height(TreeNode<T>* node) {
    int left_height = height(node->left());
    int right_height = height(node->right());

    return std::max(left_height, right_height) + 1;
}

template <typename T>
void BinaryTree<T>::remove(T value) {
    root_ = remove(root_, value);
}

template<typename T>
void BinaryTree<T>::traverse(std::function<void(T)> visitor, TraverseDirection direction) {
    if (direction == TraverseDirection::Inorder) {
        traverse_inorder(root_, visitor);
    } else if (direction == TraverseDirection::PreOrder) {
        traverse_preorder(root_, visitor);
    } else if (direction == TraverseDirection::Postorder) {
        traverse_postorder(root_, visitor);
    }
}

template<typename T>
void BinaryTree<T>::traverse(std::function<void(TreeNode<T>*)> visitor, TraverseDirection direction) {
    if (direction == TraverseDirection::Inorder) {
        traverse_inorder(root_, visitor);
    } else if (direction == TraverseDirection::PreOrder) {
        traverse_preorder(root_, visitor);
    } else if (direction == TraverseDirection::Postorder) {
        traverse_postorder(root_, visitor);
    }
}

template<typename T>
std::optional<T> BinaryTree<T>::find(T value) {
    return find(root_, value);
}

template<typename T>
inline TreeNode<T>* BinaryTree<T>::root() {
    return root_;
}

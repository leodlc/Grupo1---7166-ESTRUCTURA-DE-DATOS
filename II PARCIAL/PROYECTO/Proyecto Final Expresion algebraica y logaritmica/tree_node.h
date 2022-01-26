#pragma once

template <typename T>
class TreeNode {
public:
    explicit TreeNode(T value);

    T value();
    void value(T value);

    int height();
    void height(int height);

    TreeNode<T>* &left();
    void left(TreeNode<T>* left);

    TreeNode<T>* &right();
    void right(TreeNode<T>* right);

private:
    T value_;
    TreeNode<T> *left_ = nullptr;
    TreeNode<T> *right_ = nullptr;
    int height_ = 0;
};

template<typename T>
TreeNode<T>::TreeNode(T value) : value_(value) {}

template<typename T>
T TreeNode<T>::value() {
    return value_;
}

template<typename T>
void TreeNode<T>::value(T value) {
    value_ = value;
}

template<typename T>
int TreeNode<T>::height() {
    return height_;
}

template<typename T>
void TreeNode<T>::height(int height) {
    height_ = height;
}

template<typename T>
TreeNode<T>*& TreeNode<T>::left() {
    return left_;
}

template<typename T>
void TreeNode<T>::left(TreeNode<T> *left) {
    left_ = left;
}

template<typename T>
TreeNode<T>*& TreeNode<T>::right() {
    return right_;
}

template<typename T>
void TreeNode<T>::right(TreeNode<T> *right) {
    right_ = right;
}

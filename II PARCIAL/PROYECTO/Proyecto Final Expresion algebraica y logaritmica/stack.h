#pragma once
#include "doubly_linked_list.h"

template <typename T>
class Stack {
public:
    void push(const T& value);
    void pop();
    const T top();
    bool empty();
    size_t size();

private:
    DoublyLinkedList<T> elements;
};

template <typename T>
void Stack<T>::pop() {
    if (empty()) {
        return;
    }

    elements.remove_at(elements.size() - 1);
}

template <typename T>
const T Stack<T>::top() {
    if (empty()) {
        throw std::runtime_error("Empty stack");
    }

    return elements.at(elements.size() - 1);
}

template <typename T>
void Stack<T>::push(const T& value) {
    elements.push_back(value);
}

template <typename T>
bool Stack<T>::empty() {
    return elements.empty();
}

template <typename T>
size_t Stack<T>::size() {
    return elements.size();
}
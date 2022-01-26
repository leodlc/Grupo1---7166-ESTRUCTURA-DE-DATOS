#pragma once
#include "node.h"
#include <functional>
#include <stdexcept>
#include <exception>

template <typename T>
class DoublyLinkedList {
public:
    DoublyLinkedList() = default;

    void push_back(const T &value);
    void push_front(const T &value);
    void push_at(int index, const T &value);
    void remove_at(int index);

    void for_each(std::function<void(Node<T> *, int)> callback);
    void for_each(std::function<void(Node<T>*)> callback);
    void for_each(std::function<void(T, int)> callback);
    void for_each(std::function<void(T)> callback);
    
    void until(std::function<bool(Node<T>*)> callback);
    void until(std::function<bool(T)> callback);

    Node<T> *find(std::function<bool(Node<T> *, int)> callback);
    Node<T>* find(std::function<bool(Node<T>*)> callback);
    Node<T> *find(std::function<bool(T, int)> callback);
    Node<T>* find(std::function<bool(T)> callback);

    T at(int index);

    void clear();
    int size();
    bool empty();

private:
    Node<T> *node_at(int index);
    Node<T> *last();

    Node<T> *head = nullptr;
    int _size = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void DoublyLinkedList<T>::push_back(const T &value) {
    _size++;

    if (head == nullptr) {
        head = new Node<T>(value);
        return;
    }

    Node<T> *last_node = last();
    Node<T> *node = new Node<T>(value);

    last_node->set_next(node);
    node->set_previous(last_node);
}

template <typename T>
void DoublyLinkedList<T>::push_front(const T &value) {
    if (head == nullptr) {
        return push_back(value);
    }

    Node<T> *node = new Node<T>(value);
    Node<T> *temp = head;

    head = node;
    head->set_next(temp);
    temp->set_previous(head);
    _size++;
}

template <typename T>
void DoublyLinkedList<T>::push_at(int index, const T &value) {
    if (index < 0 || index > _size) {
        throw std::invalid_argument("index out of bounds");
    }

    if (index == 0 || _size == 0) {
        return push_front(value);
    } else if (index == _size) {
        return push_back(value);
    }

    Node<T> *target = node_at(index);
    Node<T> *previous = target->get_previous();
    Node<T> *next = target->get_next();
    Node<T> *nodo = new Node<T>(value);

    previous->set_next(nodo);
    nodo->set_next(target);
    nodo->set_previous(previous);
    target->set_previous(nodo);
    _size++;
}

template <typename T>
void DoublyLinkedList<T>::remove_at(int index) {
    if (index < 0 || index >= _size) {
        throw std::invalid_argument("index out of bounds");
    }
    
    if (_size == 1) {
        delete head;
        head = nullptr;
    } else if (index == 0) {
        Node<T> *temp = head;
        head = head->get_next();
        head->set_previous(nullptr);
        delete temp;
    } else if (index == _size - 1) {
        Node<T> *node = last();
        Node<T> *previous = node->get_previous();
        
        previous->set_next(nullptr);
        delete node;
    } else {
        Node<T> *target = node_at(index);
        Node<T> *previous = target->get_previous();
        Node<T> *next = target->get_next();

        previous->set_next(next);
        next->set_previous(previous);
        delete target;
    }

    _size--;
}

template<typename T>
void DoublyLinkedList<T>::for_each(std::function<void(Node<T>*, int)> callback) {
    Node<T> *current = head;
    int index = 0;

    while (current != nullptr) {
        callback(current, index);
        current = current->get_next();
        index++;
    }
}

template<typename T>
void DoublyLinkedList<T>::for_each(std::function<void(Node<T>*)> callback) {
    for_each([&](Node<T> *node, int index) {
        callback(node->get_data());
    });
}

template<typename T>
void DoublyLinkedList<T>::for_each(std::function<void(T)> callback) {
    for_each([&](Node<T> *node, int index) {
        callback(node->get_data());
    });
}

template<typename T>
void DoublyLinkedList<T>::for_each(std::function<void(T, int)> callback) {
    for_each([&](Node<T> *node, int index) {
        callback(node->get_data(), index);
    });
}

template<typename T>
void DoublyLinkedList<T>::until(std::function<bool(Node<T>*)> callback) {
    Node<T> *current = head;

    while (current != nullptr) {
        if (!callback(current)) break;
        current = current->get_next();
    }
}

template<typename T>
void DoublyLinkedList<T>::until(std::function<bool(T)> callback) {
    until([&](Node<T> *node) {
        return callback(node->get_data());
    });
}

template<typename T>
Node<T> *DoublyLinkedList<T>::find(std::function<bool(Node<T> *, int)> callback) {
    Node<T> *current = head;
    int index = 0;

    while (current != nullptr) {
        if (callback(current, index++)) {
            return current;
        }

        current = current->get_next();
    }

    return nullptr;
}

template<typename T>
Node<T> *DoublyLinkedList<T>::find(std::function<bool(Node<T> *)> callback) {
    return find([&](Node<T> *node, int index) -> bool {
        return callback(node->get_data());
    });
}

template<typename T>
Node<T> *DoublyLinkedList<T>::find(std::function<bool(T)> callback) {
    return find([&](Node<T> *node, int index) -> bool {
        return callback(node->get_data());
    });
}

template<typename T>
Node<T> *DoublyLinkedList<T>::find(std::function<bool(T, int)> callback) {
    return find([&](Node<T> *node, int index) -> bool {
        return callback(node->get_data(), index);
    });
}

template <typename T>
T DoublyLinkedList<T>::at(int index) {
    return node_at(index)->get_data();
}

template <typename T>
void DoublyLinkedList<T>::clear() {
    while (!empty()) {
        remove_at(0);
    }
}

template <typename T>
int DoublyLinkedList<T>::size() {
    return _size;
}

template <typename T>
bool DoublyLinkedList<T>::empty() {
    return _size == 0;
}

template<typename T>
Node<T> *DoublyLinkedList<T>::last() {
    if (_size == 0) {
        return nullptr;
    }
    
    return node_at(_size - 1);
}

template<typename T>
Node<T> *DoublyLinkedList<T>::node_at(int index) {
    if (index < 0 || index >= _size) {
        throw std::invalid_argument("index out of bounds");
    }

    if (index == 0) {
        return head;
    }

    int current_index = 0;
    Node<T> *last = head;

    while (last->get_next() != nullptr) {
        if (current_index == index) {
            break;
        }

        current_index++;
        last = last->get_next();
    }

    return last;
}

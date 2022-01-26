#pragma once

template<typename T>
class Node {
    public:
        Node(T data);
        Node(T data, Node<T> *next);
        Node(T data, Node<T> *next, Node<T> *previous);

        void set_next(Node<T> *next);
        Node<T> *get_next();

        void set_previous(Node<T> *next);
        Node<T> *get_previous();

        void set_data(T data);
        T get_data();

    private:
        T data;
        Node<T> *next = nullptr;
        Node<T> *previous = nullptr;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
Node<T>::Node(T data) {
    this->data = data;
    this->next = nullptr;
    this->previous = nullptr;
}

template<typename T>
Node<T>::Node(T data, Node<T> *next) {
    this->data = data;
    this->next = next;
    this->previous = nullptr;
}

template<typename T>
Node<T>::Node(T data, Node<T> *next, Node<T> *previous) {
    this->data = data;
    this->next = next;
    this->previous = previous;
}

template<typename T>
void Node<T>::set_data(T data) {
    this->data = data;
}

template<typename T>
T Node<T>::get_data() {
    return data;
}

template<typename T>
void Node<T>::set_next(Node<T> *next) {
    this->next = next;
}

template<typename T>
Node<T> *Node<T>::get_next() {
    return next;
}

template<typename T>
void Node<T>::set_previous(Node<T> *previous) {
    this->previous = previous;
}

template<typename T>
Node<T> *Node<T>::get_previous() {
    return previous;
}
#ifndef STACK_H
#define STACK_H

template <typename T> struct Node
{
    T value;
    Node* next;
};

template <typename T> class Stack
{
  public:
    Stack();
    Stack(T element);
    void push(T element);
    void pop();
    void print();
    ~Stack();

  private:
    Node<T>* head;
};

#include <iostream>

template <typename T> Stack<T>::Stack() = default;

template <typename T> Stack<T>::Stack(T val)
{
    head = new Node<T>;
    head->value = val;
    head->next = nullptr;
}

template <typename T> void Stack<T>::push(T val)
{
    Node<T>* temp = new Node<T>;
    temp->next = this->head;
    temp->value = val;

    this->head = temp;
}

template <typename T> void Stack<T>::pop()
{
    Node<T>* temp = this->head->next;

    delete this->head;
    this->head = temp;
}
template <typename T> void Stack<T>::print()
{
    Node<T>* curr = this->head;
    while (curr->next != nullptr)
    {
        Node<T>* temp = curr->next;
        std::cout << curr->value << ", ";
        curr = temp;
    }
    std::cout << curr->value << std::endl;
}

template <typename T> Stack<T>::~Stack()
{
    Node<T>* curr = this->head;
    while (curr != nullptr)
    {
        Node<T>* temp = curr->next;
        delete curr;
        curr = temp;
    }
}
#endif

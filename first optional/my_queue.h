#ifndef QUEUE_H
#define QUEUE_H

#include "linked_list.h"

using namespace std;

template < typename Type>
class Queue {

private:

    // List as linked list
    DoubleLinkedList<Type> linkedList;
public:

    // Check if list is empty or not
    bool empty() const;

    // Push in back of list
    void push(Type const &);

    // Pop of front of list
    Type pop();

};

template < typename Type>

bool Queue<Type>::empty() const {
    return linkedList.empty();
};

template < typename Type>

void Queue<Type>::push(Type const &value) {
    linkedList.pushBack(value);
};

template < typename Type>

Type Queue<Type>::pop() {

    Type front = linkedList.front();
    linkedList.popFront();
    return front;
};


#endif

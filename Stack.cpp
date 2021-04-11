#pragma once
#include <iostream>

using namespace std;

template <class T, int max_size > class Stack {
    T s[max_size];
    int top; 
public:
    Stack() { top = 0; }
    void reset() { top = 0; }
    
    void push(T i);
    T pop();

    bool is_full() { return top == max_size; }
 public :   
    bool empty() { return top <= 0; }
    void clear() { top = 0; }
    void print();
    void reverse();
};
    
template <class T, int max_size >
void Stack <T, max_size >::push(T i) {
    if ( !is_full() ) {
        s[top] = i;
        ++top; 
    }
else
    throw "Stack_is_full";
}

template <class T, int max_size >
T Stack <T, max_size >::pop() {
    if ( !empty() ) {
        --top;
        return s[top];
    }
    else
        throw "Stack_is_empty";
}

template <class T, int max_size >
void Stack<T, max_size >::print() {
    cout << "Stack : " << endl;
    for(int i = 0; i < top; i++) {
        cout << s[i];
    }
}

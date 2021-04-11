#pragma once
#include <iostream>
#include "Lex.cpp"

class Poliz {
    Lex *p; 
    int size; 
    int top;
    
public:
    Poliz ( int max_size ) {
        p = new Lex [size = max_size];
        top = 0; 
    };

    ~Poliz() { delete []p; };
    
    void put_lex(Lex l) { p[top]=l; ++top; }; 
    void put_lex(Lex l, int place) { p[place]=l; }; 
    
    void blank() { ++top; };
    
    int get_top() { return top; };

    Lex& operator[] ( int index ) {
        if (index > size)
            throw "POLIZ:out of array";
        else
            if ( index > top )
                throw "POLIZ:indefinite element of array";
            else
                return p[index];
    }

    void clear() { top = 0; }

    void print() {
        for ( int i = 0; i < top; ++i ) 
        cout << i << " " <<  p[i];
    }
};
// g++ -D DEBUG -o main main.cpp
// g++ -o main main.cpp
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#include "mem_manage.h"
#endif 


void buffer_overrun() {
    int *p = new int[10];
    //p[-1] = 10;
    //p[10] = 10;
    delete p;
}

void memory_leak() {
    int *p = new int[100];
}

void invalid_delete_1() {
    int *p;
    delete p;
}

void invalid_delete_2() {
    int *p = new int;
    delete p;
    delete p; // invalid
}

void valid() {
    int *p = new int;
    int *q = new int;
    delete q;
    delete p;
}

int main() {

    std::cout << "\nBUFFER OVERFLOW\n-------------------------------------------------------------\n";
    buffer_overrun();
    std::cout << "\nMEMORY LEAK\n-------------------------------------------------------------\n";
    memory_leak();
    std::cout << "\nINVALID DELETE 1\n-------------------------------------------------------------\n";
    invalid_delete_1();
    std::cout << "\nINVALID DELETE 2\n-------------------------------------------------------------\n";
    invalid_delete_2();
    std::cout << "\nVALID\n-------------------------------------------------------------\n";
    valid();
    std::cout << std::endl << "COMPLETE\n\n";
    return 0;
}
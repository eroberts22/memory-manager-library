#pragma once

#include <iostream>
#include <stdlib.h>
#include <map>
#undef new
#undef delete

////////////////////
// Memory Manager //
////////////////////
class MemoryManager {

    // memory entry struct
    struct Entry {
        size_t size;
        const char* filename;
        int line;

        Entry() {}
        Entry(size_t s,const char* f, int l) : size(s), filename(f), line(l) {}
    };

    public:

    std::map<void*, Entry> entriesInMemory;

    // allocating memory
    void * m_malloc(size_t size, const char* filename, int line) {

        void * p = malloc(size);
        std::cout << "Allocating (" << filename << ":" << line << ") address: " << p << " size: " << size <<"\n";

        // add entry in memory: address, filename, line, size
        addEntry(p,Entry(size,filename,line));

        return p;
    }

    // deallocating memory
    void m_free(void * p) {

        // delete entry in memory using address
        removeEntry(p);
    }

    void addEntry(void * p, Entry e) {
        // add entry with address, filename, line, size
        entriesInMemory.insert(std::pair<void*, Entry>(p,e));
    }

    void removeEntry(void * p) {
        // check if entry exists by address
        // delete if exists
        // if entry does not exist throw error
        if(entriesInMemory.find(p) != entriesInMemory.end()) {
            entriesInMemory.erase(p);
            std::cout << " address: " << p << std::endl;
            free(p);
        } else {
            std::cerr << " error: cannot deallocate, entry does not exist\n";
        }
    }

    // print any entries currently in the map
    void printEntries() {
        for (std::pair<void*,Entry>e: entriesInMemory) {
            std::cout << "(" << e.second.filename << ":" << e.second.line << ") address: " << e.first << "\n";
        }
        std::cout << std::endl;
    }

    // return true if there are no entries in memory
    bool empty() {
        return entriesInMemory.empty();
    }
};

static MemoryManager m;

////////////////////////
// Operator Overloads //
////////////////////////

// overload new operator
void * operator new (size_t size, const char* filename, int line) {
    void * p;
    try {
        p = m.m_malloc(size,filename,line);
    } catch (std::bad_alloc& ba) {
        std::cerr << "bad_alloc caught: " << ba.what() << std::endl;
    }
    return p;
}

// overload new [] operator
void * operator new [] (size_t size, const char* filename, int line) {
    void * p;
    try {
        p = m.m_malloc(size,filename,line);
    } catch (std::bad_alloc& ba) {
        std::cerr << "bad_alloc caught: " << ba.what() << std::endl;
    }
    return p;
}

// overload delete operator
void operator delete (void * p, size_t size) noexcept {
    m.m_free(p);
}

// overload delete [] operator
void operator delete [](void * p, size_t size) noexcept{
    m.m_free(p);
}


void access(int A) {
    std::cout << "work\n";
}

// used to output file and line name for deallocate
void mdelete(const char* filename, int line) {
    std::cout << "Deallocating (" << filename << ":" << line << ")";
    return;
}

// when program exits: check for memory leak
// by checking for any allocated memory that was not deallocated
void atexit_check() {
    // if map of entries is not empty, something was not deallocated properly
    if(!m.empty()) {
        std::cout << "error: memory leak(s) detected:" << std::endl;
        m.printEntries(); // print entries that remain in map
    }
}

const int mem_leak_check = std::atexit(atexit_check);

#define new new(__FILE__,__LINE__)
#define delete mdelete(__FILE__,__LINE__), delete


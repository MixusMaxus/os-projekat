//
// Created by os on 1/19/24.
//

#ifndef PROJECT_BASE_MEMORYALLOCATOR_H
#define PROJECT_BASE_MEMORYALLOCATOR_H

#include "../lib/hw.h";

struct Element{
    size_t size;
    Element* next;
};

class MemoryAllocator {
private:
    static Element* freeHead;
    static int tryToJoin(Element*);
public:
    static void* mem_alloc(size_t);
    static int mem_free(void*);
    static void initialize();
};


#endif //PROJECT_BASE_MEMORYALLOCATOR_H

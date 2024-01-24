//
// Created by os on 1/19/24.
//

#include "../h/MemoryAllocator.h"

Element* MemoryAllocator::freeHead = nullptr;

void MemoryAllocator::initialize() {
    freeHead = (Element*)HEAP_START_ADDR;
    freeHead->next = nullptr;
    freeHead->size = ((size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR - sizeof(Element)) / MEM_BLOCK_SIZE;
}

int MemoryAllocator::mem_free(void *ptr) {
    if (ptr == nullptr)
        return -1;
    Element* cur = nullptr;
    if (freeHead == nullptr || ptr < (char*)freeHead)
        cur = nullptr;
    else{
        for (cur = freeHead; cur->next != nullptr && ptr > (char*)(cur->next); cur = cur->next){}
    }
    Element* newSeg = ((Element*)((char*)ptr - sizeof(Element)));
    if (cur != nullptr)
        newSeg->next = cur->next;
    else
        newSeg->next = freeHead;
    if (cur != nullptr)
        cur->next = newSeg;
    else
        freeHead = newSeg;
    tryToJoin(newSeg);
    tryToJoin(cur);
    return 0;
}

void *MemoryAllocator::mem_alloc(size_t size) {
    Element *blk = freeHead, *prev = nullptr;

    for (; blk != nullptr; prev = blk, blk = blk->next){
        if (blk->size >= size)
            break;
    }
    size_t remainingSize = blk->size - size;
    if (remainingSize >= sizeof(Element) + MEM_BLOCK_SIZE){
        blk->size = size;
        size_t offset = sizeof(Element) + size;
        Element *newBlk = (Element*)((char*)blk + offset);
        if (prev != nullptr)
            prev->next = newBlk;
        else
            freeHead = newBlk;
        newBlk->next = blk->next;
        newBlk->size = remainingSize - sizeof(Element);
    }
    else{
        if (prev)
            prev->next = blk->next;
        else
            freeHead = blk->next;
    }
    blk->next = nullptr;
    return (char*)blk + sizeof(Element);
}

int MemoryAllocator::tryToJoin(Element *cur) {
    if (cur == nullptr)
        return 0;
    if (cur->next != nullptr && (char*)cur + cur->size * MEM_BLOCK_SIZE == (char*)(cur->next)){
        cur->size += cur->next->size;
        cur->next = cur->next->next;
        return 1;
    }
    else
        return 0;
}

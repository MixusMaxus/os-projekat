//
// Created by os on 1/13/24.
//

#include "../h/syscall_c.h"

inline void ecall(uint64 operation){
    __asm__ volatile ("mv a0, %0" : : "r" (operation)); //u registar a0 upisujemo kod sistemskog poziva
    __asm__ volatile ("ecall");
}

inline uint64 retVal(){
    uint64 volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r" (ret)); //u registar a0 sistemski poziv upisuje povratnu vrednost
    return ret;
}

void *mem_alloc(size_t size) {
    uint64 operation = 0x01;
    size_t sz = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    __asm__ volatile ("mv a1, %0" : : "r" (sz));
    ecall(operation);
    return (void*) retVal();
}

int mem_free(void * adr) {
    uint64 operation = 0x02;
    __asm__ volatile ("mv a1, %0" : : "r" (adr));
    ecall(operation);
    if (retVal() == 0)
            return 0;
    else
        return -1;
}

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg) {
    uint64 operation = 0x11;
    void *stack = mem_alloc(DEFAULT_STACK_SIZE);
    __asm__ volatile ("mv a4, %0" : : "r" (stack));
    __asm__ volatile ("mv a3, %0" : : "r" (arg));
    __asm__ volatile ("mv a2, %0" : : "r" (start_routine));
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    ecall(operation);
    if (retVal() == 0)
        return 0;
    else
        return -1;
}

int thread_exit() {
    uint64 operation = 0x12;
    ecall(operation);
    if (retVal() == 0)
        return 0;
    else
        return -1;
}

void thread_dispatch() {
    uint64 operation = 0x13;
    ecall(operation);
}

void thread_join(thread_t handle) {
    uint64 operation = 0x14;
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    ecall(operation);
}

int sem_open(sem_t *handle, unsigned int init) {
    uint64 operation = 0x21;
    __asm__ volatile ("mv a2, %0" : : "r" (init));
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    ecall(operation);
    if (retVal() == 0)
        return 0;
    else
        return -1;
}

int sem_close(sem_t handle) {
    uint64 operation = 0x22;
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    ecall(operation);
    if (retVal() == 0)
        return 0;
    else
        return -1;
}

int sem_wait(sem_t id) {
    uint64 operation = 0x23;
    __asm__ volatile ("mv a1, %0" : : "r" (id));
    ecall(operation);
    if (retVal() == 0)
        return 0;
    else
        return -1;
}

int sem_signal(sem_t id) {
    uint64 operation = 0x24;
    __asm__ volatile ("mv a1, %0" : : "r" (id));
    ecall(operation);
    if (retVal() == 0)
        return 0;
    else
        return -1;
}

int time_sleep(time_t sleepTime) {
    uint64 operation = 0x31;
    __asm__ volatile ("mv a1, %0" : : "r" (sleepTime));
    ecall(operation);
    if (retVal() == 0)
        return 0;
    else
        return -1;
}

char getc() {
    uint64 operation = 0x41;
    ecall(operation);
    return (char)retVal();
}

void putc(char c) {
    uint64 operation = 0x41;
    __asm__ volatile ("mv a1, %0" : : "r" (c));
    ecall(operation);
}

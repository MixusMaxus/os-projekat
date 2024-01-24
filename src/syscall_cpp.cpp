//
// Created by os on 1/15/24.
//

#include "../h/syscall_cpp.h"

Thread::Thread(void (*body)(void *), void *arg) {
    return thread_create(&myHandle, body, arg);
}

Thread::~Thread() {
    delete myHandle;
}

int Thread::start() {
    return 0;
}

void Thread::join() {
    thread_join(myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t sleep_time) {
    return time_sleep(sleep_time);
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}

void *operator new(size_t size) {
    return mem_alloc(size);
}

void operator delete(void * ptr) {
    mem_free(ptr);
}

//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"

TCB *TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThread(Body body)
{
    return new TCB(body, TIME_SLICE);
}

void TCB::yield()
{
    thread_dispatch();
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished()) {
        Scheduler::put(old);
    }
    running = Scheduler::get();

    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body();
    running->setFinished(true);
    TCB::yield();
}

TCB *TCB::createTCB(TCB::Body body, void *arg, uint64 *stack) {
    TCB *ret = (TCB*) MemoryAllocator::mem_alloc((sizeof(TCB) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    ret->body = body;
    ret->arg = arg;
    if (body != nullptr)
        ret->stack = stack;
    else
        ret->stack = nullptr;

    ret->context = {(uint64) &threadWrapper,stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0};
    ret->finished = false;
    ret->blocked = false;

    return ret;
}

TCB *TCB::createThread(TCB::Body body, void *arg, uint64 *stack) {
    TCB* ret = TCB::createTCB(body, arg, stack);
    if (ret->body != nullptr){
        Scheduler::staviNit(ret);
    }
    return ret;
}

TCB *TCB::initializeThread(TCB::Body body, void *arg, uint64 *stack) {
    return TCB::createTCB(body, arg, stack);
}

void TCB::block() {
    running->blocked = true;
    dispatch();
}

void TCB::unblock() {
    this->blocked = false;
    Scheduler::staviNit(this);
}


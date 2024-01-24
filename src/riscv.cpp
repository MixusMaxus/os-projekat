//
// Created by marko on 20.4.22..
//

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"

void Riscv::popSppSpie()
{
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}


inline static void Riscv::unexpectedTrap()
{
    uint64 scause = r_scause();
    uint64 stval = r_stval();
    uint64 stvec = r_stvec();
    uint64 sepc = r_sepc();

    printString("scause: ");
    printInt(scause);
    printString("\n");

    printString("stval: ");
    printInt(stval);
    printString("\n");

    printString("stvec: ");
    printInt(stvec);
    printString("\n");

    printString("sepc: ");
    printInt(sepc);
    printString("\n");
}

void Riscv::handleSupervisorTrap()
{
    uint64 scause = r_scause();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();
        /*TCB::timeSliceCounter = 0;
        TCB::dispatch();*/
        /*procitaj a0 registar, i ima jedan ogroman switch koji ce za sve operacije da odardi sa sistemskim klasma sta treba*/
        uint64 operation_code;
        uint64 a1;
        uint64 a2;
        uint64 a3;
        uint64 a4;
        __asm__ volatile ("mv %0, a0" : "=r" (operation_code));
        __asm__ volatile ("mv %0, a1" : "=r" (a1));
        __asm__ volatile ("mv %0, a2" : "=r" (a2));
        __asm__ volatile ("mv %0, a3" : "=r" (a3));
        __asm__ volatile ("mv %0, a4" : "=r" (a4));
        using Body = void (*)(void *);

        switch (operation_code) {
            case 0x01:      /*mem alloc*/
               /*uint64 numBlocks;
               __asm__ volatile ("mv %0, a1" : "=r" (numBlocks));*/
               void *ret = MemoryAllocator::mem_alloc(a1);
               __asm__ volatile ("mv a0, %0" : : "r" (ret));
               break;

            case 0x02:      /*mem_free*/
                int ret = MemoryAllocator::mem_free((void *)a1);
                __asm__ volatile ("mv a0, %0" : : "r" (ret));
                break;

            case 0x11:      /*thread create*/
                (TCB*) nit = (TCB*)a1;
                nit = TCB::createThread((Body)a2, (void*)a3, (uint64*)a4);
                uint64 ret = 0;
                if (nit == nullptr)
                    ret = -1;
                __asm__ volatile ("mv a0, %0" : : "r" (ret));
                break;

            case 0x12:      /*thread exit*/
                TCB::running->setFinished(true);
                TCB::dispatch();
                break;

            case 0x13:      /*thread_dispatch*/
                TCB::dispatch();
                break;

            case 0x14:      /*thread_join*/
                break;

            case 0x21:      /*sem_open*/
                Semaphor** handle = (Semaphor**)a1;
                Semaphor::sem_open(handle, (int)a2);
                int ret = 0;
                if (handle == nullptr)
                    ret = -1;
                __asm__ volatile ("mv a0, %0" : : "r" (ret));
                break;

            case 0x22:      /*sem_close*/
                Semaphor* s = (Semaphor*)(a1);
                int ret = s->sem_close();
                __asm__ volatile ("mv a0, %0" : : "r" (ret));
                break;

            case 0x23:      /*sem_wait*/
                Semaphor* s = (Semaphor*)a1;
                int ret = s->sem_wait();
                __asm__ volatile ("mv a0, %0" : : "r" (ret));
                break;


            case 0x24:      /*sem_signal*/
                Semaphor* s = (Semaphor*)a1;
                int ret = s->sem_signal();
                __asm__ volatile ("mv a0, %0" : : "r" (ret));
                break;

            case 0x41:      /*getc*/
                char ret = __getc();
                __asm__ volatile ("mv a0, %0" : : "r" (ret));
                break;

            case 0x42:      /*putc*/
                char c = (char)a1;
                __putc(ch);
                break;
        }
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
        /*TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }*/
    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
    else
    {
        // unexpected trap cause
        unexpectedTrap();
    }
}
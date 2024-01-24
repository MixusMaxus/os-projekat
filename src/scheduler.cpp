//
// Created by os on 1/16/24.
//

#include "../h/scheduler.h"

/*pogledaj da li moras da oduzimas memoriju*/

TCB *Scheduler::dohvatiNit() {
    if (prvi == nullptr)
        return nullptr;
    Element* ret = prvi;
    prvi = prvi->sledeci;
    if (prvi == nullptr)
        poslednji = nullptr;

    return ret->data;
}

void Scheduler::staviNit(TCB *tcb) {
    Element* novi = new Element();/*pogledaj ovu liniju*/
    novi->data = tcb;
    novi->sledeci = nullptr;
    if (prvi == nullptr)
        prvi = novi;
    else
        poslednji->sledeci = novi;

    poslednji = novi;
}

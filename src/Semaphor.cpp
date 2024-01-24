//
// Created by os on 1/17/24.
//

#include "../h/Semaphor.h"

Semaphor *Semaphor::sem_create(int init) {
    Semaphor* ret = new Semaphor(init);
    return ret;
}

int Semaphor::sem_open(Semaphor **handle, int val) {
    *handle = sem_create(val);
    if (*handle == nullptr)
        return -1;
    else
        return 0;
}

int Semaphor::sem_close() {
    if (upaljen == false)
        return -1;
    upaljen = false;
    TCB* nit = dohvatiPrvuBlokiranu();
    while (nit != nullptr){
        nit.unblock();
        nit = dohvatiPrvuBlokiranu();
    }
    return 0;
}

int Semaphor::sem_signal() {
    if (upaljen == false)
        return -1;

    if (prvi == nullptr)
        val++;
    else{
        TCB* nit = dohvatiPrvuBlokiranu();
        nit->unblock();
    }
    return 0;
}

int Semaphor::sem_wait() {
    if (upaljen == false)
        return -1;
    if (>val > 0)
        val--;
    else{
        this->staviNit(TCB::running);
        TCB::block();
    }
    return 0;
}

TCB *Semaphor::dohvatiPrvuBlokiranu() {
    if (prvi == nullptr)
        return nullptr;
    TCB* ret = prvi->nit;
    BlokiraniElement* stari = prvi;
    prvi = prvi->sledeci;
    if (prvi == nullptr)
        poslednji = nullptr;
    /*delete old*/
    return ret;
}

void Semaphor::staviNit(TCB *nit) {
    BlokiraniElement* novi = new BlokiraniElement();
    novi->nit = nit;
    novi->sledeci = nullptr;
    if (prvi == nullptr)
        prvi = novi;
    else
        poslednji->sledeci = novi;
    poslednji = novi;
}

Semaphor::Semaphor(int val) {
    this->val = val;
    this->upaljen = true;
    this->prvi = nullptr;
    this->poslednji = nullptr;
}

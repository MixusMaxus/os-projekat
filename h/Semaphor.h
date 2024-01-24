//
// Created by os on 1/17/24.
//

#ifndef PROJECT_BASE_SEMAPHOR_H
#define PROJECT_BASE_SEMAPHOR_H

#include "../h/tcb.hpp"

struct BlokiraniElement{
    TCB* nit;
    BlokiraniElement* sledeci;
};

class Semaphor {
private:
    int val;
    bool upaljen;
    BlokiraniElement* prvi;
    BlokiraniElement* poslednji;
    static Semaphor* sem_create(int);
    TCB* dohvatiPrvuBlokiranu();
    void staviNit(TCB*);
public:
    Semaphor(int);
    int sem_open(Semaphor**, int);
    int sem_close();
    int sem_signal();
    int sem_wait();

};


#endif //PROJECT_BASE_SEMAPHOR_H

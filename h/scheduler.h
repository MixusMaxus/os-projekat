//
// Created by os on 1/16/24.
//

#ifndef PROJECT_BASE_SCHEDULER_H
#define PROJECT_BASE_SCHEDULER_H

#include "../h/tcb.hpp"


class TCB;

struct Element{
    TCB* data;
    Element* sledeci;
};

class Scheduler {
private:
     static Element *prvi, *poslednji;
     Scheduler(){};
public:
    static TCB* dohvatiNit();
    static void staviNit(TCB *);

};


#endif //PROJECT_BASE_SCHEDULER_H

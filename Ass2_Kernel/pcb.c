#include <stdio.h>
#include <stdlib.h>
#include "pcb.h"

PCB* makePCB(FILE *p, int i){
    PCB *pcb = malloc(sizeof(PCB));
    pcb->pc = p;
    pcb->ramIndex = i;
    return pcb;
}

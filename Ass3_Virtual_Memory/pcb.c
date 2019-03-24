#include <stdio.h>
#include <stdlib.h>
#include "pcb.h"

PCB* makePCB(FILE *p){
    PCB *pcb = malloc(sizeof(PCB));
    pcb->pc = p;
    pcb->PC_page = 0;
    pcb->PC_offset = 0;
    for (int i=0; i<10; i++){ pcb->pageTable[i] = -1; } //reset to -1
    return pcb;
}

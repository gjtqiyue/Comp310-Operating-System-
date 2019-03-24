#include "pcb.h"

extern FILE *ram[10];

void addToReady(PCB *pcb);
int updateVictimPCB(int victimFrame);
int scheduler();
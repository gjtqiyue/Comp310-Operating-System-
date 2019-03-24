#ifndef CPU_H
#define CPU_H

FILE * run(int quanta);
int isCPUFree();
void swapIn(FILE *fp, int offset);
void swapOut();
int getOffset();
void resetOffset();

#endif
#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "shell.h"

struct CPU {
    FILE *IP;
    char IR[1000];  // stores the instruction that will be sent to the iterpreter()
    int quanta; // quanta is 2 lines of code for each program
    int offset;
} cpu;

FILE * run(int quanta){
    cpu.quanta = quanta;
    
    while (cpu.quanta > 0){
        
        // read lines from the current IP
        if (fgets(cpu.IR, 999, cpu.IP) != NULL){
            //printf("%s", cpu.IR);
            // pass to interpreter
            //printf("run command %d %s", cpu.offset, cpu.IR);
            parse(cpu.IR);
        }
        else {
            return NULL;
        }
        
        cpu.offset++;
        
        if (cpu.offset == 4){   //page fault happen, interrupt
            return NULL;
        }
        cpu.quanta -= 1;
    }
    //printf("one quanta\n");
    return cpu.IP;
}

void swapIn(FILE *fp, int offset){
    cpu.IP = fp;
    cpu.offset = offset;
}

void swapOut(){
    cpu.IP = NULL;
}

int isCPUFree(){
    if (cpu.IP == NULL){
        return 1;
    }
    else {
        return 0;
    }
}

int getOffset(){
    return cpu.offset;
}

void resetOffset(){
    cpu.offset = 0;
}
#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include "cpu.h"
#include "pcb.h"
#include "ram.h"

typedef struct PCBNode {
    PCB *pcb;
    struct PCBNode *next;
} pcb_node;

// ready queue
pcb_node *head = NULL, *tail = NULL;

int main() {
    printf("Kernel 1.0 loaded! \n");

    // call shell.displayUI()
    displayUI();
}

void addToReady(PCB *pcb){
    pcb_node *node = malloc(sizeof(pcb_node));
    node->pcb = pcb;
    node->next = NULL;
    if (tail == NULL && head == NULL) {
        tail = node;
        head = tail;
    }
    else{
        tail->next = node;
        tail = node;
    }
}

void removeFromReady(){
    if (head->next != NULL){
        pcb_node *temp = head;
        head = head->next;
        free(temp);
    }
    else{
        // the queue is clear
        free(head);
        head = NULL;
        tail = NULL;
    }
}

PCB * frontFromReady(){
    return head->pcb;
}

void finishExecution(PCB *p){
    // delete from RAM
    deleteFromRam(p->ramIndex);
    // free the memory
    free(p);
    // remove from queue
    removeFromReady();
}

int scheduler(){
    // execute until the queue is empty
    while (head != NULL){
        // check to see if CPU is available
        if (isCPUFree() == 0){
            // cpu is busy
        }
        else{
            // it means nothing is assigned to CPU or the quanta is finished
            // copies the PCB PC pointer to CPU IP pointer
            PCB *pcb = frontFromReady();
            swapIn(pcb->pc);
            // calls run(quanta)
            FILE *npc = run(2);
            if (npc == NULL){
                // the program finishes
                finishExecution(pcb);
            }
            else{
                // not done yet
                // push back to queue
                pcb->pc = npc;
                addToReady(pcb);
                removeFromReady();
                //printf("push to back \n");
            }
            swapOut();
        }
    }
    return 0;  
}

int myInit(FILE *p){
    // add to ram
    int index = addToRam(p);
    if (index == -1) return 7;  // RAM out of space
    // make pcb
    PCB *pcb = makePCB(p, index);
    // add to ready queue
    addToReady(pcb);

    return 0;
}
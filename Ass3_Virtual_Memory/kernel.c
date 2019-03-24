#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "shell.h"
#include "cpu.h"
#include "pcb.h"
#include "memorymanager.h"

typedef struct PCBNode {
    PCB *pcb;
    struct PCBNode *next;
} pcb_node;

// ready queue
pcb_node *head = NULL, *tail = NULL;

// RAM presentaion
// NULL pointer means RAM is not occupied
// A limit of 10 programs
FILE *ram[10] = {NULL};

void boot(){
    // assumes RAM is global array
    // each RAM cell is a frame
    // initilze every cell to NULL

    // delete Backing storage
    // create a new one
    system("rm -r BackingStore");
    system("mkdir BackingStore");
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
    //printf("terminate\n");
    for (int i=0; i<10; i++){
        int frame = p->pageTable[i];
        if (frame != -1){
            fclose(ram[frame]);
            ram[frame] = NULL;
        }
    }
    // free the memory
    free(p);
    // remove from queue
    removeFromReady();
}

void pageFaultOperation(PCB *pcb){
    pcb->PC_page++;
    //printf("current page %d, max pages %d\n", pcb->PC_page, pcb->pages_max);
    if (pcb->PC_page >= pcb->pages_max){
        //printf("Page fault offset reach 4, program terminates;\n");
        finishExecution(pcb);
    }
    else{
        //check page table for the next page
        int frameNum = pcb->pageTable[pcb->PC_page];
        if (frameNum >= 0 && frameNum < 10){
            //printf("found page in ram, load%d\n", frameNum);
            // valid frame number
            pcb->pc = ram[frameNum];
            pcb->PC_offset = 0;
        }
        else{   //invalid frame number
            // find page
            //printf("no page found in ram\n");
            int validFrame, victimFrame = -1;

            FILE *fp = fdopen (dup (fileno (pcb->pc)), "r");
            rewind(fp);
            FILE *page = findPage(pcb->PC_page, fp);
            fclose(fp);

            validFrame = findFrame(page);
            if (validFrame == -1) victimFrame = findVictim(pcb);

            updateFrame(validFrame, victimFrame, page);
            updatePageTable(pcb, pcb->PC_page, validFrame, victimFrame);

            pcb->pc = page;
            pcb->PC_offset = 0;
        }
        // task switch
        addToReady(pcb);
        removeFromReady();
    }
}

// Hleper function: find the victim pcb
int updateVictimPCB(int victimFrame){
    pcb_node* pt = head;
    while (pt != NULL){
        for (int i=0; i<pt->pcb->pages_max; i++){
            //printf("pcb table %d, %d", i, pt->pcb->pageTable[i]);
            if (pt->pcb->pageTable[i] == victimFrame){
                pt->pcb->pageTable[i] = -1;
                //printf("set pcb slot %d to -1\n", i);
                return 1;
            }
        }
        pt = pt->next;
    }
    return 0;
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
            swapIn(pcb->pc, pcb->PC_offset);
            // calls run(quanta)
            FILE *npc = run(2);
            if (npc == NULL && getOffset() == 4){
                // call the page fault operation
                //printf("page fault\n");
                resetOffset();
                pageFaultOperation(pcb);
            }
            else if (npc == NULL){
                // the program finishes
                finishExecution(pcb);
            }
            else{
                // not done yet
                // push back to queue
                pcb->pc = npc;
                pcb->PC_offset = getOffset();
                addToReady(pcb);
                removeFromReady();
                //printf("push to back \n");
            }
            swapOut();
        }
    }
    // delete backing store
    system("rm -r BackingStore/*.txt");
    return 0;  
}

// int myInit(FILE *p){
//     // add to ram
//     int index = addToRam(p);
//     if (index == -1) return 7;  // RAM out of space
//     // make pcb
//     PCB *pcb = makePCB(p, index);
//     // add to ready queue
//     addToReady(pcb);

//     return 0;
// }

int main() {
    boot();
    printf("Kernel 1.0 loaded! \n");  
    displayUI();    // call shell.displayUI()
}
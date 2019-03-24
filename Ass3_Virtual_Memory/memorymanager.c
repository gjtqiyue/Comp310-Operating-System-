#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "kernel.h"

int filenum = 0;

int countTotalPages(FILE *f){
    double line = 0;
    int page = 0;
    char ch;
    while ((ch = getc(f)) != EOF){
        if (ch == '\n')
            line++;
    }
    page = ceil((line / 4));
    rewind(f);
    //printf("check original file: %c\n", fgetc(f));
    return page;
}

FILE * findPage(int pageNumber, FILE *f){
    FILE *fp = fdopen (dup (fileno (f)), "r");
    char str[99];
    for (int i=0; i<4*pageNumber; i++){
        fgets(str, 99, fp);
    }

    // if (pageNumber >= 2){
    // printf("after rewinding text\n");
    //         while (fgets (line, 99, fp)!=NULL){
    //             printf("%s", line);
    //         }
    // }
    return fp;
}

int findFrame(FILE *page){
    for (int i=0; i<10; i++){
        if (ram[i] == NULL){
            ram[i] = page;
            //printf("find frame %d\n", i);
            return i;
        }
    }
    return -1;
}

int findVictim(PCB *p){
    //printf("need to find a victim\n");
    int r = rand() % 10;
    for (int i=0; i<10; i++){
        if (p->pageTable[i] == r){
            break;
        }
        else if (i == 9){
            //printf("find victim %d\n", r);
            return r;   // this slot doesn't belong to this pcb
        }
    }
    // if this one is in use by this pcb
    for (int i=1; i<10; i++){
        r = (r+i)%10;
        for (int i=0; i<10; i++){
            if (p->pageTable[i] == r){
                break;
            }
            else if (i == 9){
                //printf("find victim %d\n", r);
                return r;   // this slot doesn't belong to this pcb
            }
        }
    }
    return -1;
}

int updateFrame(int frameNumber, int victimFrame, FILE *page){
    int frame = frameNumber == -1 ? victimFrame : frameNumber;
    ram[frame] = page;
    //printf("update frame\n");
    return 0;
}

int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame){
    int frame;
    int res = 1;
    if (frameNumber == -1){
        frame = victimFrame;
        res = updateVictimPCB(victimFrame);
    }
    else{
        frame = frameNumber;
    }
    p->pageTable[pageNumber] = frame;
    //printf("update table %d, %d, %d\n", pageNumber, frameNumber, victimFrame);
    return res;
}

int launcher(FILE *p){
    int err;
    char ch;
    char name[50];
    int victimNum = -1, frameNum;
    sprintf(name, "BackingStore/p%d.txt", filenum);
    
    // copy file into the backing store
    FILE *target = fopen(name, "w");
    while ((ch = fgetc(p)) != EOF)
      fputc(ch, target);
    // close the file pointer
    fclose(p);
    fclose(target);
    // open the file in the backing store
    FILE *file = fopen(name, "r");
    countTotalPages(file);
    // create a PCB
    PCB *pcb = makePCB(file);
    // count total pages
    int pagenum = countTotalPages(file);
    if (pagenum <= 0) return 9;
    
    pcb->pages_max = pagenum;   // update max page

    int limit = pagenum > 1 ? 2 : 1;
    for (int i=0; i<limit; i++){
        FILE *page = findPage(i, file);
        rewind(file);

        frameNum = findFrame(page);
        if (frameNum == -1) findVictim(pcb);

        err = updateFrame(frameNum, victimNum, page);
        err = updatePageTable(pcb, i, frameNum, victimNum);
        if (err == 0) return 8;
    }
    pcb->pc = ram[pcb->pageTable[pcb->PC_page]];
    // add to ready queue
    addToReady(pcb);
    fclose(file);

    filenum++;
    return 1;
}
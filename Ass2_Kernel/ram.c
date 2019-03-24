#include <stdio.h>
#include <stdlib.h>

// RAM presentaion
// NULL pointer means RAM is not occupied
// A limit of 10 programs
FILE *ram[10] = {NULL};

int addToRam(FILE *fp){
    int i;

    for (i=0; i<10; i++){
        if (ram[i] == NULL){
            ram[i] = fp;
            break;
        }
        else if (i == 9){
            // the RAM is currently full
           return -1;
        }
    }
    return i;
}

// same file pointer
void deleteFromRam(int index){
    // close the file
    fclose(ram[index]);
    // set to NULL
    ram[index] = NULL;
}
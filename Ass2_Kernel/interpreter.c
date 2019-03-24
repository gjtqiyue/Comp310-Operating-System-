#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "shellmemory.h"
#include "shell.h"
#include "kernel.h"

// commands
int help(){
    printf("Current commands: \n");
    printf("help:               Display all the commands \n");
    printf("quit:               Exits/terminate the shell with 'Bye!' \n");
    printf("set VAR STRING:     assign a value to shell memory \n");
    printf("print VAR:          prints the STRING assigned to VAR \n");
    printf("run SCRIPT.TXT:     executes the file SCRIPT.TXT \n");
    printf("exec p1 p2 p3:      executes concurrent programs $exec prog.txt prog2.txt \n");
    return 0;
}

int quit(){
    printf("Bye! \n");
    return 1;
}

int set(char *var, char *string){
    // make sure there are at least two valid parameters passed
    if  (var == NULL || string == NULL) return 6;
    if (strcmp(var, "") == 0 || strcmp(string, "") == 0) return 6;

    return memoryset(var, string);
}

int print (char *var){
    // check if valid
    if (var == NULL) return 6;
    if (strcmp(var, "") == 0) return 6;

    const char *value = memoryget(var);
    if (strcmp(value, "Not found") == 0){
        return 4;
    }
    else {
        printf("%s \n", value);
        return 0;
    }
}

static int run (char *fname){
    FILE *fp;
    char line[100];
    int errorcode = 0;
    int location = 0;

    fp = fopen(fname, "rt"); //open file

    if (fp == NULL){
        errorcode = 5;
    }
    else {
        while (fgets (line, 99, fp)!=NULL){
            errorcode = parse(line);

            if (errorcode != 0) return errorcode;   //return if an error occurs
        }
    }

    fclose(fp); //close file

    return errorcode;
}

int exec (char *fileNames[]){
    // file index should start from 1
    // check for same names
    FILE *fp1, *fp2, *fp3;
    FILE *fps[3] = {fp1, fp2, fp3};

    int i = 0;
    while (fileNames[i+1] != NULL){
        if (i==1 && strcmp(fileNames[i+1], fileNames[i]) == 0){
            printf("Error: Script %s already loaded\n", fileNames[i+1]);
        }
        else if (i==2 && (strcmp(fileNames[i+1], fileNames[i]) == 0 || strcmp(fileNames[i+1], fileNames[i-1]) == 0)){
            printf("Error: Script %s already loaded\n", fileNames[i+1]);
        }
        else{
            fps[i] = fopen(fileNames[i+1], "rt");

            if (fps[i] == NULL){
                printf("Error: Script %s not found\n", fileNames[i+1]);
            }
            else{
                // initialize the program
                myInit(fps[i]);
            }
        }
        i++;
        if (i >= 3){
            break;
        }
    }

    // call scheduler
    return scheduler();
}

int interpreter(char *words[]){
    //extract command
    if (strcmp(words[0], "") == 0){ return 0; }
    else if (strcmp(words[0], "help") == 0){ return help();}
    else if (strcmp(words[0], "quit") == 0){ return quit();}
    else if (strcmp(words[0], "set") == 0){ return set(words[1], words[2]);}
    else if (strcmp(words[0], "print") == 0){ return print(words[1]);}
    else if (strcmp(words[0], "run") == 0){ return run(words[1]);}
    else if (strcmp(words[0], "exec") == 0){ return exec(words);}
    else { return 2; }    // unknown command error code

    return -1;
}


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "shellmemory.h"
#include "shell.h"

// commands
int help(){
    printf("Current commands: \n");
    printf("help \n");
    printf("quit \n");
    printf("set \n");
    printf("print \n");
    printf("run  \n");
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

int run (char *fname){
    FILE *fp;
    char line[100];
    int errorcode = 0;
    int location = 0;

    fp = fopen(fname, "rt"); //open file

    if (fp == NULL){
        return 5;
    }
    else {
        while (fgets (line, 60, fp)!=NULL){
            errorcode = parse(line);

            if (errorcode != 0) return errorcode;   //return if an error occurs
        }
    }

    fclose(fp); //close file

    return errorcode;
}

int interpreter(char *words[]){
    //extract command
    if (strcmp(words[0], "") == 0){ return 0; }
    else if (strcmp(words[0], "help") == 0){ return help();}
    else if (strcmp(words[0], "quit") == 0){ return quit();}
    else if (strcmp(words[0], "set") == 0){ return set(words[1], words[2]);}
    else if (strcmp(words[0], "print") == 0){ return print(words[1]);}
    else if (strcmp(words[0], "run") == 0){ return run(words[1]);}
    else { return 2; }    // unknown command error code

    return -1;
}


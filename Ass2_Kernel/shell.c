#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"

int parse(char *input) {
    char word[100];
    char *words[1000] = {NULL};

    int j;
    int i = 0;
    int w = 0;
    
    if (strcmp(input, "") == 0)  return 0;  // if the input string is empty, return immediately

    //parse words   
    while (*(input+i) != '\0' && i < 1000 && w < 10){
        //trim the space before the command
        //&& *(input+i) != '\n'
        for (i = i; *(input+i) == ' ' && i < 1000; i++);

        for (j = 0, i = i; *(input+i) != '\0' && *(input+i) != '\n' &&  *(input+i) != ' ' && j < 100; i++, j++){
            word[j] = *(input+i);
        }

        word[j] = '\0';
        
        words[w] = strdup(word);

        w++;
    }

    return interpreter(words);
}

int displayUI() {
    char input[1000];
    int errorcode = 0;

    //welcome words
    printf("Welcome to the Lancer shell! \n");
    printf("Version 2.0 Updated February 2019 \n");
    
    int quit=0;
    while (quit != 1){
        //prompt the dollar sign
        printf("$ ");
        fgets(input, 999, stdin);

        // if the input is nothing, quit the shell
        if (input[0] == '\0') quit = 1;

        // if the input get modified or not empty, meaning fgets() reads something, then we continue to the parse stage
        // if user just type space or nextline, the input will be ignored
        else {
            //get rid of the nextline symbol
            int len = strlen(input);
            
            if (input[len-1] == '\n') input[len-1] = '\0';

            errorcode = parse(input);

            input[0] = '\0';    //set input string to \0

            if (errorcode == -1){ exit(99); }
            if (errorcode == 1){ quit = 1; }
            if (errorcode == 2){ printf("Unknown command \n"); }
            if (errorcode == 3){ printf("Insufficiant memory \n"); }
            if (errorcode == 4){ printf("Undefined variable \n"); }
            if (errorcode == 5){ printf("Error: Script not found \n"); }
            if (errorcode == 6){ printf("Insufficient parameters \n"); }
            if (errorcode == 7){ printf("Ram our of space \n"); }
        }
    }
    return 0;
}


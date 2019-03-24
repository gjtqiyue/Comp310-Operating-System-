#include <stdio.h>
#include <string.h>

char *memory[1000][2];  // var and value for two slots
int var_num = 0;

int memoryset(char *var, char *string){
    int existed = 0;

    // check if such var exist
    for (int i=0; i < var_num; i++){
        // check if the var has existed
        if (strcmp(memory[i][0], var) == 0){
            memory[i][1] = strdup(string);
            existed = 1;
            break;
        }
    }

    if (existed == 0 && var_num >= 1000){
        return 3;   //not enough memory
    }

    if (existed == 0){  
        // if not then create new variable with the name
        memory[var_num][0] = strdup(var);
        memory[var_num][1] = strdup(string);
        var_num++;
        return 0;
    }

    return 0;
}

char * memoryget(char *var){
    for (int i=0; i < var_num; i++){
        //if we find a variable with the same name
        if (strcmp(memory[i][0], var) == 0){
            return memory[i][1];
        }
    }

    return "Not found";
}
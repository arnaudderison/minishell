#include <stdio.h>
#include <stdlib.h>
#include "../include/main.h"

#define MAX_BUFFER 1024

//int readCommand(char *inputCommand);
int main (){

    char *inputCommand = NULL;
    inputCommand = malloc(sizeof(char) * MAX_BUFFER);


    printf("$>");
    readCommand(inputCommand);

    printf("La commande est :%s\n", inputCommand);


    return 0;
}



int readCommand(char *inputCommand){
    char currentChar = 0;
    int count = 0;

    while(currentChar != '\n' && count < MAX_BUFFER){
        currentChar = getchar();
        inputCommand[count++] = currentChar;
    };
    return 0;
}
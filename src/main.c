#include <stdio.h>
#include <stdlib.h>

#include "../include/minishellfunct.h"

struct Command command;

void commandPrompt(){
    printf("%s%sshell$%s", BOLD, COLOR_BLU, COLOR_NRM);
}

int main (){
    char inputCommand[MAX_BUFFER];
    
    while(1){
        commandPrompt();
        readCommand(inputCommand, MAX_BUFFER);
        parseCommand(inputCommand, &command);


        processCommand(&command);
        

        /*--------FOR DEBUG-------*/
        //printf("la commande est :%d", command.argc);
        /*
        for(int i = 0; i<command.argc; i++){
            printf("%s", command.argv[i]);
        }*/
        //emptyCommandArgc(&command);
        
        
    }

    return 0;
}




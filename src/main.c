#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/main.h"


struct Command command;
int main (){
    char *inputCommand = NULL;
    inputCommand = malloc(sizeof(char) * MAX_BUFFER);
    
    while(1){
        printf("$a.derison/minishell>");
        readCommand(inputCommand);
        printf("La commande est :%s", inputCommand);

        parseCommand(inputCommand, &command);

        if(strcmp(inputCommand, "exit") == 10){
            printf("Bye\n");
            break;
        }
    }
    free(inputCommand);
    
    return 0;
}

int parseCommand(char *inputCommand, struct Command *command){
    int i = 0;
    char *token;
    token = strtok(inputCommand, " ");

    while(token != NULL){
        command->argv[i] = token; 
        token =  strtok(NULL, " ");
        i++;
    }

    command->argc = i;
    command->commandName = command->argv[0];

    /*-----------FOR DEBUG-----------*/

    printf("%s : est le nom de la commande\n", command->commandName);
    
    for(int j = 0; j<i; j++){
        printf("command->argv[%i] %s\n",j, command->argv[j]);
    }
    
}

int readCommand(char *inputCommand){
    char currentChar = 0;
    int count = 0;

    while((currentChar != '\n') && (count < MAX_BUFFER)){
        currentChar = getchar();
        inputCommand[count++] = currentChar;
    };
    inputCommand[count] = '\0'; //pour etre certain d'être a la fin de la commande actuel
    return 0;
}

int internalCommand(){
    //if(strcmp("cd", command.commandName))

    return 0;
}
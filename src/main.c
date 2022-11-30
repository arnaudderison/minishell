#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../include/main.h"


struct Command command;
int main (){
    char inputCommand[80];
    
    while(1){
        printf("%s%sa.derison~minishell%s$", BOLD,COLOR_BLU, COLOR_NRM);
        readCommand(inputCommand);
        //printf("La commande est :%s", inputCommand);

        parseCommand(inputCommand, &command);

        if(isInternalCommand() == 0){
            executeNormalCommande();
        }

        if((strcmp("exit", inputCommand) == 0) || (strcmp("exit\n", inputCommand) == 0)){
            printf("Bye\n");
            break;
        }
    }
    //free(inputCommand);
    
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
    /*
    printf("%s : est le nom de la commande\n", command->commandName);
    
    for(int j = 0; j<i; j++){
        printf("command->argv[%i] %s\n",j, command->argv[j]);
    }
    */
    
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

int isInternalCommand(){
    if((strcmp("cd", command.commandName) == 0) || (strcmp("cd\n", command.commandName) == 0)){
        changeDir();
        return 1;
    }
    return 0;
}

int changeDir(){
    if(command.argv[1] == NULL){
        chdir(getenv("HOME"));
        return 1;
    }else{
        if(chdir(command.argv[1]) == -1){
            printf("%sMinishell Error:%s    %s%sNo such directory%s\n", COLOR_RED, COLOR_NRM, command.argv[1], COLOR_MAG, COLOR_NRM);
            return 1;
        }
    }
    return 0;
}

int executeNormalCommande(){
    int pid, status;

    pid = fork();
    if(pid < 0){
        fprintf(stderr, "Fork failed:\n");
        return 0;
    }
    else if (pid == 0){
        /*CHILD*/
        execvp(command.commandName, command.argv); // replace this process with commandName

        //not execute if not err
        printf("Child process erreur...\n");
        return 0;
    }
    else if(pid > 0){
        /*PARENT*/
        if(waitpid(pid, &status, 0) == -1){
            perror("waitpid Error...");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
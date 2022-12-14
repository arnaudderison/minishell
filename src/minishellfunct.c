#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/minishellfunct.h"


int readCommand(char *inputCommand, int maxBuf){
    fgets(inputCommand, maxBuf, stdin);

    return 0;
}

int parseCommand(char *inputCommand, struct Command *command){
    char *token;
    int count = 0;

    token = strtok(inputCommand, " \r\n");

    while(token != NULL){
        command->argv[count] = token;
        token = strtok(NULL, " \r\n");
        count++;
    }
    command->argv[count++] = '\0';
    command->commandName = command->argv[0];
    command->argc = count;

    /*--------FOR DEBUG--------*/
    //printf("LE command est : %s \n", command->commandName);
    /*for(int i = 0; i<count; i++){
        printf("command->argv[%i] = %s\n", i, command->argv[i]);
    }*/

    return 0;
}

void emptyCommandArgc(struct Command *command){
    for(int i = 0; i<command->argc; i++){
        command->argv[i] = '\0';
    }
}

int processCommand(struct Command *command){
    //process
    int j;
    for(j = 0; j<command->argc-1; j++){

        if(strcmp(command->argv[0], "cd") == 0){
            changeDir(command);
            return 1;
        }

        if(strcmp(command->argv[j], "|") == 0){
            processPipeCommand(command, j);
            return 1;
        }
        if(strcmp(command->argv[j], "&&") == 0){
            processAndCommand(command, j);
            return 1;
        }
    }
    startCommand(command);
    return 0;
}

int changeDir(struct Command *command){
    if(command->argv[1] == NULL){
        chdir(getenv("HOME"));
        return 1;
    }else{
        if(chdir(command->argv[1]) == -1){
            printf("%sMinishell Error:%s    %s%sNo such directory%s\n", COLOR_RED, COLOR_NRM, command->argv[1], COLOR_MAG, COLOR_NRM);
            return 1;
        }
    }
    return 0;
}

void startCommand(struct Command *command){
    int pid, status;

    switch (pid = fork())
    {
        case -1:
            perror("Fork Error");
            exit(EXIT_FAILURE);
            break;
        case 0:
            /*Child*/
            execvp(command->commandName, command->argv);
            perror("Chils process Error");
            break;
        
        default:
            /*Parent*/
            if(waitpid(pid, &status, 0)== -1){
                perror("waitpid Error");
                exit(EXIT_FAILURE);
            }
            break;
        }
}

void processPipeCommand(struct Command *command, int i){
    char *commandA[5], *commandB[5];
    int j,k,l;
    int pid, status;
    l = 0;

    for(j = 0; j<i; j++){
        if(j<=5){
            commandA[j] = command->argv[j];
        }
    }
    commandA[j++] = '\0';
    i++;
    for(k = i; k<command->argc; k++){
        if(l<=5){
            commandB[l] = command->argv[k];
            l++;
        }
    }

    switch (pid = fork())
    {
        case -1:
            perror("Fork Error");
            exit(EXIT_FAILURE);
            break;
        case 0:
            /*Child*/
            startPipeCommand(commandA, commandB);
            perror("Chils process Error");
            break;
        
        default:
            /*Parent*/
            if(waitpid(pid, &status, 0)== -1){
                perror("waitpid Error");
                exit(EXIT_FAILURE);
            }
            break;
        }
    

    /*--------FOR DEBUG--------*/
    /*
    for(int test = 0; test<5; test++){
        printf("command a = %s\n", commandA[test]);
        printf("command b = %s\n", commandB[test]);
    }
    */
}

void startPipeCommand(char *commandA[], char *commandB[]){
    int pid, status, fd[2];

    if(pipe(fd)){
        perror("Pipe Error");
    }
    switch (pid = fork())
    {
    case -1:
        /* code */
        perror("Fork Error");
        exit(EXIT_FAILURE);
        break;
    case 0:
        /*Child*/
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        execvp(commandA[0], commandA);
        perror("Child Process Error");
        exit(EXIT_FAILURE);
        break;
    
    default:
        /*Parent*/
        waitpid(pid, &status, 0);
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        execvp(commandB[0], commandB);
        perror("Parent process Error");
        exit(EXIT_FAILURE);
        break;
    }
    
}

void processAndCommand(struct Command *command, int i){
    char *commandA[5], *commandB[5];
    int j,k,l;
    int pid, status;
    l = 0;

    for(j = 0; j<i; j++){
        if(j<=5){
            commandA[j] = command->argv[j];
        }
    }
    commandA[j++] = '\0';
    i++;
    for(k = i; k<command->argc; k++){
        if(l<=5){
            commandB[l] = command->argv[k];
            l++;
        }
    }

    switch (pid = fork())
    {
        case -1:
            perror("Fork Error");
            exit(EXIT_FAILURE);
            break;
        case 0:
            /*Child*/
            startAndCommand(commandA, commandB);
            perror("Chils process Error");
            break;
        
        default:
            /*Parent*/
            if(waitpid(pid, &status, 0)== -1){
                perror("waitpid Error");
                exit(EXIT_FAILURE);
            }
            break;
        }
    

    /*--------FOR DEBUG--------*/
    /*
    for(int test = 0; test<5; test++){
        printf("command a = %s\n", commandA[test]);
        printf("command b = %s\n", commandB[test]);
    }
    */
}
void startAndCommand(char *commandA[], char *commandB[]){
    int pid, status, fd[2];

    if(pipe(fd)){
        perror("Pipe Error");
    }
    switch (pid = fork())
    {
    case -1:
        /* code */
        perror("Fork Error");
        exit(EXIT_FAILURE);
        break;
    case 0:
        /*Child*/
        execvp(commandA[0], commandA);
        perror("Child Process Error");
        exit(EXIT_FAILURE);
        break;
    
    default:
        /*Parent*/
        execvp(commandB[0], commandB);
        perror("Parent process Error");
        exit(EXIT_FAILURE);
        break;
    }
    
}
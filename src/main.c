#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../include/main.h"


struct Command command;
int main (){
    char inputCommand[MAX_BUFFER];
    int nbI = 0;
    
    while(1){
        printf("%s%sa.derison~minishell%s$", BOLD,COLOR_BLU, COLOR_NRM);
        readCommand(inputCommand);
        //printf("La commande est :%s", inputCommand);

        parseCommand(inputCommand, &command);

        if(isInternalCommand() == 0){
            nbI = isPipeCommand();
            if(nbI > 0){
                printf("PIPE COMMAND\n");
                executePipeCommand(nbI);
            }else{
                executeNormalCommande();
            }
        }
        

        if((strcmp("exit", inputCommand) == 0) || (strcmp("exit\n", inputCommand) == 0)){
            printf("Bye\n");
            break;
        }

        clearArg();
    }
    
    return 0;
}

int parseCommand(char *inputCommand, struct Command *command){
    int i = 0;
    char *token;
    
    token = strtok(inputCommand, " \r\n");

    while(token != 0){
        command->argv[i] = token; 
        token =  strtok(0, " \r\n");
        i++;
    }
    
    //command->argv[strcspn(token, "\n")] = 0;
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
    inputCommand[count++] = '\0'; //pour etre certain d'être a la fin de la commande actuel
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
        //printf("Child process erreur...\n");
        perror(command.commandName);
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

void clearArg(){
    for(int i = 0; i<command.argc; i++){
        command.argv[i] = 0;
    }
}

int isPipeCommand(){
    int i = 0;
    for(i; i<command.argc; i++){
        if(strcmp("|", command.argv[i]) == 0){
            return i;
        }
    }
    return 0;
}

int executePipeCommand(int i){
    char *NamecommandA, *NamecommandB;
    char *argvA[5], *argvB[5];
    int j,jj=0, y;
    int fd[2];
    int pid;

    for(j = 0; j<i; j++){
        argvA[j] = command.argv[j];
    }
    argvA[j] = 0;
    NamecommandA = argvA[0];
    i++;

    for(y = i; y<command.argc; y++){
        argvB[jj] = command.argv[y];
        jj++;
    }
    argvB[j] = 0;
    NamecommandB = argvB[0];

    //gest process

     if(pipe(fd)){
        perror("Pipe Error...");
        exit(127);
     }
    
    switch (pid = fork())
    {
    case -1:
        perror("Fork Error...");
        return 0;
    case 0:
        /*Child*/
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        //char *argA[] = {"cat", "main.c", (char*)0};
        //char *cmd = "cat";
        execvp(NamecommandA, argvA);
        perror(NamecommandA);
        exit(126);
        return 0;
    
    default:
        /*Parent*/
        waitpid(pid, NULL, 0);
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        //char *argB[] = {"grep", "main", (char*)0};
        //char *cmdB = "grep";
        execvp(NamecommandB, argvB);
        perror(NamecommandB);
        exit(125);
        return 0;
    }

    /*--------FOR DEBUG--------*/
    
    printf("%d\n", i);
    printf("%d\n", command.argc);
    for(int j = 0; j < 5; j++){
        printf("argvA[%i] = %s ", j, argvA[j]);
        printf("argvB[%i] = %s\n", j, argvB[j]);
    }
    
}
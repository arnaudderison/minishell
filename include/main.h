#ifndef __main__
#define __main__

#define MAX_BUFFER 1024

//colors
#define COLOR_NRM  "\x1B[0m"
#define COLOR_RED  "\x1B[31m"
#define COLOR_BLU  "\x1B[34m"
#define COLOR_MAG  "\x1B[35m"
#define BOLD "\e[1m"

struct Command {
    char *commandName;
    char *argv[MAX_BUFFER];
    int argc;
};

int readCommand(char *inputCommand);
int parseCommand(char *inputCommand, struct Command *command);
int isInternalCommand();
int changeDir();
int executeNormalCommande();
void clearArg();
int isPipeCommand();
int executePipeCommand(int i);
#endif
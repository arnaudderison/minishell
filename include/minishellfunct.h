#ifndef __minishellfunct__
#define __minishellfunct__

//colors
#define COLOR_NRM  "\x1B[0m"
#define COLOR_RED  "\x1B[31m"
#define COLOR_BLU  "\x1B[34m"
#define COLOR_MAG  "\x1B[35m"
#define BOLD "\e[1m"

#define MAX_BUFFER 1024

struct Command {
    char *commandName;
    char *argv[64];
    int argc;
};

int readCommand(char*, int maxBuf);
int parseCommand(char *inputCommand, struct Command *command);
void emptyCommandArgc(struct Command *command);
int processCommand(struct Command *command);
void startCommand(struct Command *command);
void processPipeCommand(struct Command *command, int i);
void startPipeCommand(char *commandA[], char *commandB[]);
int changeDir(struct Command *command);
void processAndCommand(struct Command *command, int i);
void startAndCommand(char *commandA[], char *commandB[]);

#endif
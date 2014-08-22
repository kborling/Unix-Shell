#ifndef SHELLUTIL_H
#define SHELLUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "linkedList.h"

#define MAX 100

void clearBuffer(void);
void strip(char * s);
char * trimWhiteSpace(char * word);
void clean(int argc, char **argv);
void printargs(int argc, char **argv);
int stringToInt(char * s);
int isValidCharacter(char * s);
int isValidOperator(char * s);
int isValidHistory(char * s);
int isValidWord(char * s);
int isValidFirstWord(char * s);
int isValidPipe(char * s);
int makeargs(char *s, char *** argv);
void forkIt(char ** argv);
void pipeIt(char ** prePipe, char ** postPipe);
void doublePipeIt(char ** prePipe, char ** midPipe, char ** postPipe);
void processFile(FILE * fin, struct Node ** head, int count);
void writeFile(FILE * fin, struct Node * head);
int findCount(FILE * fin);
FILE * openFile();

#endif

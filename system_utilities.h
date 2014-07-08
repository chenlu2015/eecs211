/*

    Computer simulating program

    Author:  Chen Lu
             EECS 211, Winter 2013, Program 7                  

*/
#include "machines.h"

#ifndef SYSTEM_UTILITIES_H
#define SYSTEM_UTILITIES_H

void systemStatus();
int findNode(char *n);
int addNetworkNode(char *toks[], int numberToks);
int deleteNetworkNode(char*toks[], int numberToks);

int createFile(char *comp, char* n, int length, char*dat);
int ls(char *comp);
int printFiles(int argc, char*argv[]);
int deleteFiles(int argc, char*argv[]);

void wait();

void printError(int err);
int str2int(std::string s);
int findInNetwork(char *name);
computer *getNode(int n);
int transferFile(char *c1, char *c2, char*f);

void fillCommandList();

int getCommandNumber(char *s);
int parseCommandLine(char line[], char *tokens[]);

#endif
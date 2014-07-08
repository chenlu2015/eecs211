/*

    Computer simulating program

    Author:  Chen Lu
             EECS 211, Winter 2013, Program 7              

*/


#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define BAD_BLOCK_NUMBER 200

#define MAX_NETWORK_NODES 10

#define NETWORK_FULL 201
#define COMPUTER_NOT_FOUND 202
#define INVALID_COMPUTER_TYPE 203
#define FILE_NOT_FOUND 204

#define MAX_CMD_LINE_LENGTH 255
#define MAX_TOKENS_ON_A_LINE 10

#define FILE_SYSTEM_FULL 205
#define HARD_DRIVE_FULL 206

#define NUMBER_OF_COMMANDS			9
#define SYSTEM_STATUS				50
#define HALT						51
#define ADD_NETWORK_NODE			60
#define DELETE_NETWORK_NODE			61
#define CREATE_FILE					70
#define LS							71
#define DELETE_FILES				72
#define PRINT_FILES					73
#define TRANSFER_FILE				80
#define UNDEFINED_COMMAND			99

struct fileDescriptor {
	char fileName[8];
	int fileLength;
	int firstFilledBlockNum;
};

struct packet {
	char cname[20];
	char fname[8];
	int fileLength;
	char file[4096];
};


#endif
/*

    Computer simulating program

    Author:  Chen Lu
             EECS 211, Winter 2013, Program 7                   

*/


#ifndef MACHINES_H
#define MACHINES_H

#include "definitions.h"
#include "disk_drive.h"

class computer {
private:
	int receive_file(packet p);
protected:
	char name[21];
	diskDrive *diskptr;
	int maximum_number_of_files;
	
	int findFreeFileDescriptor();
	void getFileDescriptor(int n, fileDescriptor *fd);
	void putFileDescriptor(int n, fileDescriptor *fd);
	int findFile(char *fname, fileDescriptor *fd);
	void printOneFile(fileDescriptor fd);
	int delete_one_file(char* s);
	
public:
	//constructor
	computer(char compName[20], int diskSize);
	
	//destructor
	~computer();

	
	int isThisMyName(char *n);
	int createFile(char *n, int len, char*dat);

	int transfer_file(char *f, char *c);
	int delete_files(int argc, char*argv[]);

	virtual void print();
	void printDirectory();
	void printFiles(int argc, char*argv[]);

};

class PC : public computer {
private:
	char ownerName[32];
public:
	PC(char compName[20], int diskSize, char owner[31]);
	~PC();
	void print();
};

class server : public computer {
private:
	char location[16];
public:
	server(char compName[20], int diskSize, char location[15]);
	~server();
	void print();
};

class printer : public computer {
private:
	int numberOfPrinters;
public:
	printer(char compName[20], int diskSize, int p);
	~printer();
	void print();
};


#endif
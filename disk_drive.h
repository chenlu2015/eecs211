/*

    Computer simulating program

    Author:  Chen Lu
             EECS 211, Winter 2013, Program 7               

*/

#ifndef DISK_DRIVE_H
#define DISK_DRIVE_H

#include "definitions.h"

class diskDrive {

private:
	int numberOfBlocks;
	char *bitMap;
	char *data;

public:
	//constructor
	diskDrive(int nBytes);
	
	//destructor
	~diskDrive();
	
	int findNBlocks(int n, int start);

	void formatDrive(int maxFiles);

	int storeBlock(char *d, int blk);

	int retrieveBlock(char *d, int blk);

	
	void print();

	int allocateBlock(int targetBlockNumber);

	int freeBlock(int targetBlockNumber);

	int isBlockFree(int targetBlockNumber);
};

#endif
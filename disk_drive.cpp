#include <iostream>
using namespace std;

#include <math.h>
#include <fstream>
/*
bg
    Computer simulating program

    Author:  Chen Lu
             EECS 211, Winter 2013, Program 7                 

*/


#include "disk_drive.h"

//constructor
diskDrive::diskDrive(int nBytes){

	//stores the number of blocks
	numberOfBlocks = (nBytes+63)/64;
	// allocates the data
	data = (char*)malloc(numberOfBlocks*64*sizeof(char));
	//create the bitmap
	bitMap = (char*)malloc((numberOfBlocks+7)*sizeof(char)/8);

	//sets all the bitmaps to 0
	for (int j = 0; j < (numberOfBlocks+7)/8; j++) {
		bitMap[j]=0;
	}
	cout << "\n";
}

//destructor
diskDrive::~diskDrive() {
	free( (void *)data);
	free( (void *)bitMap);
}

int diskDrive::allocateBlock(int targetBlockNumber) {
	if((targetBlockNumber <0) || (targetBlockNumber >= numberOfBlocks)){
		return BAD_BLOCK_NUMBER;
	} else {
		//change the bitmap
	int pos = targetBlockNumber/8;
	int bitpos = targetBlockNumber%8;
	bitMap[pos] |= (1 << bitpos);
	return 0;
	}
}

int diskDrive::findNBlocks(int n, int start) {
	// FIX THIS
	/*for(int i = start; i< numberOfBlocks-1;i++) {
		if( (isBlockFree(i)==1) && (isBlockFree(i+1) ==1) ) return i;
	}*/
	int currBlock = start;
	int cont=1;
	while(currBlock < numberOfBlocks-n) {

		for(int i =0; i < n; i++) {
			if(isBlockFree(currBlock+i)==1) cont=1;
			else { 
				cont=-1;
				break;
			}
		}

		if(cont==1) return currBlock;

		currBlock+=1;
	}
	
	return -1;
}


void diskDrive::formatDrive(int maxFiles) {
	//IMPLEMENT//IMPLEMENT//IMPLEMENT//IMPLEMENT//IMPLEMENT//IMPLEMENT
	memset(data,0,numberOfBlocks*64);

	for (int i = 1; i <= numberOfBlocks; i++) {
		freeBlock(i-1); // frees all the blocks
	}

	fileDescriptor *fd;
	for (int i = 0; i < maxFiles; i++) {
		fd = ((fileDescriptor*)data)+i;	
		fd->fileLength = -1;
		memset(fd->fileName, 0,8);
		fd->firstFilledBlockNum =0;
	}

	int blocksTaken = (maxFiles+3)/4;
	for(int i = 0; i < blocksTaken; i++) {
		this->allocateBlock(i);
	}

}

int diskDrive::storeBlock(char *d, int blk) {
	if ((blk >= numberOfBlocks) || (blk < 0)) return BAD_BLOCK_NUMBER;
	//IMPLEMENT//IMPLEMENT//IMPLEMENT//IMPLEMENT//IMPLEMENT//IMPLEMENT

	memcpy(data+(blk*64*sizeof(char)),d, 64);
	//data[blk*64] = *d;
	return 0;
}

int diskDrive::retrieveBlock(char *d, int blk) {
	if ((blk >= numberOfBlocks) || (blk < 0)) return BAD_BLOCK_NUMBER;
	//IMPLEMENT//IMPLEMENT//IMPLEMENT//IMPLEMENT//IMPLEMENT//IMPLEMENT
	memcpy(d, data+(blk*64) ,64);
	//*d = data[blk*64];
	return 0;
}

int diskDrive::freeBlock(int targetBlockNumber) {
	if((targetBlockNumber < 0)){
		return BAD_BLOCK_NUMBER;
	} else if (targetBlockNumber >= numberOfBlocks) {
		return BAD_BLOCK_NUMBER;
	} else {
		//change the bitmap
		int pos = targetBlockNumber/8;
		int bitpos = targetBlockNumber%8;

		bitMap[pos] &= ~(1 << bitpos);
		return 0;
	}
}

int diskDrive::isBlockFree(int targetBlockNumber) {
	if((targetBlockNumber <0) || (targetBlockNumber >= numberOfBlocks)){
		return BAD_BLOCK_NUMBER;
	} else {
		if ((bitMap[targetBlockNumber/8] & (1 << targetBlockNumber%8)) == 0 ){
			return 1; //if free return 1
		} else return 0; //otherwise return 0
	}
}

void diskDrive::print() {
	cout <<"     The drive's disk size is: " << numberOfBlocks*64;
	cout <<"\n     The number of blocks in the drive is: " << numberOfBlocks;
	cout <<"\n     Bitmap size: "<< (numberOfBlocks+7)/8 ;
	cout <<"\n     Bitmap: ";
	for (int j = 0; j < (numberOfBlocks+7)/8; j++) {
		int k = bitMap[j];
		k = k&0xff;
		cout <<hex<< k<<dec<<" ";
	}
	cout << "\n\n";

}




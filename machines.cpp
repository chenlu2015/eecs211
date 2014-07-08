#include <iostream>

/*

    Computer simulating program

    Author:  Chen Lu
             EECS 211, Winter 2013, Program 7                   

*/


using namespace std;

#include "machines.h"
#include "disk_drive.h"
#include "system_utilities.h"

computer::computer(char compName[21], int diskSize){
	diskptr = new diskDrive(diskSize);	
	

	//copies the name data
	strcpy(name, compName);
}

void computer::print() {
	cout <<"     Computer Name: "<<name<<"\n";
	if(diskptr != NULL) diskptr->print();
}

int computer::isThisMyName(char*n) {
	if (strcmp(n,name) == 0) {
		return 1;	
	} else
		return 0;
}

int computer::transfer_file(char *f, char *c){
	int result =0;
	fileDescriptor *fd = new fileDescriptor();
	result = findFile(f,fd);
	if (result == -1) return FILE_NOT_FOUND;
	else {
		// check for the computer
		int t = findInNetwork(c);
		if (t == COMPUTER_NOT_FOUND) return COMPUTER_NOT_FOUND;
		else{
			computer *comp = getNode(t); //grab the node pointer
			
			//make the packet
			packet p;

			memset(p.cname,0,20);
			strcpy(p.cname, comp->name);

			p.fileLength = fd->fileLength;

			memset(p.fname,0,8);
			strcpy(p.fname, fd->fileName);

			memset(p.file,0,4096);

		int numblocks = (fd->fileLength+63)/64;
		char temp[64];
		int start = fd->firstFilledBlockNum;

		for (int i = 0; i < numblocks; i++) {
			diskptr->retrieveBlock(temp,i+start);

			memcpy(p.file+(i*64),temp,64);
		}
			//packet done!

			int err = 0;
			err = comp->receive_file(p);
			return err;
		}
	}

	return 0;
}

int computer::receive_file(packet p){
	int err = createFile(p.fname,p.fileLength,p.file);
	return err;
}

int computer::delete_files(int argc, char*argv[]){

	fileDescriptor *fd = new fileDescriptor();
	int fdnum=-1;
	for(int i = 2; i < argc-1; i++) {
		strcpy(fd->fileName,argv[i]);
		fdnum = findFile(argv[i],fd);

		if(fdnum!=-1) {
			delete_one_file(fd->fileName);
			cout << "file deleted!" <<endl;
		}
		else cout<<"ERROR** file "<< fd->fileName<<" was not found! \n";
	}


	return 0;
}

int computer::delete_one_file(char * s) {
	fileDescriptor *fd = new fileDescriptor();
	int f = findFile(s,fd);

	if (f == -1) return FILE_NOT_FOUND;
	else {
		char blank[64];
		memset(blank,0,64);

		int numblocks = (fd->fileLength+63)/64;
		int start = fd->firstFilledBlockNum;
		for (int i = 0; i < numblocks; i++) {
			diskptr->storeBlock(blank,i+start);
			diskptr->freeBlock(i+start);
		}
	}

	fd = new fileDescriptor();
	fd->fileLength = -1;
	memset(fd->fileName, 0,8);
	fd->firstFilledBlockNum =0;

	putFileDescriptor(f,fd); //blank file descriptor


	return 0;
}

void computer::printDirectory(){
	for (int i = 0; i < maximum_number_of_files; i++) {
		fileDescriptor *fd = new fileDescriptor();;
		getFileDescriptor(i,fd);

		if (fd->fileLength == -1) {
			cout<< "      File Entry " << i<<": not in use. \n";
		} else {
			cout<< "      File Entry " << i<<": File Name = ";
			for (int j=0; j<8; j++) cout<< fd->fileName[j];
			cout << ", length = " <<fd->fileLength 
				<< ", First Block = " << fd->firstFilledBlockNum << "\n";
		}
	}
}

//destructor
computer::~computer() {
	delete diskptr;
}

int computer::createFile(char *n, int len, char*dat) {
	//IMPLEMENT//IMPLEMENT//IMPLEMENT//IMPLEMENT//IMPLEMENT//IMPLEMENT
	int emptyfd = findFreeFileDescriptor();
	if (emptyfd == FILE_SYSTEM_FULL) return FILE_SYSTEM_FULL;

	fileDescriptor *nfd = new fileDescriptor();
	nfd->fileLength = len;  //sets file size
	memset(nfd->fileName,0,8);
	int s = strlen(n);
	memcpy(nfd->fileName,n,s);//sets file name

		
	char temp[64]; //buffer for the block to be copied onto
	int numblocks = (len+63)/64;
	int start=0;	
	int blk = emptyfd/4;
	start = diskptr->findNBlocks(numblocks,blk+1);

	

	//saves the data!
	if (start != -1) {

		for (int i = 0; i < numblocks; i++) {
			diskptr->retrieveBlock(temp,i+start);
			memcpy(temp,dat+(i*64),abs((len*i)-64));
			diskptr->storeBlock(temp,i+start);

			diskptr->allocateBlock(i+start);
		}
	} else {
		return HARD_DRIVE_FULL;
	}

	nfd->firstFilledBlockNum = start;

	//saves the file descriptor!
	/*
	diskptr->retrieveBlock(temp,blk); //grabs the block at the block #
	//modify the block, temp
	int pos = emptyfd%4; //position in the block
	memcpy(((fileDescriptor*)temp)+pos,nfd,16);
	//store the block
	diskptr->storeBlock(temp,blk);
	*/
	putFileDescriptor(emptyfd,nfd);

	return 0;
}

int computer::findFreeFileDescriptor() {
	int blocksToCheck = maximum_number_of_files/4;
	
	int emptyFdNumber;

	for (int i = 0; i < blocksToCheck; i++) { //blocks
		char temp[64];
		diskptr->retrieveBlock(temp,i);

		for(int j = 0; j < 4; j++) { //filedescriptor
			fileDescriptor *fd;
			fd = ((fileDescriptor*)temp)+j;

			if (fd->fileLength == -1) {
				emptyFdNumber = i*4 +j; //empty 
				return emptyFdNumber;
			}
		}
		
	}
	return FILE_SYSTEM_FULL;
}

int computer::findFile(char *fname, fileDescriptor *fd) {
	char *ncpy;
	ncpy = (char*)malloc(8*sizeof(char));
	strcpy(ncpy,fname);

	for(int i = 0; i < maximum_number_of_files; i++) {
		getFileDescriptor(i,fd);
		if(strcmp(fd->fileName, ncpy) == 0) {
			free(ncpy);
			return i;
		}
	}
	free(ncpy);
	return -1;
}

void computer::printFiles(int argc, char*argv[]) {
	fileDescriptor *fd = new fileDescriptor();
	int fdnum=-1;
	for(int i = 2; i < argc; i++) {
		strcpy(fd->fileName,argv[i]);
		fdnum = findFile(argv[i],fd);
		if(fdnum!=-1) printOneFile(*fd);
		else cout<<"That file was not found! \n";
	}
}

void computer::printOneFile(fileDescriptor fd) {
	int val = 0;
	val = findFile(fd.fileName,&fd);


	cout << "Printing file "<<fd.fileName <<endl;
	cout<<"============="<<endl;


	char *temp;
	temp = (char*)malloc(64*((fd.fileLength+63)/64)*sizeof(char));
	memset(temp,0,64*((fd.fileLength+63)/64)*sizeof(char));

	for (int i = 0; i<(fd.fileLength+63)/64;i++) {
		diskptr->retrieveBlock(temp+(64*i),fd.firstFilledBlockNum+i);
	}

	cout <<temp;
	cout<<"============="<<endl;

	free(temp);

	cout<< "End of file.\n\n\n\n";
}

void computer::getFileDescriptor(int n, fileDescriptor *fd) {
	//IMPLEMENT//IMPLEMENT//IMPLEMENT//IMPLEMENT//IMPLEMENT//IMPLEMENT
	char buf[64];
	int block = n/4;
	int pos = n%4;
	diskptr->retrieveBlock(buf,block);

	memcpy(fd,(((fileDescriptor*)buf)+pos),16);
	//fd = (((fileDescriptor*)buf)+pos);	
}

void computer::putFileDescriptor(int n, fileDescriptor *fd) {
	//IMPLEMENT//IMPLEMENT//IMPLEMENT//IMPLEMENT//IMPLEMENT
	char buf[64];
	int block = n/4;
	int pos = n%4;
	diskptr->retrieveBlock(buf,block);

	memcpy(((fileDescriptor*)buf)+pos,fd,16);
	diskptr->storeBlock(buf,block);
}

PC::PC(char compName[21], int diskSize, char owner[31]) :computer(compName, diskSize) {
	strcpy(ownerName,owner);
	maximum_number_of_files = 8;
	diskptr->formatDrive(maximum_number_of_files);

}

void PC::print() {
	cout <<"     PC Owner Name: "<<ownerName<<"\n";
	computer::print();
}

server::server(char compName[21], int diskSize, char loc[16]) :computer(compName, diskSize) {
	strcpy(location,loc);
	maximum_number_of_files = 16;
	diskptr->formatDrive(maximum_number_of_files);
}

void server::print() {
	cout <<"     Location: "<<location<<"\n";
	computer::print();

}

printer::printer(char compName[21], int diskSize, int numPrinters) :computer(compName, diskSize) {
	numberOfPrinters = numPrinters;
	maximum_number_of_files = 16;
	diskptr->formatDrive(maximum_number_of_files);
}

void printer::print() {
	cout <<"     Number of Printers: "<<numberOfPrinters<<"\n";
	computer::print();
}
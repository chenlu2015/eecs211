/*

    Computer simulating program

    Author:  Chen Lu
             EECS 211, Winter 2013, Program 7                

*/


#include <iostream>
#include <string>
using namespace std;

#include "system_utilities.h"
#include "definitions.h"
#include "machines.h"


computer *network[MAX_NETWORK_NODES];

int findInNetwork(char *name) {
	for(int i = 0; i < MAX_NETWORK_NODES; i++) {
		int a = network[i]->isThisMyName(name);
		if (a ==1) return i;
	}
	return COMPUTER_NOT_FOUND;
}
int transferFile(char *c1, char *c2, char*f){
	int c = findInNetwork(c1);
	
	int err = 0;
	err = network[c]->transfer_file(f,c2);

	return err;
}


computer * getNode(int n) {
	return network[n];
}


void printError(int err) {

switch(err) {
	case BAD_BLOCK_NUMBER:
		cout << " **ERROR: Bad block number has been encountered. \n\n";
		break;
	case NETWORK_FULL:
		cout << "\n                            FAILED: The Network was full. \n\n";
		break;
	case COMPUTER_NOT_FOUND:
		cout << "                            FAILED: The computer was not found in the network. \n\n";
		break;

	case INVALID_COMPUTER_TYPE:
		cout << "\n                            FAILED: Invalid Computer Type. \n\n";
		break;
	case FILE_SYSTEM_FULL:
		cout << "\n                            FAILED: File System is full. \n\n"; 
		break;
	case HARD_DRIVE_FULL:
		cout << "\n                            FAILED: Hard drive is full, the file cannot be saved. \n\n"; 
		break;
	case FILE_NOT_FOUND:
		cout << "\n                            FAILED: Could not find that file. \n\n"; 
		break;

	default: 
		cout << "                             SUCCESS! \n\n";
	}

}

void wait() {
	std::cout << "Please press ENTER to continue .....";
	std::string trash;
	std::getline(std::cin,trash);
}
void systemStatus() {
	for (int i = 0; i < MAX_NETWORK_NODES; i++) {
				if(network[i]!= NULL) network[i]->print();
	}
}

int createFile(char *comp, char*n, int length, char *dat) {
	int node = findNode(comp);
	if (node ==-1) return COMPUTER_NOT_FOUND;

	int err = network[node]->createFile(n,length,dat);
	if (err == HARD_DRIVE_FULL) return HARD_DRIVE_FULL;
	return 0;
}

int ls(char*comp) {
	int node = findNode(comp);
	if (node ==-1) return COMPUTER_NOT_FOUND;

	network[node]->printDirectory();
	return 0;
}
int printFiles(int argc, char*argv[]){
	int node = findNode(argv[1]);
	if (node ==-1) return COMPUTER_NOT_FOUND;

	network[node]->printFiles(argc,argv);
	return 0;
}
int deleteFiles(int argc, char*argv[]){
	int node = findNode(argv[1]);
	if (node ==-1) return COMPUTER_NOT_FOUND;

	network[node]->delete_files(argc,argv);
	return 0;
}

int findNode(char*n) {
	for(int i = 0; i < MAX_NETWORK_NODES; i++) {
		if(network[i]!= NULL) {
		if (network[i]->isThisMyName(n) == 1) return i;
		}
	}
	return -1;
}

int addNetworkNode(char *toks[], int numberToks) {
	int usableNode = 0;

	for(int i =0; i < MAX_NETWORK_NODES; i++) {
		if (network[i] == NULL) {
			usableNode = i;
			break;
		}
		if (i == MAX_NETWORK_NODES-1) return NETWORK_FULL;
	}

	if(strcmp(toks[1],"PC") == 0) {
		network[usableNode] = new PC(toks[2],str2int(toks[3]),toks[4]);
		return 0;
	} else if (strcmp(toks[1],"server") == 0) {
		network[usableNode] = new server(toks[2],str2int(toks[3]),toks[4]);
		return 0;
	} else if (strcmp(toks[1],"printer") == 0) {
		network[usableNode] = new printer(toks[2],str2int(toks[3]),str2int(toks[4]));
		return 0;
	} else return INVALID_COMPUTER_TYPE;

}

int deleteNetworkNode(char *toks[], int numberToks) {
	int searchResult;
	searchResult = findNode(toks[1]);

	if (searchResult == -1){
		return COMPUTER_NOT_FOUND;
	} else {
		delete network[searchResult];
		network[searchResult] = NULL;
		return 0;
	}


}


int str2int(std::string s) {
	int integer = 0;

	for (int i = 0; i < s.length(); i++) {
		integer = integer*10 + (s[i]-'0');
	}
	
	return integer;
}

class commandElement {
private:
	char *command;
	int commandNum;
public:
	commandElement(char*,int);
	int amIThisCommand(char*);

};

commandElement::commandElement(char *c, int cn) {
	command = (char*) malloc(25*sizeof(char));
	memset(command,0,25);
	memcpy(command,c,25*sizeof(char));
	commandNum = cn;
}

int commandElement::amIThisCommand(char* c) {
	if(strcmp(this->command, c)==0) {
		return commandNum;
	} else {
		return 0;
	}
}

commandElement *com[NUMBER_OF_COMMANDS];

void fillCommandList() {
	
		com[0] = new commandElement("system_status", SYSTEM_STATUS);
		com[1] = new commandElement("halt", HALT);
		com[2] = new commandElement("add_network_node", ADD_NETWORK_NODE);
		com[3] = new commandElement("delete_network_node", DELETE_NETWORK_NODE);
		com[4] = new commandElement("create_file", CREATE_FILE);
		com[5] = new commandElement("ls", LS);
		com[6] = new commandElement("delete_files", DELETE_FILES);
		com[7] = new commandElement("print_files", PRINT_FILES);
		com[8] = new commandElement("transfer_file", TRANSFER_FILE);
	
}


int getCommandNumber(char *s) {

	for(int i = 0; i<NUMBER_OF_COMMANDS; i++) {		
		if (com[i]->amIThisCommand(s) != 0) {
			//this is the command!
			return com[i]->amIThisCommand(s);
		}
	}
	//if not a command
	return UNDEFINED_COMMAND;
}


//parse tokens
int parseCommandLine(char line[], char *tokens[]) {
		int tokenCount = 0;
		int count = 0;
		int tokenStart = -1;
		int tokenEnd = -1;

		char tokenTerminator = ' ';

		// null token terminator
		while (line[count] != 0) {

			if((line[count] == tokenTerminator) && (tokenStart != -1)){
				tokenEnd = count; //if it reaches tokenTerminator, set tokenend
			}

			//sets the tokenstart
			if((line[count] != ' ') && tokenStart == -1) {
				if(line[count] == '"') {
					tokenTerminator = '"'; //quote terminator
					tokenStart = count+1;
				} else {
					tokenTerminator = ' '; //blank terminator
					tokenStart = count;
				}
			}
			
			// save the token
			if ((tokenStart!=-1) && (tokenEnd!=-1)) {
			int size = tokenEnd-tokenStart;
			tokens[tokenCount] = (char*)malloc((size+1)*sizeof(char));
			memset(tokens[tokenCount],0,size+1); //set everything to 0
			for(int i = 0; i < size; i++) {
				tokens[tokenCount][i] = line[tokenStart+i];
			}

				tokenCount++;
				tokenStart=-1;
				tokenEnd=-1;
				tokenTerminator = ' ';
			}

			count++;
	} 
		tokenEnd = count;

		int size = tokenEnd-tokenStart;
		//save the final token
		tokens[tokenCount] = (char*)malloc((size+1)*sizeof(char));
		memset(tokens[tokenCount],0,size+1); //set everything to 0
		for(int i = 0; i < size; i++) {
			tokens[tokenCount][i] = line[tokenStart+i];
		}
		//final token increment
		tokenCount ++;

		return tokenCount;
}
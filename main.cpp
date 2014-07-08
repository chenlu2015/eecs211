/*
	Main.cpp

    Author:  Chen Lu
             EECS 211, Winter 2013, Program 7             

*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "system_utilities.h"
#include "definitions.h"
#include "machines.h"

ifstream file;
int more;

char line[MAX_CMD_LINE_LENGTH+1];
char *mytokens[MAX_TOKENS_ON_A_LINE];

int currentCommandNumber;
int goFlag = 1;



int main() {
	
	file.open("p9input.txt", ios::in);
		if(file.fail()) {
			cout << "Could not open input file.  Program terminating.";
			return 0;
		} else {
			fillCommandList();
		}

		while (goFlag == 1) {

		memset(line,0,MAX_CMD_LINE_LENGTH);
		file.getline(line, MAX_CMD_LINE_LENGTH);
		int numtokens = parseCommandLine(line, mytokens);
		
		currentCommandNumber = getCommandNumber(mytokens[0]);

		int errno;

		switch(currentCommandNumber) {

		case SYSTEM_STATUS:

			errno = 0;
			cout << "\n========================SYSTEM_STATUS=======================\n";
			cout << "Recognized command SYSTEM_STATUS. Printing out network information: \n\n";
			systemStatus();
			cout << "======================END OF SYSTEM_STATUS=====================\n";
			wait();

			break;

		case HALT:
			cout << "You have reached the HALT command. Enter any key to terminate. \n\n";
			goFlag = 0;
			cin >> more;
			return 0;

			break;

		case ADD_NETWORK_NODE:
			errno = 0;
	        cout << "  Recognized command ADD_NETWORK_NODE: \n  Trying to add node.......";
			errno = addNetworkNode(mytokens,numtokens);

			for (int j = 0; j<numtokens;j++) {
				free(mytokens[j]);
			}
			break;

		case DELETE_NETWORK_NODE:

			cout << "  Recognized command DELETE_NETWORK_NODE. \n";
			cout << "  Trying to delete: " << mytokens[1]<<"\n";
			
			errno = 0;
			errno = deleteNetworkNode(mytokens, numtokens);

			for (int j = 0; j<numtokens;j++) {
				free(mytokens[j]);
			}
			break;
		case CREATE_FILE:
			cout << "  Recognized command CREATE_FILE. \n";
			cout << "  Trying to create file: " << mytokens[1]<<" "<<mytokens[2]<<" "<<mytokens[3]<<"\n";

		
			char temp;
			char *buf;
			buf =(char*) malloc(str2int(mytokens[3])*sizeof(char)+1);
			memset(buf,0,str2int(mytokens[3])*sizeof(char)+1);

			for(int i = 0; i <str2int(mytokens[3]) ; i++) {
				temp = file.get();
				buf[i] = buf[i] + temp;
			}


			errno = 0;
			errno = createFile( mytokens[1],mytokens[2], str2int(mytokens[3]), buf);
			
			//frees space
			free(buf);
			for (int j = 0; j<numtokens;j++) {
				free(mytokens[j]);
			}
			break;

		case LS:
			cout << "  Recognized command LS. \n";
			cout << "  Trying to print the directory of: " << mytokens[1]<<"\n";
			
			errno = 0;
			errno = ls(mytokens[1]);
			
			for (int j = 0; j<numtokens;j++) {
				free(mytokens[j]);
			}

			break;
		case TRANSFER_FILE:
			cout << "  Recognized command TRANSFER_FILE. \n";
			

			errno = 0;
			errno = transferFile(mytokens[1],mytokens[2],mytokens[3]);

			break;

		case DELETE_FILES:
			cout << "  Recognized command DELETE_FILES. \n";

			errno = 0;
			errno = deleteFiles(numtokens,mytokens);

			break;
		case PRINT_FILES:
			cout << "  Recognized command PRINT_FILES: \n"<<endl;
			
			errno = printFiles(numtokens, mytokens);


			break;
		case UNDEFINED_COMMAND:
			cout << "Your command input is INVALID. \n";
			cout << "The tokens were: \n";
			for (int j = 0; j<numtokens;j++) {
				cout <<"     "<<mytokens[j] <<"\n";
				free(mytokens[j]);
			}
			cout << "\n\n";
			break;


		}
		printError(errno);
	}
	
	
	
	
	return 0;
}

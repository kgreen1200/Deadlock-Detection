/*
File:			lab07.c
Author:			Kevin Green
Assignment:		Lab 07 - Deadlock Detection
Date:			11/22/2020

Compilation:	gcc -o lab07 lab07.c

Usage:			./lab07
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool bankerAlgorithm(int numProcesses, int numResources, int available[], int allocation[][numResources], int need[][numResources]);

int main(int argc, char *argv[]) {

	FILE *file;						// file object to be read from
	char buff[255];					// input buffer for the file
	char filepath[255];				// filepath to the file
	bool continueProgram = true;	// true until the user decides to quit the program

	// continues until the user enters QUIT or quit in the prompt for a file path
	while (continueProgram) {

		/* STEP 0: GET USER INPUT FOR THE FILEPATH */
		printf("Please enter the file you would like to test. (Enter QUIT to exit): ");
		scanf("%s", filepath);

		// exit program if the user wants to quit
		if (strcmp(filepath, "QUIT") == 0 || strcmp(filepath, "quit") == 0) {
			continueProgram = false;
			break;
		}

		/* STEP 1: OPEN FILE */
		if(!(file = fopen(filepath, "r"))) {
			fprintf(stderr, "\nError: File \"%s\" not found. Please try again.\n\n", filepath);
		}
		else {
			/* STEP 2: LOAD FILE CONTENTS INTO DATA STRUCTURES */
			// read in number of processes
			fscanf(file, "%s", buff);
			int n = atoi(buff);

			// read in number of resource types
			fscanf(file, "%s", buff);
			int m = atoi(buff);

			// read in available
			int available[m];
			for(int i = 0; i < m; i++) {
				fscanf(file, "%s", buff);
				available[i] = atoi(buff);
			}

			// read in allocation matrix
			int allocation[n][m];
			for(int i = 0; i < n; i++) {
				for (int j = 0; j < m; j++) {
					fscanf(file, "%s", buff);
					allocation[i][j] = atoi(buff);
				}
			}

			// read in need matrix
			int need[n][m];
			for(int i = 0; i < n; i++) {
				for (int j = 0; j < m; j++) {
					fscanf(file, "%s", buff);
					need[i][j] = atoi(buff);
				}
			}

			/* STEP 3: PERFORM BANKER'S ALGORITHM */
			bankerAlgorithm(n, m, available, allocation, need);
		}
	}

	return 0;
}

bool bankerAlgorithm(int numProcesses, int numResources, int available[], int allocation[][numResources], int need[][numResources]) {
	
	bool finish[numProcesses]; // array of whether each process has been able to finish, initialized to false
	for (int i = 0; i < numProcesses; i++) {
		finish[i] = false;
	}

	int numSafeProcesses = 0; // number of processes that have been able to finish

	while(numSafeProcesses < numProcesses) {	// do until all processes have been able to finish, if able
		bool foundSafe = false; // used to keep track if a process was able to finish

		// find an i such that both finish[i] = false and need <= available
		for (int i = 0; i < numProcesses; i++) {

			if(finish[i] == false) { // process has not finished yet
				
				// Check to see if need is <= available
				bool lessThanAvail = true;
				for(int j = 0; j < numResources; j++) {
					if(need[i][j] > available[j]) {
						lessThanAvail = false;
						break;
					}
				}
				if (lessThanAvail) { // matches all criteria and is able to finish
					for(int j = 0; j < numResources; j++) {	// available = available + allocation[i]
						available[j] = available[j] + allocation[i][j];
					}
					finish[i] = true;

					foundSafe = true;
					numSafeProcesses = numSafeProcesses + 1;
				}
			}
		}

		// no remaining unfinished processes are able to finish, system is in a deadlock
		if (foundSafe == false) {
			printf("\nThe system will result in deadlock.");
			printf("\nProcesses involved that do not finish due to deadlock:\n");
			for (int i = 0; i < numProcesses; i++) {
				if (finish[i] == false) {
					printf("Process %d\n", i + 1);
				}
			}
			printf("\n");
			return false;
		}
	}

	printf("\nThe system does not have deadlock.\n\n");
	return true;
}
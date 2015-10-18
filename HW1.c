/*Lily Nguyen
	CS 201 HW 1

	In terminal, you run a c program by doing ./programName.c
	AFTER you've compiled by doing gcc programName.c

	In this assignment, you want to be able to do
	./HW1.c numOfRands upperLim*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {

	int numOfRands = atoi(argv[1]);
	int upperLim = atoi(argv[2]);

	int i;

	// If numOfRands = null or upperLim = null, terminate program
	if (argc != 3)
	{
		// Terminate program
		return 1;
	}
	// Else create array with size numOfRands
	else
	{
		time_t t;
		srand(time(NULL));

		int randNums[numOfRands];
		for (i = 0; i < numOfRands; i++)
		{
			randNums[i] = (rand() % upperLim);
		}
		for (i = 0; i < numOfRands; i++)
		{
			printf("%i\n", randNums[i]);
		}
	}

	return 0;
}
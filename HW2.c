/*
	Lily Nguyen
	CS 201 HW 2

	COMPILE		gcc programName.c
	RUN 			./a.out numOfRands upperLim

	argc is number of arguments
	3 Arguments - File Name, numOfRands, upperLim
	argv[0] is the File Name
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define bool char
#define FALSE 0
#define TRUE 1

#define ILLEGAL_RANGE 8

typedef struct node {
	int value;
	struct node * next;
} node;

typedef struct listParameters {
	node * listOfRands;
	int numOfRands;
	int upperLim;
} listParameters;

void printList(node * head) {
	node * current = head;
	current = current->next; // this was my easy fix...printing out 0 always every time...not included in the list itself
	while (current != NULL) {
		printf("%d\n", current->value);
		current = current->next;
	}
} // end print_list

int size(node * head) {
	int count = 0;
	node * current = head;
	while (current->next != NULL) {
		current = current->next;
		count ++;
	}
	return count;
} // end size

void add(node * head, int x) {
	node * current = head;
	while (current->next != NULL) {
		current = current->next;
	}
	// add new variable
	current->next = malloc(sizeof(node));
	current->next->value=x;
	current->next->next = NULL;

	printf("Added %d to Linked List\n", x);
} // end add

void fillList(node * listOfRands, int numOfRands, int upperLim, int currLen) {
	int i;
	for (i = currLen; i < numOfRands; i++) {
		add(listOfRands, (rand() % upperLim));
	}
	printf("Filled Linked List\n");
} // end fillList

void createList(node * listOfRands, int numOfRands, int upperLim) {
	int i;

	for (i = 0; i < numOfRands; i++) {
		add(listOfRands, (rand() % upperLim));
	}
	printf("Created Linked List\n");
} // end createList

void editList(node * head) {

	// compare first node to the next node, traverse accordingly
	node * current = head;
	node * nextNode = head->next;

	while (current->next != NULL) { // while not the end of the list...while there is still a node to compare to
		if (abs(current->value -  nextNode->value) <= ILLEGAL_RANGE) { // see if difference, inclusive of 8
			printf("Deleted Node: %d\n", current->next->value);
			if (nextNode->next == NULL) { // end of the list anyway, no problem
				current->next = NULL; // should break the while loop
			} 
			else { // if not the end of the list and it IS a problem node
				nextNode = nextNode->next; // move the comparison node to the next one
				free(current->next); // DEALLOCATION
				current->next = nextNode; // delete aka skip over that concerning node
			}
		} 
		else { // if the nodes are totally fine, more than 8 between them
			if (nextNode->next == NULL) { // but if it's the end of the list,
				break; // release from the loop!
			} 
			else { // however if there is a node to compare current to, then...
				current = nextNode; // move to the next node scootchy scootch, current = current->next;
				nextNode = nextNode->next; // move to the next node scootchy scootch
			}
		}
	}
	printf("Checked List of Illegal Parameters \n");
} // end editList

void * firstThreadWrapper(void * arg) {
	printf("Start First Thread\n");
 
	listParameters * parameters = (listParameters *)arg;
	
	createList(parameters->listOfRands, parameters->numOfRands, parameters->upperLim);
	printf("Size of List: %d\n", size(parameters->listOfRands));
	printList(parameters->listOfRands);

	return NULL;
} // end firstThreadWrapper

void * secondThreadWrapper(void * arg) {
	printf("Start Second Thread \n");

	listParameters * parameters = (listParameters *)arg;

	editList(parameters->listOfRands);
	printf("Size of List: %d\n", size(parameters->listOfRands));
	printList(parameters->listOfRands);

	while (size(parameters->listOfRands) != parameters->numOfRands) {
		fillList(parameters->listOfRands, parameters->numOfRands, parameters->upperLim, size(parameters->listOfRands));
		printf("Size of List: %d\n", size(parameters->listOfRands));
		printList(parameters->listOfRands);

		editList(parameters->listOfRands);
		printf("Size of List: %d\n", size(parameters->listOfRands));
		printList(parameters->listOfRands);
	}
	return NULL;
} // end secondThreadWrapper

int main(int argc, char *argv[]) {

	srand(time(NULL));

	if (argc != 3) {
		printf("ERROR: Argument Count\n");
		return 1; 
	}
	else if (argc = 3 && atoi(argv[1]) <= 0 && atoi(argv[2]) >= 100) {
		printf("ERROR: Number of Items <= 0\n");
		return 1; 
	}
	else if (argc = 3 && atoi(argv[1]) > 0 && atoi(argv[2]) < 100) {
		printf("ERROR: Random Number Limit < 100\n");
		return 1; 
	}
	else if (argc = 3 && atoi(argv[1]) > 0 && atoi(argv[2]) >= 100){

		listParameters * parameters = malloc(sizeof(listParameters));

		parameters->listOfRands = malloc(sizeof(node));
		parameters->numOfRands = atoi(argv[1]);
		parameters->upperLim = atoi(argv[2]);

		pthread_t firstThread;
		pthread_t secondThread;

		if (pthread_create(&firstThread, NULL, &firstThreadWrapper, parameters)) {
			printf("ERROR: Producer Thread Creation Err");
			return 1;
		}

		pthread_join(firstThread, NULL);
		printf("Finish First Thread\n");

		if (pthread_create(&secondThread, NULL, &secondThreadWrapper, parameters)) {
			printf("ERROR: Consumer Thread Creation Err");
			return 1;
		}

		pthread_join(secondThread, NULL);
		printf("Finish Second Thread\n");

		printf("Finish Program\n");
	}
	else {
		printf("ERROR: Something went wrong in something I didn't catch.\n");
		return 1; 
	}
	return 0; // return 0 of main if successful
} // end main
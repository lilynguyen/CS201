/*
	Lily Nguyen
	CS 201 HW 2

	COMPILE		gcc programName.c
	RUN 			./a.out numOfRands upperLim

	argc is numer of arguments
	3 Arguments - File Name, numOfRands, upperLim
	argv[0] is the File Name
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

// C doesn't have boolean types so you have to manually define it
#define bool char
#define FALSE 0
#define TRUE 1

#define ILLEGAL_RANGE 8

typedef struct node {
	int value;
	struct node * next;
} node;

typedef struct listParameters {
	int numOfRands;
	int upperLim;
} listParameters;

node * LOR; //= malloc(sizeof(node));

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
}

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
}

void createList(node * listOfRands, int numOfRands, int upperLim) {
	int i;

	//LOR = malloc(sizeof(node)); ///////////////////////////////

	for (i = 0; i < numOfRands; i++) {
		add(listOfRands, (rand() % upperLim));
	}
	printf("Created Linked List\n");
}

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
	printf("Edited List of Illegal Parameters \n");
}

void * firstThreadWrapper(void * arg) {
	printf("Start First Thread\n");

	LOR = malloc(sizeof(node)); ///////////////////////////////

	listParameters * parameters = (listParameters *)arg;
	
	createList(LOR, parameters->numOfRands, parameters->upperLim);
	printf("Size of List: %d\n", size(LOR));
	printList(LOR);

	return NULL;
}

void * secondThreadWrapper(void * arg) {
	printf("Start Second Thread \n");

	LOR = malloc(sizeof(node)); ///////////////////////////////

	listParameters * parameters = (listParameters *)arg;

	editList(LOR);
	printf("Size of List: %d\n", size(LOR));
	printList(LOR);

	while (size(LOR) != parameters->numOfRands) {
		fillList(LOR, parameters->numOfRands, parameters->upperLim, size(LOR));
		printf("Size of List: %d\n", size(LOR));
		printList(LOR);

		editList(LOR);
		printf("Size of List: %d\n", size(LOR));
		printList(LOR);
	}
	return NULL;
}

int main(int argc, char *argv[]) {

	srand(time(NULL));

	listParameters * parameters = malloc(sizeof(listParameters));
	parameters->numOfRands = atoi(argv[1]);
	parameters->upperLim = atoi(argv[2]);

	if (argc > 3) {
		printf("ERROR: Too Many Arguments\n");
		return 1; 
	}
	else if (argc < 3) { // seg faulting param, edit so it actually does something
		printf("ERROR: Not Enough Arguments\n");
		return 1;
	}
	else {
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
	return 0; // return 0 of main if successful
}
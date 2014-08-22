#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node
{
	void * data;
	struct Node * next;
};
typedef struct Node Node;

extern int size;

#define MAX 100

void addOrdered(void * value, struct Node ** head);
void * findItem(void * value, struct Node * head);
void clearList(struct Node * head);
void printList(struct Node * head);
void * findLast(struct Node * head);
#endif
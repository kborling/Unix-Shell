#include "linkedList.h"

int size;

/*
 * Helper method to create new nodes for the linked list.
 */
Node * createList(void * value)
{

	Node * newNode;
	if(!(newNode = calloc(1, sizeof(Node))))
		return NULL;
	newNode->data = calloc(strlen((char*)value)+1, sizeof(char));
	strcpy(newNode->data, (char*)value);
	newNode->next = NULL;
	size++;
	return newNode;
}// End createList()

/*
 * Adds values to linked list in order from text input.
 */
void addOrdered(void * value, struct Node ** head)
{
	
	if((*head) == NULL)
		(*head) = createList(value);

	else
	{
	    Node * curr = (*head);
	    Node * newNode;
        newNode = createList(value);

	   if (curr == NULL)
	       curr = newNode;
	   else
	   {
	       while(curr->next)
		      curr = curr->next;

	       curr->next = newNode;
	   }
	}
}// End addOrdered()

/*
 * Searches the list for the specified command by command #.
 */
void * findItem(void * value, struct Node * head)
{
	Node * temp;
	temp = head;
    int count = 1;
    if(head == NULL)
        return;

	while(temp->next)
    {
        if(count == (int)value)
        {
            return temp->data;
        }
        temp = temp->next;
        count++;
    }
    return NULL;
}

/*
 * Frees up the linked list and sets head to NULL.
 */
void clearList(struct Node * head)
{
		Node * cur, * temp;
        cur = head;

		while(cur != NULL)
		{
            temp = cur;
            cur = cur->next;
			free(temp->data);
			free(temp);
		}
		head = NULL;
		size = 0;
}// End clearList()

/*
 * Prints the linked list.
 */
void printList(struct Node * head)
{
if(head == NULL)
	printf("Empty List\n");
else {
	Node * cur = head;
    int x = 1;
	while(cur != NULL)
	{
		printf("%d %s", x++, ((char *)cur->data));
		cur = cur->next;
	}
    printf("\n");
    }
}// End printList()

/*
 * Find last element in list for !! command.
 */
void * findLast(struct Node * head)
{
    if(head == NULL)
        return;
    if (head->next == NULL)
        return head->data;
    else
    {
        Node * cur = head;
        while(cur->next->next)
            cur = cur->next;
        return cur->data;
    }
}
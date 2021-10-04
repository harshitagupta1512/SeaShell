#ifndef _SLinkedList_
#define _SLinkedList_

#include "ElementType.h"


struct node
{
    element_type element;
    struct node* next;
};

typedef struct node* ptr_to_node;

typedef struct node* List;

ptr_to_node MakeNode(element_type x); //Makes a node

List CreateEmptyList(); //create an empty list with header/dummy node
                        // This ptr_to_node i.e. List is the pointer to the header 

void InsertAtFront(List* LP,element_type x);

void InsertAtRear(List LP,element_type x);

// When using Dynamic memory allocation a mechanism to delete/deallocate the allocated memory is also essential
void DeleteNode(ptr_to_node P);

//Delete all elements of value X from the linked list. (Do nothing if there is no such element)
void DeleteAll(List L, element_type x);

//Find the last occurence of element X in linked list and return its position (assume 0 indexed). Return -1 if there is no such element.
int FindLast(List L, element_type x);

void DeleteList(List LP); //Deletes all the nodes including the header node 

void EmptyList(List* LP); //Deletes all the nodes except the header/dummy node

void PrintList(List L);


#endif
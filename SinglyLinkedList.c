#include "SinglyLinkedList.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

ptr_to_node MakeNode(element_type x)
{
    ptr_to_node T = (ptr_to_node)malloc(sizeof(struct node));
    assert(T != NULL);
    T->element = x;
    T->next = NULL;
    return T;
}

List CreateEmptyList()
{
    ptr_to_node LP = MakeNode(0);
    return LP;
}

void InsertAtFront(List *LP, element_type x)
{
    ptr_to_node T = MakeNode(x);

    List L = *LP;
    T->next = L->next;
    L->next = T;
}

void InsertAtRear(List LP, element_type x)
{
    ptr_to_node temp = LP;

    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    ptr_to_node new = MakeNode(x);

    temp->next = new;
}

void DeleteNode(ptr_to_node P)
{

    if (P != NULL)
        free(P);
}

void DeleteList(List L)
{

    ptr_to_node temp;
    temp = L;
    while (temp->next != NULL)
    {
        free(temp);
        temp = temp->next;
    }
    free(temp);
}

void PrintList(List L)
{
    ptr_to_node temp = L;
    while (temp->next != NULL)
    {
        printf(" %d ->", temp->element);
        temp = temp->next;
    }

    printf(" %d ", temp->element);
}

// Delete all nodes except header
void EmptyList(List *LP)
{
    List L = *LP;
    L = L->next;
    ptr_to_node Curr;
    while (L != NULL)
    {
        Curr = L;
        L = L->next;
        DeleteNode(Curr);
    }
    (*LP)->next = NULL;
}

int FindLast(List L, element_type x)
{
    int index = 0;
    int position = -1;
    ptr_to_node temp;
    temp = L;
    while (temp->next != NULL)
    {
        if (temp->element == x)
        {
            position = index;
        }

        index++;

        temp = temp->next;
    }

    if (temp->element == x)
    {
        position = index;
    }

    return position;
}

void DeleteAll(List L, element_type x)
{
    ptr_to_node curr = L;

    while (curr->next != NULL)
    {
        if (curr->next->element == x)
        {
            ptr_to_node temp = curr;
            ptr_to_node toDelete;
            toDelete = temp->next;
            temp->next = toDelete->next;
            free(toDelete);
        }
        if (curr->next != NULL)
            curr = curr->next;
    }
}

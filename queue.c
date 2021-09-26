#include "def.h"

Queue initQueue()
{
    Queue D = (Queue)malloc(sizeof(struct Queue));
    D->Front = NULL;
    D->Rear = NULL;
    D->NumEle = 0;
    return D;
}

void Inject(Queue D, char* x)
{
    if(strcmp(lastAddedCommand, x) != 0)
    {
        PtrQueueEle P = (PtrQueueEle) malloc(sizeof(QueueEle));
        strcpy(P->data, x);
        P->next = NULL;
        P->prev = NULL;

        if (D->Rear == NULL) //Adding the first element in the deque
        {
            D->Rear = P;
            D->Front = P;
        } else {
            D->Rear->next = P;
            P->prev = D->Rear;
            D->Rear = P;
        }

        D->NumEle++;
        strcpy(lastAddedCommand, x);
    }
}

void Pop(Queue D)
{
    if (!IsEmpty(D))
    {

        if (D->Front == D->Rear)
        {
            D->Rear = NULL;
            D->Front = NULL;
            D->NumEle--;
        }
        else
        {
            D->Front = D->Front->next;
            D->Front->prev = NULL;
            D->NumEle--;
        }
    }
    else
    {
        printf(RED "Deletion not Possible : The Queue is Empty\n");
    }
}

int IsEmpty(Queue D)
{
    return D->NumEle == 0;
}

void deleteQueue(Queue D)
{
    PtrQueueEle Curr = D->Front;
    while (Curr->next != NULL)
    {
        PtrQueueEle Temp = Curr;
        free(Temp);
        Curr = Curr->next;
    }
    free(Curr);
    D->Front = NULL;
    D->Rear = NULL;
}

void printQueue(Queue D, int number)
{
    PtrQueueEle Curr = D->Front;

    if (D->NumEle == 0)
        printf(" \n");

    else
    {
        if(D->NumEle <= number)
        {
            int i = 1;
            while (Curr->next != NULL)
            {
                printf(GREEN "[%d]    %s\n", i, Curr->data);
                Curr = Curr->next;
                i++;
            }
        printf(GREEN "[%d]    %s\n", i, Curr->data);

        }
        else
        {
            int n = D->NumEle;
            for(int j = 1; j <= n - number ; j++)
            {
                Curr = Curr-> next;
            }

            int i = 1;
            while (Curr->next != NULL)
            {
                printf(GREEN "[%d]    %s\n", i, Curr->data);
                Curr = Curr->next;
                i++;
            }
            printf(GREEN "[%d]    %s\n", i, Curr->data);
        }

    }
}

#include <stdio.h>
#include "mymalloc.h"

//size of metablock is 2, tested in iLab
typedef struct _metablock
{
    unsigned int size : 12; 
    unsigned int free : 1;
} __attribute__((packed, aligned(1))) metablock; // prevents structure padding that would make this 4 bytes

void initialize()
{ //create a empty metablock
    start->size = 4096 - sizeof(metablock);
    start->free = 1;
}

// Combine curr's and next metablock's size if both are free.
void mergeNext(metablock *curr)
{
    metablock *next = curr + sizeof(metablock) + curr->size;
    if ((void *)next > ((void *)myblock + (sizeof(char) * 4095) - sizeof(metablock)))
    {
        return;
    }
    if (next->free == 1)
    {
        (curr->size) += ((next->size) + sizeof(metablock));
        mergeNext(curr);
    }
    return;
}

void *mymalloc(int memory, int linenum, char *filename)
{
    if (memory > (4096 - sizeof(metablock)))
    { // Memory too large to fit in array
        printf("Error on line #%d in file %s: Attempted saturation of dynamic memory - asking for too much memory!.\n", linenum, filename);
        return NULL;
    }

    metablock *ptr1;
    //check the free memory address

    if (!(start->size)) //point to NULL(means no allocated memory before)
    {
        initialize();
    } //if there is allocated memorry before, we need to find the first fit memory location.
    ptr1 = start;
    while ((ptr1->free == 0) || (ptr1->free == 1 && ptr1->size < memory))
    {
        if ((void *)ptr1 > ((void *)myblock + sizeof(char) * 4095 - sizeof(metablock))) // ptr1 surpassed myblock's memory limit
        {
            printf("Error on line #%d in file %s: Not enough available memory to malloc\n", linenum, filename);
            return NULL; //failed and return
        }

        ptr1 = (void *)((void *)ptr1 + ptr1->size + sizeof(metablock));
        //now we get to the location
    }
    //create new empty block
    metablock *new = (void *)((void *)ptr1 + memory + sizeof(metablock));
    new->size = ptr1->size - memory - sizeof(metablock);
    new->free = 1;
    ptr1->size = memory;
    ptr1->free = 0;
    //ptr1 is our allocated memory metablock
    void *result;
    result = (void *)(++ptr1);
    return result;
}

void *myfree(void *givenBlock, int linenum, char *filename)
{
    if ((((metablock *)givenBlock)->size) > (4096 - sizeof(metablock)))
    { // Too big, couldn't have existed in our memory block
        printf("Error on line #%d in file %s: Trying to free memory that was not allocated by malloc.\n", linenum, filename);
        return NULL;
    }

    metablock *curr, *prev;
    curr = start;
    prev = NULL;
    while (!curr)
    { //curr will go through all the char[]
        if (curr != givenBlock)
        { //when not find it, jump to next metablock
            prev = curr;
            curr = (void *)((void *)curr + curr->size + sizeof(metablock));
        }
        else
        { //we find it
            break;
        }
    }
    if (curr != givenBlock)
    { // checked all of myBlock, but givenBlock not found
        printf("Error on line #%d in file %s: Trying to free memory that was not allocated by malloc.\n", linenum, filename);
        return NULL;
    } ///that' great

    //so now it should be case found it
    if ((curr->free) == 1)
    { // Error. givenBlock was freed before. Don't free twice.
        printf("Error on line #%d in file %s: Redundant freeing - pointer was freed before.\n", linenum, filename);
        return NULL;
    }
    curr->free = 1;

    if (prev->free == 1) // Check if there are free metablocks after the newly merged prev
    {
        (prev->size) += ((curr->size) + sizeof(metablock));
        mergeNext(prev);
        return prev;
    }
    else
    {
        mergeNext(curr);
        return curr;
    }
}

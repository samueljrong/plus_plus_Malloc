#include<stdio.h>
#include "mymalloc.h"

//size of metablock is 2, tested in iLab
typedef struct _metablock
{
    unsigned int size:12;
    unsigned int free:1;
    unsigned int magicNum:3;
}__attribute__((packed, aligned(1))) metablock; // this prevents structure padding

void* mymalloc(int memory, int linenum, char* filename){
    //check the free memory address

    
    struct metablock *newblock = (void *) myblock;
    if(newblock) //point to NULL(means no allocated memory before)
    {
        newblock->size = memory;
        newblock->free = 1;
        return newblock;
    }

    //if there is allocated memory before, we need to find it 
    while(newblock->free == 1||newblock->size<memory)
    {    //loop until we found a block is free and have enough space 
        *newblock = newblock + 4 + newblock->size; //jump to the next block
    }
    
    
    
    //printf("error on line #%d in file %s\n", linenum, filename);
    //return NULL;
}

void* myfree(void*, int, char*){






    //printf("error on line #%d in file %s\n", linenum, filename);
    //return NULL;
}

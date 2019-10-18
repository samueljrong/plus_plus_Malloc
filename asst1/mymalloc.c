#include <stdio.h>
#include "mymalloc.h"

//size of metablock is 2, tested in iLab
typedef struct _metablock
{ 
    unsigned int size:12;  //do we need to declare our function in header file first ? yeah that'll be best practice i think, but not necessary
    unsigned int free:1;
}__attribute__((packed, aligned(1))) metablock; // prevents structure padding that makes this 4 bytes

void initialize(){   //create a empty mteablock
    start->size = 4096-sizeof(metablock);
    start->free = 1;
}

// Combine curr's and next metablock's size if both are free.
void mergeNext(metablock* curr) {
    metablock* next = (void*)curr + sizeof(metablock) + curr->size;
    if ((void*)next > ((void*)myblock + sizeof(char)*4095 - sizeof(metablock))) {
        return;
    }
    if (next->free == 1) {
        (curr->size) += ((next->size) + sizeof(metablock));
        mergeNext(next);
    }
    return;
}

void* mymalloc(int memory, int linenum, char* filename){
    if (memory > (4096 - sizeof(metablock))) { // Memory too large to fit in array
        printf("Error on line #%d in file %s\n\t Attempted saturation of dynamic memory - asking for too much memory!.\n", linenum, filename);
    }
    //i didn't add the case when no enough  space for allocated 
    metablock *ptr1;
    //check the free memory address
    
    
    if(!(start->size)) //point to NULL(means no allocated memory before)
    {
        initialize();
    }              //if there is allocated memorry before, we need to find the first fit memory location.
        ptr1 = start;
        while((ptr1->free ==0)||(ptr1->free == 1 && ptr1->size<memory)){
            //i just said pointer keep going
            // Oh i see. we'll just add a conditional to see if pointer reached the end of array
            // then return error yep
            if ((void*)ptr1 > ((void*)myblock + sizeof(char)*4095 - sizeof(metablock))) // ptr1 surpassed myblock's memory limit
            {
                printf("Error on line #%d in file %s\n\t Not enough available memory to malloc\n", linenum, filename);
                return NULL;  //failed and return 
            }
            
            ptr1 = (void*)((void*)ptr1 + ptr1->size + sizeof(metablock));
        //now we get to the location
        }
        //create new empty block
        metablock *new =(void*)((void*)ptr1 + memory + sizeof(metablock));
        new->size = ptr1->size - memory - sizeof(metablock);
        new->free = 1;
        ptr1->size = memory;
        ptr1->free = 0;
        //ptr1 is our allocated memory metablock 
        void *result;
        result = (void*)(++ptr1);
        return result;
    
    
    
    
}

void* myfree(void* givenBlock, int linenum, char* filename){

    //how about we check giveBlock == pointer in array
    // 
    // Oh I see what you mean. So to see if the givenBlock has the same variables as
    // the location it points at.
    //yep, since we already need to parse it
    // okay im assuming i will have a pointer that goes through all metablocks, and 
    // if we don't have a valid match for any pointer, then this givenBlock doesn't exist in array
    //yes
    //i don't know how to set up case when we go through the array, but not found it
    // oh i think we'll just add another conditional comparing curr == givenBlock, and then error
    if ((((metablock*)givenBlock)->size) > (4096 - sizeof(metablock))) { // Too big, couldn't have existed
        printf("Error on line #%d in file %s\n\t Trying to free memory that was not allocated by malloc.\n", linenum, filename);
        return NULL;
    }
    
    metablock *curr, *prev;
    curr = start;
    prev = NULL;
    while(!curr){  //curr will go through all the char[]
        if(curr != givenBlock){ //when not find it, jump to next metablock
            prev = curr;
            curr = (void*)((void*)curr + curr->size + sizeof(metablock)); 
        }else{ //we find it
            break;
        }
    }
    if (curr != givenBlock) { // checked all of myBlock, but givenBlock not found
        printf("Error on line #%d in file %s\n\t Trying to free memory that was not allocated by malloc.\n", linenum, filename);
        return NULL;
    } ///that' great
    
    //so now it should be case found it 
    if ((curr->free) == 1) { // Error. givenBlock was freed before. Don't free twice.
        printf("Error on line #%d in file %s\n\t Redundant freeing - pointer was freed before.\n", linenum, filename);
        return NULL;        
    }
    curr -> free = 1; 
    //and do we need to clean up the data? set char[] = /0
    //since when people allocated, they store their date in the array
    // do they? I assumed that there's nothing actually in the char[], only the metadata and our size
    // and that when we free, we just set free = 1. And only when we see two adjacent free metablocks
    // do we actually clean it up and delete the metablock. I think. got it
    if(prev->free == 1){
        (curr->size) += ((prev->size) + sizeof(metablock));
    }

    // should we create a merge() function? Like "void merge(void" yep, it may help
    // yeah cause i think it'll just keep finding consecutive free blocks
    // Check if there are free metablocks after curr
    mergeNext(curr);
    
    //sam, how to u merge two free metablock
    // After freeing the 2nd metablock (so set it's free = 1):
    // i think we'll have a new pointer going from beginning to end, and then
    // it will find the first free metablock. Then it checks if the next metablock is also free.
    // so it deletes the next metablock, and adds the next metablock's size to the first
    // free metablock's size.


    //printf("error on line #%d in file %s\n", linenum, filename);
    //return NULL;
}

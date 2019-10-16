#include <stdio.h>
#include "mymalloc.h"
// #define MAGIC 7

//size of metablock is 2, tested in iLab
typedef struct _metablock
{
    unsigned int size:12;
    unsigned int free:1;
    // unsigned int magicNum:3;
}__attribute__((packed, aligned(1))) metablock; // this prevents structure padding

void initialize(){   //create a empty mteablock
    start->size = 4096-sizeof(struct metablock);
    start->free = 1;
}


void* mymalloc(int memory, int linenum, char* filename){
    if (memory > (4096 - sizeof(metablock)) { // Memory too large to fit in array
        printf("error on line #%d in file %s\n", linenum, filename);
    }
    //i didn't add the case when no enough  space for allocated 
    struct metablock *ptr1;
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
            if (ptr1 > &(myblock[4095] - sizeof(metablock))) // ptr1 surpassed myblock's memory limit
            {
                printf("error on line #%d in file %s\n", linenum, filename);
                return NULL;  //failed and return 
            }
            
            ptr1 = (void*)((void*)ptr1 + ptr1->size + sizeof(struct metablock));
        //now we get to the location
        }
        //create new empty block
        struct metablock *new =(void*)((void*)ptr1 + memory + sizeof(struct metablock));
        new->size = ptr1->size - memory - sizeof(struct metablock);
        new->free = 1;
        ptr1->size = memory;
        ptr1->free = 0;
        //ptr1 is our allocated memory metablock 
        void *result;
        result = (void*)(++ptr1);
        return result;
    
    
    
    
}

void* myfree(void* givenBlock, int linenum, char* filename){
    // // Check if given metablock to free is an existing metablock
    // if ((givenBlock->magicNum) != MAGIC) {
    //     // Error, not a malloc'ed metablock we provided
    //     printf("error on line #%d in file %s\n", linenum, filename);
    // }

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
    if ((givenBlock->size) > (4096 - sizeof(metablock))) { // Too big, couldn't have existed
        printf("error on line #%d in file %s\n", linenum, filename);
        return NULL;
    }
    
    struct metablock *curr, *prev;
    curr = start;
    prev = NULL;
    while(!curr){  //curr will go through all the char[]
        if(curr != givenBlock){} //when not find it, jump to next metablock
            prev = curr;
            curr = (void*)((void*)ptr1 + ptr1->size + sizeof(struct metablock)); 
        }else{ //we find it
            break;
        }
    }
    if (curr != givenBlock) { // checked all of myBlock, but givenBlock not found
        printf("error on line #%d in file %s\n", linenum, filename);
        return NULL;
    } ///that' great
    
    //so now it should be case found it 
    curr -> 
    

    
    //sam, how to u merge two free metablock
    // After freeing the 2nd metablock (so set it's free = 1):
    // i think we'll have a new pointer going from beginning to end, and then
    // it will find the first free metablock. Then it checks if the next metablock is also free.
    // so it deletes the next metablock, and adds the next metablock's size to the first
    // free metablock's size.


    //printf("error on line #%d in file %s\n", linenum, filename);
    //return NULL;
}

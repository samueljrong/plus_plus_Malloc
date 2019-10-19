#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// Memgrind: a series of workloads to test our mymalloc and myfree implementations.
// Each workload will be run 100 times, one after the other.

// NEED TO RECORD RUNTIME OF EACH WORKLOAD AND CREATE MEANs

int main(int argc, char **argv)
{
    struct timeval startTime; 
    struct timeval endTime;
    
    // Workload A: malloc() 1 byte and immediately free it - do this 150 times
    
    long runtimeA = 0L;
    int i, j, k;
    for (i = 0; i < 100; i++)
    {
        gettimeofday(&startTime, NULL);
        for (j = 0; j < 150; j++)
        {
            void *ptr = (void *)malloc(1);
            free(ptr);
        }
        gettimeofday(&endTime, NULL);
        runtimeA += ((endTime.tv_sec-startTime.tv_sec)*1000000L + endTime.tv_usec-startTime.tv_usec);
    }
    runtimeA = (runtimeA / 100); // Calculate mean runtime of workload A
    
    // Workload B: malloc() 1 byte, store the pointer in an array - do this 150 times.
    // Once you've malloc()ed 50 byte chunks, then free() the 50 1 byte pointers one by one.
    for (i = 0; i < 100; i++)
    {
        void *ptrArr[50];
        for (j = 0; j < 3; j++)
        {
            for (k = 0; k < 50; k++)
            {
                void *ptr = (void *)malloc(1);
                ptrArr[k] = ptr;
            }
            for (k = 0; k < 50; k++)
            {
                free(ptrArr[k]);
            }
        }
    }


    // Workload C: Randomly choose between a 1 byte malloc() or free()ing a 1 byte pointer
    //      > do this until you have allocated 50 times
    // - Keep track of each operation so that you eventually malloc() 50 bytes, in total
    //      > if you have already allocated 50 times, disregard the random and just free() on each iteration
    // - Keep track of each operation so that you eventually free() all pointers
    //      > don't allow a free() if you have no pointers to free()
    for (i = 0; i < 100; i++)
    {
        int counter = 0;             // Total amount of times malloc() has been called.
        int remainingPtrs = counter; // Total number of malloc()ed pointers remaining.
        void *ptrArr[50];
        while (counter < 50) // Loop until malloc() called 50 times total.
        {
            if ((rand() % 2) == 0) // Malloc 1 byte
            {
                void *ptr = (void *)malloc(1);
                ptrArr[remainingPtrs];
                counter++;
                remainingPtrs++;
            }
            else
            {
                if (remainingPtrs > 0) // Only free if a pointer exists
                {
                    remainingPtrs--;
                    free(ptrArr[remainingPtrs]);
                }
            }
        }
        while (remainingPtrs > 0)
        { // Free rest of memory.
            remainingPtrs--;
            free(ptrArr[remainingPtrs]);
        }
    }
    

    // ERROR, might be going out of array in this workload
    
    // Workload D: Randomly choose between a randomly-sized malloc() or free()ing a pointer – do this many times (see below)
    // Keep track of each malloc so that all mallocs do not exceed your total memory capacity
    // Keep track of each operation so that you eventually malloc() 50 times
    // Keep track of each operation so that you eventually free() all pointers
    // Choose a random allocation size between 1 and 64 bytes
    for (i = 0; i < 100; i++)
    {
        int counter = 0;             // Total amount of times malloc() has been called.
        int remainingPtrs = counter; // Total number of malloc()ed pointers remaining.
        void *ptrArr[50];
        while (counter < 50) // Loop until malloc() called 50 times total.
        {
            if ((rand() % 2) == 0) // Malloc between 1 and 64 bytes
            {
                int randSize = (rand() % 64 + 1);
                void *ptr = (void *)malloc(randSize);
                if (ptr != NULL)
                { // If NULL, malloc failed, possibly due to exceeding total memory capacity.
                    ptrArr[remainingPtrs];
                    counter++;
                    remainingPtrs++;
                }
            }
            else // Free a pointer
            {
                if (remainingPtrs > 0) // Only free if a pointer exists
                {
                    remainingPtrs--;
                    free(ptrArr[remainingPtrs]);
                }
            }
        }
        while (remainingPtrs > 0)
        { // Free rest of memory.
            remainingPtrs--;
            free(ptrArr[remainingPtrs]);
        }
    }

    // Workload E
    for (i = 0; i < 100; i++)
    {
    }

    // Workload F
    for (i = 0; i < 100; i++)
    {
    }

    return 0;
}

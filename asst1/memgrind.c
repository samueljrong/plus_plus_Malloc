#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// Memgrind: a series of workloads to test our mymalloc and myfree implementations.
// Each workload will be run 100 times, one after the other.

int main(int argc, char **argv)
{
    struct timeval startTime;
    struct timeval endTime;

    // Workload A: malloc() 1 byte and immediately free it - do this 150 times
    double runtimeA = 0.0;
    int i, j, k;
    for (i = 0; i < 100; i++)
    {
        gettimeofday(&startTime, NULL);
        for (j = 0; j < 150; j++)
        {
            void *ptr = (void *)malloc(1);
            if (ptr != NULL)
            {
                free(ptr);
            }
        }
        gettimeofday(&endTime, NULL);
        runtimeA += ((endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000000.0);
    }
    runtimeA = (runtimeA / 100); // Calculate mean runtime of workload A

    // Workload B: malloc() 1 byte, store the pointer in an array - do this 150 times.
    // Once you've malloc()ed 50 byte chunks, then free() the 50 1 byte pointers one by one.
    double runtimeB = 0.0;
    for (i = 0; i < 100; i++)
    {
        gettimeofday(&startTime, NULL);
        void *ptrArr[50];
        for (j = 0; j < 3; j++)
        {
            for (k = 0; k < 50; k++)
            {
                void *ptr = (void *)malloc(1);
                if (ptr != NULL) {
                    ptrArr[k] = ptr;
                }
            }
            for (k = 0; k < 50; k++)
            {
                if (ptrArr[k] != NULL) {
                    free(ptrArr[k]);
                }
            }
        }
        gettimeofday(&endTime, NULL);
        runtimeB += ((endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000000.0);
    }
    runtimeB = (runtimeB / 100);

    // Workload C: Randomly choose between a 1 byte malloc() or free()ing a 1 byte pointer
    //      > do this until you have allocated 50 times
    // - Keep track of each operation so that you eventually malloc() 50 bytes, in total
    //      > if you have already allocated 50 times, disregard the random and just free() on each iteration
    // - Keep track of each operation so that you eventually free() all pointers
    //      > don't allow a free() if you have no pointers to free()
    double runtimeC = 0.0;
    for (i = 0; i < 100; i++)
    {
        gettimeofday(&startTime, NULL);
        int counter = 0;             // Total amount of times malloc() has been called.
        int remainingPtrs = counter; // Total number of malloc()ed pointers remaining.
        void *ptrArr[50];
        while (counter < 50) // Loop until malloc() called 50 times total.
        {
            if ((rand() % 2) == 0) // Malloc 1 byte
            {
                void *ptr = malloc(1);
                if (ptr != NULL)
                {
                    ptrArr[remainingPtrs] = ptr;
                    counter++;
                    remainingPtrs++;
                }
            }
            else
            {
                if (remainingPtrs > 0) // Only free if a pointer exists
                {
                    remainingPtrs--;
                    if (ptrArr[remainingPtrs] != NULL)
                    {
                        free(ptrArr[remainingPtrs]);
                    }
                    else
                    {
                        remainingPtrs++;
                    }
                }
            }
        }
        while (remainingPtrs > 0)
        { // Free rest of memory.
            remainingPtrs--;
            if (ptrArr[remainingPtrs] != NULL) {
                free(ptrArr[remainingPtrs]);
            }
        }
        gettimeofday(&endTime, NULL);
        runtimeC += ((endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000000.0);
    }
    runtimeC = (runtimeC / 100);

    // Workload D: Randomly choose between a randomly-sized malloc() or free()ing a pointer – do this many times (see below)
    // Keep track of each malloc so that all mallocs do not exceed your total memory capacity
    // Keep track of each operation so that you eventually malloc() 50 times
    // Keep track of each operation so that you eventually free() all pointers
    // Choose a random allocation size between 1 and 64 bytes
    double runtimeD = 0.0;
    for (i = 0; i < 100; i++)
    {
        gettimeofday(&startTime, NULL);
        int counter = 0;             // Total amount of times malloc() has been called.
        int remainingPtrs = counter; // Total number of malloc()ed pointers remaining.
        void *ptrArr[50];
        while (counter < 50) // Loop until malloc() called 50 times total.
        {
            if ((rand() % 2) == 0) // Malloc between 1 and 64 bytes
            {
                int randSize = (rand() % 64 + 1);

                void *ptr = malloc(randSize);
                if (ptr != NULL)
                { // If NULL, malloc failed, possibly due to exceeding total memory capacity.
                    ptrArr[remainingPtrs] = ptr;
                    counter++;
                    remainingPtrs++;
                }
            }
            else // Free a pointer
            {
                if (remainingPtrs > 0 && remainingPtrs < 51) // Only free if a pointer exists
                {
                    remainingPtrs--;
                    if (ptrArr[remainingPtrs] != NULL)
                    {
                        free(ptrArr[remainingPtrs]);
                    }
                    else
                    {
                        remainingPtrs++;
                    }
                }
            }
        }
        while (remainingPtrs > 0)
        { // Free rest of memory.
            remainingPtrs--;
            if (ptrArr[remainingPtrs] != NULL)
            {
                free(ptrArr[remainingPtrs]);
            }
            else
            {
                remainingPtrs++;
            }
        }
        gettimeofday(&endTime, NULL);
        runtimeD += ((endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000000.0); // Add runtime of current workload to total runtime
    }
    runtimeD = (runtimeD / 100); // Mean runtime of 100 workloads

    // Workload E
    double runtimeE = 0.0;
    for (i = 0; i < 100; i++)
    {
    }
    runtimeE = (runtimeE / 100); // Mean runtime of 100 workloads

    // Workload F
    double runtimeF = 0.0;
    for (i = 0; i < 100; i++)
    {
    }
    runtimeF = (runtimeF / 100); // Mean runtime of 100 workloads

    printf("Mean runtime for workload A: %0.6f seconds\n", runtimeA);
    printf("Mean runtime for workload B: %0.6f seconds\n", runtimeB);
    printf("Mean runtime for workload C: %0.6f seconds\n", runtimeC);
    printf("Mean runtime for workload D: %0.6f seconds\n", runtimeD);
    printf("Mean runtime for workload E: %0.6f seconds\n", runtimeE);
    printf("Mean runtime for workload F: %0.6f seconds\n", runtimeF);

    return 0;
}

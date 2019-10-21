#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define NUM_WORKLOADS 100

// Memgrind: a series of workloads to test our mymalloc and myfree implementations.
// Each workload will be run NUM_WORKLOADS times, one after the other.

double workloadA(double *, struct timeval, struct timeval);
double workloadB(double *, struct timeval, struct timeval);
double workloadC(double *, struct timeval, struct timeval);
double workloadD(double *, struct timeval, struct timeval);
double workloadE(double *, struct timeval, struct timeval);
double workloadF(double *, struct timeval, struct timeval);
void calculateRuntime(double *runtime, struct timeval, struct timeval);

// Calculate runtime in microseconds.
void calculateRuntime(double *runtime, struct timeval startTime, struct timeval endTime)
{
    *runtime += ((endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000000.0);
}

// Workload A: malloc() 1 byte and immediately free it - do this 150 times
double workloadA(double *runtime, struct timeval startTime, struct timeval endTime)
{
    int i;
    gettimeofday(&startTime, NULL);
    for (i = 0; i < 150; i++)
    {
        void *ptr = (void *)malloc(1);
        if (ptr != NULL)
        {
            free(ptr);
        }
    }
    gettimeofday(&endTime, NULL);
    calculateRuntime(runtime, startTime, endTime);
    return *runtime;
}

// Workload B: malloc() 1 byte, store the pointer in an array - do this 150 times.
// Once you've malloc()ed 50 byte chunks, then free() the 50 1 byte pointers one by one.
double workloadB(double *runtime, struct timeval startTime, struct timeval endTime)
{
    int j, k;
    gettimeofday(&startTime, NULL);
    void *ptrArr[50];
    for (j = 0; j < 3; j++)
    {
        for (k = 0; k < 50; k++)
        {
            void *ptr = (void *)malloc(1);
            if (ptr != NULL)
            {
                ptrArr[k] = ptr;
            }
        }
        for (k = 0; k < 50; k++)
        {
            if (ptrArr[k] != NULL)
            {
                free(ptrArr[k]);
            }
        }
    }
    gettimeofday(&endTime, NULL);
    calculateRuntime(runtime, startTime, endTime);
    return *runtime;
}

// Workload C: Randomly choose between a 1 byte malloc() or free()ing a 1 byte pointer
//      > do this until you have allocated 50 times
// - Keep track of each operation so that you eventually malloc() 50 bytes, in total
//      > if you have already allocated 50 times, disregard the random and just free() on each iteration
// - Keep track of each operation so that you eventually free() all pointers
//      > don't allow a free() if you have no pointers to free()
double workloadC(double *runtime, struct timeval startTime, struct timeval endTime)
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
        if (ptrArr[remainingPtrs] != NULL)
        {
            free(ptrArr[remainingPtrs]);
        }
    }
    gettimeofday(&endTime, NULL);
    calculateRuntime(runtime, startTime, endTime);
    return *runtime;
}

// Workload D: Randomly choose between a randomly-sized malloc() or free()ing a pointer – do this many times (see below)
// Keep track of each malloc so that all mallocs do not exceed your total memory capacity
// Keep track of each operation so that you eventually malloc() 50 times
// Keep track of each operation so that you eventually free() all pointers
// Choose a random allocation size between 1 and 64 bytes
double workloadD(double *runtime, struct timeval startTime, struct timeval endTime)
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
    calculateRuntime(runtime, startTime, endTime);
    return *runtime;
}

// Workload E:
//malloc(1) bytes store the pointer in array - do this 150 times.
//then, freeing the pointer in the even index in the pointer array one by one. And then freeing the pointer in the odd index in the pointer array.
double workloadE(double *runtime, struct timeval startTime, struct timeval endTime)
{
    int i;
    gettimeofday(&startTime, NULL);
    void *ptrArr[150];
    for (i = 0; i < 150; i++)
    {
        void *ptr = (void *)malloc(1);
        if (ptr != NULL)
        {
            ptrArr[i] = ptr;
        }
    }
    for (i =0; i<150; i=i+2)
    {
        if (ptrArr[i] != NULL)
        {
            free(ptrArr[i]);
        }
    }
    for(i = 1; i<150; i=i+2){
        if (ptrArr[i] != NULL)
        {
            free(ptrArr[i]);
        }
    }
    gettimeofday(&endTime, NULL);
    calculateRuntime(runtime, startTime, endTime);
    return *runtime;
}

// Workload F:

//do 3 interation
//first time malloc(1) bytes for 50 times, and then free some of pointers
//second time malloc (2) bytes for 50 times, and free some of pointers
//third time malloc (3) bytes for 50 times, and free some of pointers
double workloadF(double *runtime, struct timeval startTime, struct timeval endTime)
{
    int i , j;
    int counter = 0;
    gettimeofday(&startTime, NULL);
    void *ptrArr[150];
    // create 20 metablock pointers used to test merge
    for (j = 0; j < 3; j++)
    {
        for (i = 0; i < 50; i++)
        {
            void *ptr = (void *)malloc(1+j);
            if (ptr != NULL)
            {
                ptrArr[i+j*50] = ptr;
            }
        }
        for (i = 0; i < 50; i++)
        {
            if(i%4 != 0 && ptrArr[i+j*50] != NULL)
            {
                free(ptrArr[i+j*50]);
            }
        }
    }
    
    gettimeofday(&endTime, NULL);
    calculateRuntime(runtime, startTime, endTime);
    return *runtime;
}

int main(int argc, char **argv)
{
    struct timeval startTime, endTime;
    double runtimeA, runtimeB, runtimeC, runtimeD, runtimeE, runtimeF;
    runtimeA = runtimeB = runtimeC = runtimeD = runtimeE = runtimeF = 0;
    int i;

    for (i = 0; i < NUM_WORKLOADS; i++)
    {
        workloadA(&runtimeA, startTime, endTime);
        workloadB(&runtimeB, startTime, endTime);
        workloadC(&runtimeC, startTime, endTime);
        workloadD(&runtimeD, startTime, endTime);
        workloadE(&runtimeE, startTime, endTime);
        workloadF(&runtimeF, startTime, endTime);
    }

    runtimeA = runtimeA / NUM_WORKLOADS;
    runtimeB = runtimeB / NUM_WORKLOADS;
    runtimeC = runtimeC / NUM_WORKLOADS;
    runtimeD = runtimeD / NUM_WORKLOADS;
    runtimeE = runtimeE / NUM_WORKLOADS;
    runtimeF = runtimeF / NUM_WORKLOADS;

    printf("Mean runtime for workload A: %0.6f seconds\n", runtimeA);
    printf("Mean runtime for workload B: %0.6f seconds\n", runtimeB);
    printf("Mean runtime for workload C: %0.6f seconds\n", runtimeC);
    printf("Mean runtime for workload D: %0.6f seconds\n", runtimeD);
    printf("Mean runtime for workload E: %0.6f seconds\n", runtimeE);
    printf("Mean runtime for workload F: %0.6f seconds\n", runtimeF);

    return 0;
}

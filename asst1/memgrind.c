#include <stdio.h>

// Memgrind: a series of workloads to test our mymalloc and myfree implementations.

int main(int argc, char** argv) {
    // Workload A: malloc() 1 byte and immediately free it - do this 150 times
    int i;
    for(i = 0; i < 150; i++){
        void* ptr = (void*)malloc(1);
        free(ptr);
    }

    // Workload B: malloc() 1 byte, store the pointer in an array - do this 150 times.
    // Once you've malloc()ed 50 byte chunks, then free() the 50 1 byte pointers one by one.
    void* ptrArr[50];
    int j;
    for(i = 0; i < 3; i++){
        for(j = 0; j < 50; j++){
            void* ptr = (void*)malloc(1);
            ptrArr[j] = ptr;
        }
        for(j = 0; j < 50; j++){
            free(ptrArr[j]);
        }
    }

    // Workload C

    return 0;
}

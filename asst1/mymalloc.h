#include<stdio.h>
 
#ifndef _MYMALLOC_H
#define _MYMALLOC_H
 

static char myblock[4096];
#define malloc(x) mymalloc(x, __LINE__, __FILE__)
struct metablock *start= (void *) myblock;
void* mymalloc(int memory, int linenum, char* filename);
 
#define free(x) myfree(x, __LINE__, __FILE__)
void* myfree(void*, int, char*);
 
#endif

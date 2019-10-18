all: mymalloc.o memgrind.c 

mymalloc.o: mymalloc.c
		gcc -c mymalloc.c

memgrind.c: mymalloc.o 
		gcc memgrind.c mymalloc.o 

clean:
	rm a.out; rm mymalloc.o


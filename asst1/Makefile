all: mymalloc.o memgrind.c 

mymalloc.o: mymalloc.c
		gcc -c mymalloc.c

memgrind.c: mymalloc.o 
		gcc -o memgrind memgrind.c mymalloc.o 

clean:
	rm memgrind; rm mymalloc.o


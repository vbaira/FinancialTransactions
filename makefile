OBJS = main.o HashTable.o BucketChain.o Graph.o Stack.o
SOURCE = main.c HashTable.c BucketChain.c Graph.c Stack.c
HEADER = HashTable.h BucketChain.h Graph.h Stack.h
OUT = elegxos
CC = gcc
FLAGS = -g -c


$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@ 


main.o: main.c
	$(CC) $(FLAGS) main.c

HashTable.o: HashTable.c
	$(CC) $(FLAGS) HashTable.c

BuckeChain.o: BucketChain.c
	$(CC) $(FLAGS) BucketChain.c

Graph.o: Graph.c
	$(CC) $(FLAGS) Graph.c

Stack.o:Stack.c
	$(CC) $(FLAGS) Stack.c




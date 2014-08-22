all: main

main:
	gcc -o ssh cscd340s14hw3.c cscd340s14hw3.h shellutil.c shellutil.h linkedList.c linkedList.h

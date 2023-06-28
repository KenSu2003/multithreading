CC = gcc
# HEADER = include/header.h
CFLAGS = -I.

# all: main

# main

ken_su_make: ken_su_threads.o 
	$(CC) -o ken_su_make ken_su_threads.o -l pthread
	
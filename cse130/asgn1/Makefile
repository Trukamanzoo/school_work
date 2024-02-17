# Make File

# Truman Vu, tvu8, pa1
#------------------------------------------------------------------------------
# Makefile for CSE 103 Programming Assignment 1
#
# make                   makes memory
# make hello	         makes memory
# make clean             removes all binaries
# make format		 	 format in clang-format
#------------------------------------------------------------------------------

CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all : memory format

memory : memory.o
	$(CC) -o memory memory.o

memory.o : memory.c 
	$(CC) $(CFLAGS) -c memory.c
 
clean :
	rm -f memory memory.o 
	
format :
	clang-format -i -style=file *.[ch]	
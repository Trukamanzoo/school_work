# Make File

# Truman Vu, tvu8, pa1
#------------------------------------------------------------------------------
# Makefile for CSE 101 Programming Assignment 1
#
# make                   makes hello
# make hello			 makes hello
# make clean             removes all binaries
# make format		 format in clang-format
#------------------------------------------------------------------------------

all : hello

hello : hello.o
	gcc -std=c17 -Wall -o hello hello.o

hello.o : hello.c 
	gcc -std=c17 -Wall -c hello.c

clean :
	rm -f hello hello.o 
	
format :
	clang-format -i -style=file *.[ch]
	


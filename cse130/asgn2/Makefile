# Make File

# Truman Vu, tvu8, pa1
#------------------------------------------------------------------------------
# Makefile for CSE 103 Programming Assignment 1
#
# make                   makes memory
# make httpserver	         makes memory
# make clean             removes all binaries
# make format		 	 format in clang-format
#------------------------------------------------------------------------------

CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all : httpserver format

new : new.o asgn2_helper_funcs.a
	$(CC) -o new new.o asgn2_helper_funcs.a

new.o : new.c
	$(CC) $(CFLAGS) -c new.c

httpserver : httpserver.o asgn2_helper_funcs.a
	$(CC) -o httpserver httpserver.o asgn2_helper_funcs.a


httpserver.o : httpserver.c
	$(CC) $(CFLAGS) -c httpserver.c

clean :
	rm -f httpserver new new.o httpserver.o
	
format :
	clang-format -i -style=file *.[ch]

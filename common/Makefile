# Makefile for common
#   Builds and compiles all these utility functions into a library.
#
# Rehoboth Okorie Feb 3 2022

# object files, and the target library
OBJS = pagedir.o word.o index.o
LIB = common.a
LIBS = ../libcs50/libcs50-given.a
FLAGS = -lm
INDEXCOEFF = -DINDEXCOEFF
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TEST) $(FLAGS) -I../libcs50/
CC = gcc
TEST = 
MAKE = make

# Build $(LIB) by archiving object files
# (you will need to drop in copy of ????)
$(LIB): $(OBJS) $(LIBS)
	ar cr $(LIB) $(OBJS)


pagedir.o: pagedir.c pagedir.h
index.o: index.c index.h word.o
word.o: word.c word.h

all: $(LIB)

valgrind:
	clear; make clean; make;
	# $(VALGRIND)
	
.PHONY: all clean test valgrind


# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o

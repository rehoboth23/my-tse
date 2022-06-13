# Makefile for indexer
#   Builds and compiles all these utility functions into a library.
#
# Rehoboth Okorie Feb 3 2022

# object files, and the target library
OBJS = indexer.o
TESTOBJS = indextest.o
LIBS = ../common/common.a ../libcs50/libcs50-given.a
FLAGS =
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TEST) $(FLAGS) -I../libcs50/ -I../common
CC = gcc
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: indexer indextest

# Build indexer
indexer: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

# Build indexerTest
indextest: $(TESTOBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

# Dependencies: object files depend on header files
indexer.o: indexer.c
indextest.o: indextest.c


test:
	@export TEST=-DTEST
	@clear; make clean; make	
	@make text_inner
	@export TEST=

text_inner: indexer
	@clear
	@echo TESTING indexer
	@./testing.sh

valgrind:
	@export TEST=-DTEST
	@clear; make clean; make
	@make valgrind_inner
	@export TEST=

valgrind_inner: indexer
	@clear;
	@echo TESTING indexer with valgrind
	@./testing.sh "$(VALGRIND)"
	
.PHONY: all clean test valgrind

# clean up after our compilation
clean:
	rm -f core
	rm -f $(OBJS) $(TESTOBJS) *~ *.o indexer indextest

# Makefile for querier
#   Builds and compiles all these utility functions into a library.
#
# Rehoboth Okorie Feb 3 2022

# object files, and the target library
OBJS = querier.o
LIBS = ../common/common.a ../libcs50/libcs50-given.a
FLAGS =
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TEST) $(FLAGS) -I../libcs50/ -I../common
CC = gcc
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

    # this    is a    mesy   input    

all: querier 

# Dependencies: object files depend on header files
querier.o: querier.c

# Build querier
querier: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

test:
	@clear; make clean; make	
	@make text_inner

text_inner: querier
	@clear
	@echo TESTING querier
	@./testing.sh

valgrind:
	@clear; make clean; make
	@make valgrind_inner

valgrind_inner: querier
	@clear;
	@echo TESTING querier with valgrind
	@./testing.sh "$(VALGRIND)"
	
.PHONY: all clean test valgrind

# clean up after our compilation
clean:
	rm -f core
	rm -f $(OBJS) $(TESTOBJS) *~ *.o querier

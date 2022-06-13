# Makefile for crawler
#   Builds and compiles all these utility functions into a library.
#
# Rehoboth Okorie Feb 3 2022

# object files, and the target library
OBJS = crawler.o
LIBS = ../libcs50/libcs50-given.a ../common/common.a
FLAGS =
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TEST) $(FLAGS) -I../libcs50/ -I../common
CC = gcc
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

# Build crawler
crawler: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

# Dependencies: object files depend on header files
crawler.o: crawler.c

test:
	@export TEST=-DTEST
	@clear; make clean; make
	@echo TESTING crawler
	@./testing.sh
	@export TEST=
	

all: crawler

valgrind:
	@export TEST=-DTEST
	@clear; make clean; make;
	@echo TESTING crawler with valgrind
	@./testing.sh "$(VALGRIND)"
	@export TEST=
	
.PHONY: all clean test valgrind

# clean up after our compilation
clean:
	rm -f core
	rm -f $(OBJS) *~ *.o crawler

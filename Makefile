
CXX = g++
CC = gcc
CFLAGS = -Wall -g
CXXFLAGS = -Wall -std=c++11

C_SOURCE = trees.c rb_example.c treap_example.c bst_example.c avl_example.c
CXX_SOURCE = benchmark.cpp

OBJECTS = $(C_SOURCE:.c=.o) $(CXX_SOURCE:.cpp=.o) 

all: benchmark bst_example rb_example treap_example avl_example onlyexec

objects:
	$(CC) -c $(CFLAGS) $(C_SOURCE)
	$(CXX) -c $(CXXFLAGS) $(CXX_SOURCE)

benchmark: benchmark.o trees.o
	$(CXX) $^ -o $@ 

bst_example: bst_example.o trees.o 
	$(CC) $^ -o $@

rb_example: rb_example.o trees.o 
	$(CC) $^ -o $@

treap_example: treap_example.o trees.o 
	$(CC) $^ -o $@

avl_example: avl_example.o trees.o 
	$(CC) $^ -o $@

.PHONY: clean
clean:
	rm -f *.o benchmark bst_example rb_example treap_example avl_example 
onlyexec:
	rm *.o

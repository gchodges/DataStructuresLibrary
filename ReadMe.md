# Data Structures Library

This is a C++ data structures library that I have created in order to become a more proficient C++ programmer and to learn to manipulate data.

## Red Black Tree

The Red Black Tree is found in the RBTree.cpp file in the Structures Directory. This file contains a self-balancing Red-Black Binary Search tree that supports insertion, deletion, rank, and can produce different tree traversals to the console.

## Circular Dynamic Array

The Circular Dynamic Array is a data structure that supports efficient insertion and deletion from both ends. This may also be called a double-ended queue. I have also added a mergesort algorithm to sort the array in O(nlogn) time as well as the quickselect algorithm

## Binomial Heap

The binomial heap is a type of priority queue that stores a list of pointers to the roots of binomial tree. Binomial trees have order k, which is created by a tree of order k-1 as the left child of the root of another tree of order k-1. This list of  pointers is contained in a Circular Dynamic Array. Binomial heaps allow for efficient insertion, removal, peek-min, and combination of two heaps. In this implementation, I have created a min-heap. This can be expanded in the future to assist graph traversal algorithms, Huffman encoding, or any other scenarios a priority queue may be used.
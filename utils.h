/*
   utils.c, .h: module for helping process strings and other
    simple functions.
   Part of Assignment 1: using linked list for dictionary
   Written by Anh Vo for COMP20003 as a sample solution
      for Assignment 1, Semester 2 2024
   Last updated: 30/07/2024
*/
#ifndef _UTILS_H_
#define _UTILS_H_
// common #include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

// common defs
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define MAX_STR_LEN 127   // expected max length of an input string
#define MAX_BUF_LEN 511   // expected max length of an input line

/* Number of bits in a single character. */
#define BITS_PER_BYTE 8

// dynamic memory tools
void *myMalloc(size_t size);
void *myCalloc(size_t n, size_t size);
void *myRealloc(void *curr, size_t size);

// FILE tools
FILE *myFopen(const char *fname, const char *mode);

/*--------- some useful string functions  --------------*/
char *myStrdup(char *);

// get string field from well-formatted .csv fileName
// cut-off and ignore from MAX_STR_LEN-th char if applicable
char *getCsvString(FILE *f);

// get an input string (ended with \n or \r) from file *f
// cut-off and ignore from MAX_BUF_LEN-th char if applicable
char *getString(FILE *f);


/*--------- errors & warning  ---------- --------------*/
void error(char *);
void warning(char *);



/* Helper function. Gets the bit at bitIndex from the string s. */
static int getBit(char *s, unsigned int bitIndex);

/* Allocates new memory to hold the numBits specified and fills the allocated
    memory with the numBits specified starting from the startBit of the oldKey
    array of bytes. */
char *createStem(char *oldKey, unsigned int startBit, unsigned int numBits);

/* Returns min of 3 integers 
    reference: https://www.geeksforgeeks.org/edit-distance-in-c/ */
int min(int a, int b, int c);

/* Returns the edit distance of two strings
    reference: https://www.geeksforgeeks.org/edit-distance-in-c/ */
int editDistance(char *str1, char *str2, int n, int m);

/* TRACING */
#if 0
#define TRACE printf("TRACE: file %s funct %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
#define TRACEMSG(X) printf("%s: file %s funct %s line %d\n", X,  __FILE__, __FUNCTION__, __LINE__);
#endif

#endif

/*
   linkedList.c, .h: module for working with a linked list
   Part of Assignment 1: using linked list for dictionary
	 Written by Anh Vo for COMP20003 as a sample solution
	    for Assignment 1, Semester 2 2024
	 Last updated: 31/07/2024
*/

#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include "data.h"

typedef struct llist llist_t;

// create & return an empty list
void *llistCreate();

// insert a single data to the end of list ll
//     return number of records inserted (1 or 0)
int llistInsert(void *ll, void *data);

// remove all record sharing "key",
//     return number of records removed
int llistDelete(void *ll, void *key);

// print all detailed nodes to f, using "labels" for data field label
void llistPrint(void *ll, char **labels, FILE *f);

// find all nodes in "source" having "key" and add them to the end
//      of "target", in appearance order
//      return number of found data
int llistSearch(void *source, void *target, void *key);

// free "list", also applying dataFree() if not NULL)
void llistFree(void *ll, void (*dataFree)(void *data));

// output list *ll in csv format
void llistCsvOutput(void *ll, FILE *f);

#endif

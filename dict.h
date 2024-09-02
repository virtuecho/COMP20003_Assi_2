/*
    dict.c, .h: module for working with a dictionary
                (as an abstract data type)
    Part of Assignment 1: using linked list for dictionary
    Written by Anh Vo for COMP20003 as a sample solution
       for Assignment 1, Semester 2 2024
    Last updated: 31/07/2024
*/

#ifndef _DICT_H_
#define _DICT_H_

// common defs for all dict
typedef enum {
  LOWER_BOUND = 0, 
  LINKED_LIST = 1, 
  // By default, enum values take the value preceeding
  //  plus one.
  UPPER_BOUND
} concreteDS_t;

typedef struct dict dict_t;

// create empty dict (with empty collection)
dict_t *dictCreate(concreteDS_t dsType, int isMain, char **labels);

// return an empty dict with the same structure
dict_t *dictCopyStructure(dict_t *dict);

// print whole dict
void dictPrint(dict_t *dict, FILE *f);

// free all memory of dict, including data memory if isMain is true
void dictFree(dict_t *dict);

// insert a single data to dict,
//     return number of records inserted (1 or 0)
int dictInsert(dict_t *dict, void *data);

// remove all record sharing "key",
//     return number of records removed
int dictDelete(dict_t *dict, void *key);

// search for (possibly duplicated) key in dictinary,
//     append found records to file "result"
//     return the number of found record
int dictSearch(void *key, dict_t *dict, dict_t *result);

// write dictionary to a file in the CSV format
void dictOutputCsvFile(dict_t *dict, char *fname);

#endif

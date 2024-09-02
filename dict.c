/*
   dict.c, .h: module for working with a dictionary
               (as an abstract data type)
   Part of Assignment 1: using linked list for dictionary
	 Written by Anh Vo for COMP20003 as a sample solution
	    for Assignment 1, Semester 2 2024
	 Last updated: 31/07/2024
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "utils.h"
#include "dict.h"
#include "linkedList.h"        // supporting concrete data structure

/* NOTE: current version is only working with linked list */

struct dict {
  concreteDS_t dsType;    // Underlying data structure
  char **labels;          // NULL-terminated sequence of data column labels
  void *coll;	            // data structure holding all records of the collection
	int isMain;             // = 1 if this is the main dict that holds actual data

  // function pointers for operations with sepific data structure
  //     to create, free, and modify the coresponding collection
  void *((*create)(void));  // create empty collection
  int (*insert)(void *coll, void *data);  // insert 1 data into coll
  int (*delete)(void *coll, void *key);   // delete all keys from coll
  int (*search)(void *key, void *sourceColl, void *targetColl);
                          // search for key in sourceColl
                          // append found records to targetColl
  void (*print)(void *coll, char **labels, FILE *f);
                          // print whole collection to FILE *f
  void (*csvOutput)(void *coll, FILE *f);  // output coll in csv format
  void (*free)(void *coll, void (*dataFree)(void *data));
};

/*----- Helper functions, using only in this .c file   -----*/
// build new empty dict with the same structure as "template"
dict_t *dictCreateFromTemplate(dict_t *template);


/*----- implementation of all funtions -----*/

// create empty dict (with empty collection)
dict_t *dictCreate(concreteDS_t dsType, int isMain, char **labels) {
  dict_t *dict = myMalloc(sizeof(*dict));
  dict->dsType = dsType;
  dict->labels = labels;
  dict->isMain = isMain;

  // bind operations
  switch(dsType) {
    case LINKED_LIST:
      dict->create = llistCreate;
      dict->insert = llistInsert;
      dict->search = llistSearch;
      dict->delete = llistDelete;
      dict->print = llistPrint;
      dict->free = llistFree;
      dict->csvOutput = llistCsvOutput;
      break;
    default:
      error("dictCreate: Required data structure not yet implemented");
  }

  // create empty collection
  dict->coll = dict->create();

  return dict;
}


// print whole dict
void dictPrint(dict_t *dict, FILE *f) {
  assert(dict && f && dict->coll);
  dict->print(dict->coll, dict->labels, f);
}

// free all memory of dict, including data memory if isMain is true
void dictFree(dict_t *dict) {
  if (dict->isMain) {
    if (dict->labels) {
      for (int i = 0; dict->labels[i]; i++) {
        free(dict->labels[i]);
      }
      free(dict->labels);
    }
    if (dict->coll) {
      dict->free(dict->coll, dataFree);
    }
  } else {
    if (dict->coll) {
      dict->free(dict->coll, NULL);
    }
  }
  free(dict);
}

// insert, search, deletion in  dictionary, returning SUCSESS/FAILURE
//        returning the number of inserted records
int dictInsert(dict_t *dict, void *data) {
  return dict->insert(dict->coll, data);
}

// remove from dict all records that have "key",
//        returning the mnuber of removed records
int dictDelete(dict_t *dict, void *key) {
  return dict->delete(dict->coll, key);
}

// search for (possibly duplicated) key in dictinary,
//     append found records to file "result"
//     return the number of found record
int dictSearch(void *key, dict_t *dict, dict_t *result) {
  return dict->search(key, dict->coll, result->coll);
}


// return an empty dict with the same structure
dict_t *dictCopyStructure(dict_t *dict) {
  dict_t *copy = myMalloc(sizeof(*dict));
  *copy = *dict;
  copy->isMain = FALSE;
  copy->coll = copy->create();
  return copy;
}


// write dictionary to a file in the CSV format
void dictOutputCsvFile(dict_t *dict, char *fname) {
  FILE *f = myFopen(fname, "w");
  for (int i = 0; dict->labels[i]; i++) {
    fprintf(f, "%s", dict->labels[i]);
    if (dict->labels[i + 1]) {
      fprintf(f, ",");
     } else {
      fprintf(f, "\n");
    }
  }
  dict->csvOutput(dict->coll, f);
  fclose(f);
}

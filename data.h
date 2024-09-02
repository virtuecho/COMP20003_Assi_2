/*
   data.c, .h: module for working with a CSV file of known structure
   Part of Assignment 1: using linked list for dictionary
   Written by Anh Vo for COMP20003 as a sample solution
      for Assignment 1, Semester 2 2024
   Last updated: 31/07/2024
*/

#ifndef _DATA_H_
#define _DATA_H_

// Specific defs for this data set
#define COLS 10        // expected number of columms

typedef struct header header_t;
typedef struct data data_t;

// read and parse the csv header line from file f
char **dataGetLabels(FILE *f);

// read and parse a csv line from file f, returns the resulted data record
data_t *dataGetLine(FILE *f);

// print a data record to file f, using labels[] as attribute labels
void dataPrint(void *data, char **labels, FILE *f);

// free memory associated with "data" pointer
void dataFree(void *data);

// compare data with a key
int dataKeyCmp(void *data, void *key, int *comps);

// output a data as a CSV line
void dataCsvOutput(void *v, FILE *f);


#endif

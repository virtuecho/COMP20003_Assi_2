/*
   data.c, .h: module for working with a CSV file of known structure
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
#include "data.h"

// struct of CSV records
struct data {
  int recId;           // record ID in the full set of data
  int suburbCode;
  char *suburbName;
  int year;
  char *stateCode;
  char *stateName;
  char *govCode;
  char *govName;
  double latitude;
  double longitude;
};

// read and parse the csv header line from file f
//    assuming always successful
//    reurn: NULL-terminated array of strings
char **dataGetLabels(FILE *f) {
  char **labels = myCalloc(COLS + 1, sizeof(*labels));
  for (int i = 0; i < COLS; i++) {
    labels[i] = getCsvString(f);
  }
  return labels;
}


// read and parse a csv line from file f,
//    returns pointer to the resulted data record
//    or NULL if on EOF
data_t *dataGetLine(FILE *f) {
  while(1) {
    int recId;
    if (fscanf(f, "%d , ", &recId) != 1) {
      return NULL;   /* return on EOF */
    }
    data_t *data = myMalloc(sizeof(*data));
    data->recId = recId;
    if (fscanf(f, "%d , ", &data->suburbCode) != 1) {
      //NOTE: all "if"s in this function are for safeguarding data
      warning("dataGetLine: error in reading suburbCode");
      fprintf(stderr, "RecID = %d ignored\n", recId);
      // data not well-formatted, discard the line and try the next one
      for (int c = fgetc(f); c != '\n' && c != '\r'; c = fgetc(f)){
        //   ( '\r' is <end-of-line> for some text files )
      }
        
      dataFree(data);
      continue;
    }
    data->suburbName = getCsvString(f);
    if (fscanf(f, "%d , ", &data->year) != 1){
      warning("dataGetLine: error in reading year");
      fprintf(stderr, "RecID = %d ignored\n", recId);
      for (int c = fgetc(f); c != '\n' && c != '\r'; c = fgetc(f)){

      }
      dataFree(data);
      continue;
    }
    data->stateCode = getCsvString(f);
    data->stateName = getCsvString(f);
    data->govCode = getCsvString(f);
    data->govName = getCsvString(f);
    if (fscanf(f, "%lf ,%lf ", &data->latitude, &data->longitude) != 2) {
      warning("dataGetLine: error in reading latitude/longitude");
      fprintf(stderr, "RecID = %d ignored\n", recId);
      for (int c = fgetc(f); c != '\n' && c != '\r'; c = fgetc(f)){

      }
      dataFree(data);
      continue;

    }
    return data;
  }
}

// print a data record to file f, using labels[] as attribute labels
void dataPrint(void *vdata, char **labels, FILE *f) {
  assert(vdata);
  data_t *data = vdata;
  int i;

  for (i = 0; i < COLS ; i++) {
    if (labels && labels[i]) {
      fprintf(f, "%s: ", labels[i]);
    }
    switch (i) {
      case  0: 
        fprintf(f, "%d, ", data->recId); 
        break;
      case  1: 
        fprintf(f, "%d, ", data->suburbCode); 
        break;
      case  2: 
        fprintf(f, "%s, ", data->suburbName); 
        break;
      case  3: 
        fprintf(f, "%d, ", data->year); 
        break;
      case  4: 
        fprintf(f, "%s, ", data->stateCode); 
        break;
      case  5: 
        fprintf(f, "%s, ", data->stateName); 
        break;
      case  6: 
        fprintf(f, "%s, ", data->govCode); 
        break;
      case  7: 
        fprintf(f, "%s, ", data->govName); 
        break;
      case  8: 
        fprintf(f, "%.7lf, ", data->latitude); 
        break;
      case  9: 
        fprintf(f, "%.7lf\n", data->longitude); 
        break;
    }
  }
}

// free memory associated with "data" pointer
void dataFree(void *vdata) {
  assert(vdata);
  data_t *data = vdata;
  // NOTE: the "if"s are only helpful when cleaning data
  if (data->suburbName) {
    free(data->suburbName);
  }
  if (data->stateCode) {
    free(data->stateCode);
  }
  if (data->stateName) {
    free(data->stateName);
  }
  if (data->govCode) {
    free(data->govCode);
  }
  if (data->govName) {
    free(data->govName);
  }

  free(data);
}

// compare data with a key, counts character comparisons
int dataKeyCmp(void *vdata, void *key, int *comps) {
  data_t *data = vdata;
  char *s1 = data->suburbName, *s2 = key;
  int n1 = strlen(s1), n2 = strlen(s2);

  if (comps == NULL) {
    return strcmp(s1, s2);
  }

  int i = 0;
  while (i <= n1 && i <= n2) { // <= to include nullbyte
    (*comps)++;
    if (s1[i] > s2[i]) {
      return 1;
    }
    else if (s1[i] < s2[i]) {
      return -1;
    }
    i++;
  }

  return 0;
}

// output a data as a CSV line
void dataCsvOutput(void *v, FILE *f) {
  data_t *data= v;
  int i;
  for (i=0; i<COLS; i++) {
    char *str=NULL;
    switch (i) {
      case  0: 
        fprintf(f, "%d,", data->recId); 
        break;
      case  1: 
        fprintf(f, "%d,", data->suburbCode); 
        break;
      case  2: 
        str = data->suburbName; 
        break;
      case  3: 
        fprintf(f, "%d,", data->year); 
        break;
      case  4: 
        str = data->stateCode; 
        break;
      case  5: 
        str = data->stateName; 
        break;
      case  6: 
        str = data->govCode; 
        break;
      case  7: 
        str = data->govName; 
        break;
      case  8: 
        fprintf(f, "%.7lf,", data->latitude); 
        break;
      case  9: 
        fprintf(f, "%.7lf\n", data->longitude); 
        break;
    }
    if (str) {
      // if a field contains commas, it should start and end with "
      if (strchr(str,',')) {
        fprintf(f,"\"%s\",", str);
      } else {
        fprintf(f,"%s,", str);
      }
    }
  }
}

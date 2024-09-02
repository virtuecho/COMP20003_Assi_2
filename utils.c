/*
   utils.c, .h: module for helping process strings and other
    simple functions.
   Part of Assignment 1: using linked list for dictionary
   Written by Anh Vo for COMP20003 as a sample solution
      for Assignment 1, Semester 2 2024
   Last updated: 30/07/2024
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "utils.h"


// dynamic memory tools
void *myMalloc(size_t size){
    assert(size > 0);
    void *p = malloc(size);
    assert(p);
    return p;
}

void *myCalloc(size_t n, size_t size){
    assert(size > 0);
    void *p = calloc(n, size);
    assert(p);
    return p;
}

void *myRealloc(void *curr, size_t size) {
    assert(size > 0);
    curr = realloc(curr, size);
    assert(curr);
    return curr;
}


// FILE tools
FILE *myFopen(const char *fname, const char *mode) {
    FILE *f = fopen(fname, mode);
    assert(f);
    return f;
}

/*--------- some useful string functions  --------------*/
char *myStrdup(char *source) {
    char *target = strdup(source);
    assert(source);
    return target;
}

// get string until encountering a comma or newline
// cut-off and ignore from maxLength-th char if applicable
char *getCsvString(FILE *f) {
    int ch;
    char scanch;
    if (fscanf(f, " %c", &scanch) != 1) {
        return NULL;
    }
    ch = scanch;
    int i;
    int tooLong = FALSE;
    char *s = myMalloc((MAX_STR_LEN + 1) * sizeof(*s));
    char *p = s;
    if (ch == '\"') {
        for (i = 0; (ch = fgetc(f)) != EOF
                   && ch != '\"' 
                   && ch != '\n' 
                   && ch != '\r'; i++) {
            if (i < MAX_STR_LEN){
                *(p++) = ch;
            } else {
                tooLong = TRUE;
            }
        }
        if (ch == '\"') {
            ch = fgetc(f);   // consume comma (or \n)
        }
    } else if (ch != EOF && ch != ',' && ch != '\n' && ch != '\r') {
                                       // also account for empty string
        *(p++) = ch;
        for (i = 1; (ch = fgetc(f)) != EOF 
            && ch != ','
            && ch != '\n'
            && ch != '\r'; i++) {
            if (i < MAX_STR_LEN){
                *(p++) = ch;
            } else {
                tooLong = TRUE;
            }
        }
    }
    if (tooLong) {
        warning("getCsvString: too-long string chopped");
    }
    // chop spaces at the end [not executed for well-formated datafile]
    for (p--; p >= s && isspace(*p); p--){

    };
    *(p + 1) = '\0';

    s = myRealloc(s, (strlen(s) + 1) * sizeof(*s));  // shrink s to its size
    return s;
}

// get an input string from file *f until encountering a newline
// cut-off and ignore from MAX_BUF_LEN-th char if applicable
char *getString(FILE *f) {
    char ch;
    if (fscanf(f, " %c", &ch) != 1) {
        return NULL;
    }
    int i;
    int tooLong = FALSE;
    char *s = myMalloc((MAX_BUF_LEN + 1) * sizeof(*s));
    char *p = s;
    *(p++) = ch;
    for (i = 1; (ch = fgetc(f)) != EOF && ch != '\n' && ch != '\r' ; i++) {
            if (i < MAX_STR_LEN) {
                *(p++) = ch;
            } else {
                tooLong = TRUE;
            }
    }
    if (tooLong)
        warning("getCsvString: too-long string chopped");
    *p = '\0';
    s = myRealloc(s, (strlen(s) + 1) * sizeof(*s));  // shrink s to its size
    return s;
}




/*--------- errors & warning  ---------- --------------*/
void error(char *message) {
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

void warning(char *message) {
    fprintf(stderr, "%s\n", message);
    return;
}

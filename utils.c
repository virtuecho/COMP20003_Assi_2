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


/* Helper function. Gets the bit at bitIndex from the string s. */
static int getBit(char *s, unsigned int bitIndex){
    assert(s && bitIndex >= 0);
    unsigned int byte = bitIndex / BITS_PER_BYTE;
    unsigned int indexFromLeft = bitIndex % BITS_PER_BYTE;
    /* 
        Since we split from the highest order bit first, the bit we are interested
        will be the highest order bit, rather than a bit that occurs at the end of the
        number. 
    */
    unsigned int offset = (BITS_PER_BYTE - (indexFromLeft) - 1) % BITS_PER_BYTE;
    unsigned char byteOfInterest = s[byte];
    unsigned int offsetMask = (1 << offset);
    unsigned int maskedByte = (byteOfInterest & offsetMask);
    /*
        The masked byte will still have the bit in its original position, to return
        either 0 or 1, we need to move the bit to the lowest order bit in the number.
    */
    unsigned int bitOnly = maskedByte >> offset;
    return bitOnly;
}

/* Allocates new memory to hold the numBits specified and fills the allocated
    memory with the numBits specified starting from the startBit of the oldKey
    array of bytes. */
char *createStem(char *oldKey, unsigned int startBit, unsigned int numBits){
    assert(numBits > 0 && startBit >= 0 && oldKey);
    int extraBytes = 0;
    if((numBits % BITS_PER_BYTE) > 0){
        extraBytes = 1;
    }
    int totalBytes = (numBits / BITS_PER_BYTE) + extraBytes;
    char *newStem = malloc(sizeof(char) * totalBytes);
    assert(newStem);
    for(unsigned int i = 0; i < totalBytes; i++){
        newStem[i] = 0;
    }
    for(unsigned int i = 0; i < numBits; i++){
        unsigned int indexFromLeft = i % BITS_PER_BYTE;
        unsigned int offset = (BITS_PER_BYTE - indexFromLeft - 1) % BITS_PER_BYTE;
        unsigned int bitMaskForPosition = 1 << offset;
        unsigned int bitValueAtPosition = getBit(oldKey, startBit + i);
        unsigned int byteInNewStem = i / BITS_PER_BYTE;
        newStem[byteInNewStem] |= bitMaskForPosition * bitValueAtPosition;
    }
    return newStem;
}

/* Returns min of 3 integers 
    reference: https://www.geeksforgeeks.org/edit-distance-in-c/ */
int min(int a, int b, int c) {
    if (a < b) {
        if(a < c) {
            return a;
        } else {
            return c;
        }
    } else {
        if(b < c) {
            return b;
        } else {
            return c;
        }
    }
}

/* Returns the edit distance of two strings
    reference: https://www.geeksforgeeks.org/edit-distance-in-c/ */
int editDistance(char *str1, char *str2, int n, int m){
    assert(m >= 0 && n >= 0 && (str1 || m == 0) && (str2 || n == 0));
    // Declare a 2D array to store the dynamic programming
    // table
    int dp[n + 1][m + 1];

    // Initialize the dp table
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            // If the first string is empty, the only option
            // is to insert all characters of the second
            // string
            if (i == 0) {
                dp[i][j] = j;
            }
            // If the second string is empty, the only
            // option is to remove all characters of the
            // first string
            else if (j == 0) {
                dp[i][j] = i;
            }
            // If the last characters are the same, no
            // modification is necessary to the string.
            else if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = min(1 + dp[i - 1][j], 1 + dp[i][j - 1],
                    dp[i - 1][j - 1]);
            }
            // If the last characters are different,
            // consider all three operations and find the
            // minimum
            else {
                dp[i][j] = 1 + min(dp[i - 1][j], dp[i][j - 1],
                    dp[i - 1][j - 1]);
            }
        }
    }

    // Return the result from the dynamic programming table
    return dp[n][m];
}
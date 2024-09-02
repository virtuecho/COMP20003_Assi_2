/*
   linkedList.c, .h: module for working with a linked list
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
#include "linkedList.h"


typedef struct lnode lnode_t;
struct lnode {
    void *data;       // node contains pointer to actual data (of data_t type)
    lnode_t *next;
};

// A linked ll is defined as a couple of pointers
struct llist {
    lnode_t *head;
    lnode_t *tail;
    size_t length;               // Number of elements in the ll
};


// create & return an empty ll
void *llistCreate() {
    llist_t *ll = myMalloc(sizeof(*ll));
    ll->head = NULL;
    ll->tail = NULL;
    ll->length = 0;
    return ll;
}

// free "ll", also applying dataFree() if not NULL)
void llistFree(void *vll, void (*dataFree)(void *)){
    llist_t *ll = vll;
    assert(ll != NULL);
    lnode_t *curr = ll->head;
    while (curr) {
        lnode_t *tmp = curr;
        curr = curr->next;
        if (dataFree) {
            dataFree(tmp->data);
        }
        free(tmp);
    }
    free(ll);
}


// Returns the number of elements in ll
int llistCount(llist_t *ll){
    assert(ll);
    return ll->length;
}

// insert a single data to the end of ll ll
//     return number of records inserted (1 or 0)
int llistInsert(void *vll, void *data) {
    llist_t *ll = vll;
    assert(ll);
    // Creates a new lnode and set data to provided value
    lnode_t *new = myMalloc(sizeof(*new));

    new->data = data;
    new->next = NULL;
    if (ll->head == NULL) {     // original list is empty, update both head and tail
        ll->head = new;
        ll->tail = new;
    } else {                    // otherwise append node to the tail & update tail
        ll->tail->next = new;
        ll->tail = new;
    }

    // Updates the number of elements in the ll
    (ll->length)++;
    return 1;
}

// remove all record sharing "key",
//     return number of records removed
int llistDelete(void *vll, void *vkey) {
    llist_t *ll = vll;
    char *key = vkey;
    assert(ll && key);
    int removals = 0;
    if (ll->head == NULL) {
        return removals;  // original list is empty, no removal possible
    }

    lnode_t *curr = ll->head;        // current node
    lnode_t *prev = NULL;            // previous node

    while (curr != NULL) {
      lnode_t *next = curr->next;  // next node

      if (dataKeyCmp(curr->data, key, NULL) == 0) {  // key found, remove the node
        if (prev == NULL) {          // removed node is head node: update head
            ll->head = curr->next;
            if (ll->head == NULL) {  // list becomes empty after removing head
                ll->tail = NULL;
            }
        } else {                     // removed node is non-head, bypass it
            prev->next = curr->next;
        }

        if (curr == ll->tail) {      // removed node is tail node: update tail
            ll->tail = prev;
        }

        // free memory associated with the removed node
        dataFree(curr->data);  // Assuming data needs separate memory freeing
        free(curr);

        ll->length--;
        removals++;
      } else {   // key not found in this node, move to the next
          prev = curr;
      }

      curr = next;  // move to the next node
  }
    return removals;
}

// print all detailed nodes to f, using "labels" for data field label
void llistPrint(void *vll, char **labels, FILE *f) {
    llist_t *ll = vll;
    assert(ll);
    for (lnode_t *curr = ll->head; curr; curr = curr->next) {
        dataPrint(curr->data, labels, f);
    }
}

// find all nodes in "source" having "key" and add them to the end
//      of "target", in appearance order
//      return number of found data
int llistSearch(void *key, void *vsource, void *vtarget) {
    llist_t *source = vsource;
    llist_t *target = vtarget;
    assert(source && target && key);
    int matches = 0;
    for (lnode_t *curr = source->head; curr; curr = curr->next) {
        if (dataKeyCmp(curr->data, key, NULL) == 0) { // found matched node
            llistInsert(target, curr->data);
            matches++;
        }
    }
    return matches;
}

// output whole collection in csv format
void llistCsvOutput(void *vllist, FILE *f) {
    assert(vllist);
    llist_t *ll = vllist;
    for (lnode_t *p = ll->head; p; p = p->next) {
        dataCsvOutput(p->data, f);
    }
}

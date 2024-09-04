/*
   main.c
   driver for Assignment 1: using linked list for dictionary
                              operations supplied; insert, search, deleted
     Compile with:
            make
     Usage: to see usage, run:
           ./dict1

     Written by Anh Vo for COMP20003 as a sample solution
        for Assignment 1, Semester 2 2024
     Last updated: 30/07/2024
*/


#include "utils.h"
#include "dict.h"
#include "data.h"
#include "comparison_info.h"
#include "patricia_trie.h"

// valid tasks
typedef enum {
    LOWER_TASK = 0, // bound
    LL_SEARCH = 3,  // search-on-linked-list
    PATRICIA_SEARCH = 4, // search-on-patricia-trie
    LL_DELETE = 2,  // delete-on-linked-list
    // By default, enum values take the value preceeding
    //  plus one.
    UPPER_TASK = 5      // bound
} task_t;

#define ARGC 4

void processArgs(int argc, char *argv[], task_t *, char **, char **);
void batchSearch(dict_t *dict, char *outFileName, FILE *msgFile);
void batchDelete(dict_t *dict, char *outFileName, FILE *msgFile);

void batchSearch_patricia(patricia_node_t* root, char *outFileName, FILE *msgFile, char** labels);


int main(int argc, char *argv[]) {
    task_t task;
    char *inFileName, *outFileName;
    processArgs(argc, argv, &task, &inFileName, &outFileName);
    concreteDS_t dsType = LINKED_LIST;

    FILE *msgFile = stdout;
    // build linked list dictionary from csv file argv[2]
    FILE *inFile = myFopen(inFileName, "r");
    char **labels = dataGetLabels(inFile);            // read CSV header line
    dict_t *dict = dictCreate(dsType, TRUE, labels);  // create empty dict
    patricia_node_t* patricia_root = NULL;
    void *data = NULL;
    while ( (data = dataGetLine(inFile)) != NULL)  {
        dictInsert(dict, data);                        // build the dataset
        if (strcmp(argv[1], "4") == 0) {
            patricia_root = insert_patricia(patricia_root, ((data_t*)data)->suburbName, data);
        }
        
    }
    fclose(inFile);


    // do respective task
    switch (task) {
        case LL_SEARCH:
            batchSearch(dict, outFileName, msgFile);
            break;
        case PATRICIA_SEARCH:
            batchSearch_patricia(patricia_root, outFileName, msgFile, labels);
        case LL_DELETE:
            batchDelete(dict, outFileName, msgFile);
            break;
        default:
            error("main: Unrecognized Task");
    }
    // top-level cleaning
    dictFree(dict);
    free_all_patricia(patricia_root);
    return 0;
}

void batchSearch_patricia(patricia_node_t* root, char *outFileName, FILE *msgFile, char** labels) {
    FILE *outFile = myFopen(outFileName, "w");
    char *query = NULL;
    while ( (query = getString(stdin)) != NULL) {

        // perform a search, then output
        comparison_info_t compare_info = {0, 0, 0}; // Initiate comparison info
        data_t* matches = search_patricia(root, query, &compare_info);
        if (matches != NULL){
            fprintf(msgFile, "%s --> 1 records - comparisons: b%d n%d s%d\n", query,
            compare_info.bit_comparisons, compare_info.node_accesses, compare_info.string_comparisons);
            dataPrint(matches, labels, outFile);  // print matches to file
        } else {
            fprintf(msgFile, "%s --> NOTFOUND\n", query);
        }

        fprintf(outFile, "%s -->\n", query);
        free(query);
    }
    fclose(outFile);
}

// do multiple search on dict, output result after each search
// note: dict can be of any types (concrete data structures)
void batchSearch(dict_t *dict, char *outFileName, FILE *msgFile) {
    FILE *outFile = myFopen(outFileName, "w");
    char *query = NULL;
    while ( (query = getString(stdin)) != NULL) {
        // create an empty linked list dictionary for holding search output
        dict_t *outputDict = dictCopyStructure(dict);
        // perform a search, then output
        comparison_info_t compare_info = {0, 0, 0}; // Initiate comparison info
        int matches = dictSearch(query, dict, outputDict, &compare_info);
        if (matches){
            fprintf(msgFile, "%s --> %d records - comparisons: b%d n%d s%d\n", query, matches,
            compare_info.bit_comparisons, compare_info.node_accesses, compare_info.string_comparisons);
        } else {
            fprintf(msgFile, "%s --> NOTFOUND\n", query);
        }

        fprintf(outFile, "%s -->\n", query);
        dictPrint(outputDict, outFile);
        // cleaning after each query
        dictFree(outputDict);
        free(query);
    }
    fclose(outFile);
}

// do multiple record deletions on dict, output ID of the remaing records at the end
// note: dict can be of any types
void batchDelete(dict_t *dict, char *outFileName, FILE *msgFile) {
    char *query = NULL;
    int recordCount = 0, queryCount = 0;

    while ( (query = getString(stdin)) != NULL) {
        //printf("query = %s\n", query);
        int records = dictDelete(dict, query);
        if (records){
            fprintf(msgFile, "%s --> %d records deleted\n", query, records);
        } else {
            fprintf(msgFile, "%s --> NOTFOUND\n", query);
        }
        recordCount += records;
        queryCount++;
        free(query);
    }

    dictOutputCsvFile(dict, outFileName); // output remaining records
}

// check arguments of main()
void processArgs(int argc, char *argv[], task_t *task, char **inFileName, char **outFileName) {
    if (argc < ARGC || strtol(argv[1], NULL, 10) <= (int) LOWER_TASK
                    || strtol(argv[1], NULL, 10) >= (int) UPPER_TASK)  {
        fprintf(stderr, "\n    ---- USING A DICTIONARY OF AUSTRALIAN SUBURBS ----\n");
        fprintf(stderr, "This program perform search/delete operations on a dictionary.\n");
        fprintf(stderr, "The dictionary is built from a CSV file about Australian suburbs.\n\n");
        fprintf(stderr, "Usage: %s LOWER_TASK input_file output_file\n", argv[0]);
        fprintf(stderr, "       \t where:\n");
        fprintf(stderr, "       \t    - task is 1 for search, 2 for delete\n");
        fprintf(stderr, "       \t    - input_file: input CSV data file\n");
        fprintf(stderr, "       \t    - output_file: result output file\n");
        fprintf(stderr, "Notes on input queries:\n");
        fprintf(stderr, "       each query is a suburb name in a single line\n\n");
        exit(EXIT_FAILURE);
    }
    *task = strtol(argv[1], NULL, 10);
    *inFileName = argv[2];
    *outFileName = argv[3];
    return;
}

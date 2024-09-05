#ifndef _PATRICIA_TRIE_H_
#define _PATRICIA_TRIE_H_

#include "data.h"
#include "comparison_info.h"

typedef struct patricia_node patricia_node_t;

struct patricia_node {
	unsigned int prefix_bits; // how many bits of prefix
	char* prefix;
	patricia_node_t* branchA; // next bit is 0
	patricia_node_t* branchB; // nextbit is 1
	data_t* data;
};

// root should be NULL for creating a new patricia trie
patricia_node_t* insert_patricia(patricia_node_t* root, char* key, data_t* data);

// find mismatch bit, if all match, return all bits, else return the mismatch bit
unsigned int find_mismatch_bit(char* prefix, char* key, unsigned int prefix_bits);

// search by the key, if mismatch in key, return the closest match
data_t* search_patricia(patricia_node_t* root, char* key, comparison_info_t* compare_info);

// find the closest match, if strings have an equal distance, return alphabetically earliest
data_t* find_closest_match(patricia_node_t* node, char* key, comparison_info_t* compare_info);

void free_all_patricia(patricia_node_t* root);


#endif
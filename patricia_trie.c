#include "utils.h"
#include "patricia_trie.h"

// root should be NULL for creating a new patricia trie
patricia_node_t* insert_patricia(patricia_node_t* root, char* key, data_t* data) {
	
	// create a new patricia trie
	if (root == NULL) {
		patricia_node_t* node = malloc(sizeof(patricia_node_t));
		node->prefix_bits = strlen(key) * BITS_PER_BYTE;
		node->prefix = key;
		node->data = data;
		node->branchA = NULL;
		node->branchB = NULL;
		return node;
	}

	// check bits match
	unsigned int mismatch_bit = find_mismatch_bit(root->prefix, key, root->prefix_bits);

	// See if All Bits Match
	if (mismatch_bit == root->prefix_bits) {
		
		// See if All Bits Match, recursively
		unsigned int next_bit = getBit(key, mismatch_bit);
		if (next_bit == 0) {
			root->branchA = insert_patricia(root->branchA, key, data);
		} else {
			root->branchB = insert_patricia(root->branchB, key, data);
		}
	} else {
		// Not in Tree Already, split node
		patricia_node_t* new_node = malloc(sizeof(patricia_node_t));
		new_node->prefix_bits = mismatch_bit;
		new_node->prefix = createStem(key, 0, mismatch_bit);
		new_node->data = NULL; // no need to store data in split node

		// rearrange the Tree
		unsigned int next_bit = getBit(key, mismatch_bit);
		if (next_bit == 0) {
			new_node->branchA = insert_patricia(NULL, key, data);
			new_node->branchB = root;
		} else {
			new_node->branchA = root;
			new_node->branchB = insert_patricia(NULL, key, data);
		}

		return new_node;
	}

	return root; 
}

// find mismatch bit, if all match, return all bits, else return the mismatch bit
unsigned int find_mismatch_bit(char* prefix, char* key, unsigned int prefix_bits) {

	for (unsigned int i = 0; i < prefix_bits; i++) {
		if (getBit(prefix, i) != getBit(key, i)) {
			return i; // return mismatch bit
		}
		return prefix_bits; // all match
	}

}

// search by the key, if mismatch in key, return the closest match
data_t* search_patricia(patricia_node_t* root, char* key, comparison_info_t* compare_info) {
	
	if (root == NULL) {
		return NULL;
	}

	compare_info->node_accesses++; // enter a node

	// check bits match
	unsigned int mismatch_bit = find_mismatch_bit(root->prefix, key, root->prefix_bits);

	if (mismatch_bit == root->prefix_bits) {
		
		// see if All Bits Match, recursively
		unsigned int next_bit = getBit(key, mismatch_bit);
		compare_info->bit_comparisons++; // compare a bit

		// search branch A and B, recursively
		if (next_bit == 0) {
			data_t* result = search_patricia(root->branchA, key, compare_info);
			if (result != NULL) {
				return result;
			} else {
				return root->data; // return data in current node
			}
		} else {
			data_t* result = search_patricia(root->branchB, key, compare_info);
			if (result != NULL) {
				return result;
			} else {
				return root->data; // return data in current node
			}			
		}
		
	} else if (mismatch_bit == strlen(key) * BITS_PER_BYTE) {
		
		// found the key
		return root->data;
	} else {

		// not match, find the closest match
		return find_closest_match(root, key, compare_info);
	}
}

// find the closest match, if strings have an equal distance, return alphabetically earliest
data_t* find_closest_match(patricia_node_t* node, char* key, comparison_info_t* compare_info) {

	// calculate the distance of strings
	int distance = editDistance(node->prefix, key, (node->prefix_bits) / BITS_PER_BYTE, strlen(key));
	compare_info->string_comparisons++;

	data_t* closest_match = node->data; // for now this is the closest
	// find the closest in branches
	data_t* closest_A = find_closest_match(node->branchA, key, compare_info);
	data_t* closest_B = find_closest_match(node->branchB, key, compare_info);

	// calculate the distance of strings in branches
	int distance_A = editDistance(closest_A->suburbName, key, strlen(closest_A->suburbName), strlen(key));
	int distance_B = editDistance(closest_B->suburbName, key, strlen(closest_B->suburbName), strlen(key));

	// compare distance in branches, if equal, return alphabetically earliest
	if (distance_A < distance) {
		closest_match = closest_A;
	} else if (distance_A == distance) {
		if (strcmp(closest_A->suburbName, closest_match->suburbName) < 0) {
			closest_match = closest_A;
		}
	}

	if (distance_B < distance) {
		closest_match = closest_B;
	} else if (distance_B == distance) {
		if (strcmp(closest_B->suburbName, closest_match->suburbName) < 0) {
			closest_match = closest_B;
		}
	}
	
	return closest_match;
}

void free_all_patricia(patricia_node_t* root) {
	if (root == NULL) {
		return;
	}
	
	free_all_patricia(root->branchA);
	free_all_patricia(root->branchB);
	free(root->data);
	free(root->prefix);
	free(root);
}
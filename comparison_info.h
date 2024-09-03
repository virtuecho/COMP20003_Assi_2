#ifndef _COMPARISON_INFO_H_
#define _COMPARISON_INFO_H_

#include "data.h"

typedef struct comparison_info {
	int bit_comparisons;
	int node_accesses;
	int string_comparisons;
} comparison_info_t;

#endif
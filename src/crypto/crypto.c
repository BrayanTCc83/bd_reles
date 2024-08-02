#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "crypto.h"
#include "../utils/utils.h"

// K = Sum[ c ^ 2 ] * 10
#define ASCII_A_MAYUS 65
#define ASCII_A_MINUS 97
#define SHIFT_1 1
#define SQUARE 2
const int string_hash(const char *string) {
	char *c = (char*) string;
	int hash = 0;
	for(; *c; c++) {
		hash << SHIFT_1;
		hash += pow(*c, SQUARE);
	}
	return hash;
}

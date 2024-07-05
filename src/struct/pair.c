#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "../types/types.h"

extern const char *INSUFICIENT_MEMORY, *PAIR_TYPE;

pair_t *new_pair(const char *key, mdb_type_t *value) {
	pair_t *Pair = (pair_t*) malloc(sizeof(pair_t));

	if(Pair == NULL)
		PrintError(INSUFICIENT_MEMORY, PAIR_TYPE);

	Pair->o = PAIR;
	strcpy(Pair->key, key);
	Pair->value = value;
	return Pair;
}

compare_result_t compare_pair(pair_t *pair, const char *key) {
	return strcmp(pair->key, key);
}

pair_t *clone_pair(const pair_t pair) {
	return new_pair(pair.key, pair.value);
}

void delete_pair(pair_t *pair) {
	delete_mdb_type(pair->value);
	strcpy(pair->key, "");
	free(pair);
}

char *pair_to_string(pair_t instance) {
	char *string = (char*) malloc(STRINGIFY_OBJECT_SIZE);
	char *value = to_string(instance.value);
	sprintf(string, "<%s, %s>", instance.key, value);
	return string;
}

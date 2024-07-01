#include <stdlib.h>
#include <stdio.h>
#include "structs.h"
#include "../utils/utils.h"

extern const char*INSUFICIENT_MEMORY, *TUPLE_TYPE;

tuple_t *new_tuple(types_t type, void *value1, void *value2) {
	tuple_t *Tuple = (tuple_t*) malloc(sizeof(tuple_t));
	if(Tuple == NULL)
		PrintError(INSUFICIENT_MEMORY, TUPLE_TYPE);

	Tuple->type = type;
	Tuple->value1 = value1;
	Tuple->value2 = value2;
	return Tuple;
}

tuple_t *clone_tuple(const tuple_t tuple) {
	return new_tuple(
		tuple.type,
		clone_object(tuple.type, tuple.value1),
		clone_object(tuple.type, tuple.value2)
	);
}

compare_result_t compare_tuple(tuple_t tuple1, tuple_t tuple2) {
	compare_result_t result = tuple1.type == tuple2.type ? EQUALS : DIFFERENT;
	if(result == DIFFERENT)
		return result;
	result = compare_objects(tuple1.type, tuple1.value1, tuple2.value1);
	if(result == EQUALS)
		result = compare_objects(tuple1.type, tuple1.value2, tuple2.value2);
	return result;
}

void delete_tuple(tuple_t *tuple) {
	tuple->value1 = tuple->value2 = NULL;
	free(tuple);
}

char *tuple_to_string(tuple_t tuple) {
	char *string = (char*) malloc(STRINGIFY_OBJECT_SIZE);
	sprintf(string, "T<%s, %s>", to_string(tuple.type, tuple.value1), to_string(tuple.type, tuple.value2));
	return string;
}
#include <stdlib.h>
#include <string.h>
#include "utils.h"

extern const char *INSUFICIENT_MEMORY, *RESULT_TYPE, *NO_ERROR;

result_t *new_result() {
	result_t *result = (result_t*) malloc(sizeof(result_t));

	if(result == NULL)
		PrintError(INSUFICIENT_MEMORY, RESULT_TYPE);

	result->o = RESULT;
	result->value = NULL;
	result->isSuccess = true;
	strcpy(result->error, NO_ERROR);
	
	return result;
}

result_t *result_set_error(result_t *result, const char *error) {
	result->isSuccess = false;
	strcpy(result->error, error);
	return result;
}

result_t *result_set_value(result_t *result, void *value) {
	result->isSuccess = true;
	result->value = value;
	return result;
}
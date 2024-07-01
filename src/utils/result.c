#include <stdlib.h>
#include <string.h>
#include "utils.h"

extern const char *INSUFICIENT_MEMORY, *RESULT_TYPE;

result_t *new_result(void *value, const bool isSuccess, const char* error) {
	result_t *result = (result_t*) malloc(sizeof(result_t));

	if(result == NULL)
		PrintError(INSUFICIENT_MEMORY, RESULT_TYPE);

	result->value = value;
	result->isSuccess = isSuccess;
	strcpy(result->error,error);

	return result;
}

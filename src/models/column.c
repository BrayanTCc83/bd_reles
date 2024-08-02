#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "models.h"
#include "../utils/utils.h"

extern const char *INSUFICIENT_MEMORY, *NO_ERROR;
extern sequence_t DEFAULT_SEQUENCE;

column_t *new_column(const char *name, modifier_t modifiers, data_type_t *type, generator_t generator_type, void *generator) {
	column_t *NewColumn = (column_t*) malloc(sizeof(column_t));
	if(NewColumn == NULL)
		PrintError(INSUFICIENT_MEMORY, "struct column");

	NewColumn->o = COLUMN;
	NewColumn->type = type;
	NewColumn->modifiers = modifiers;
	NewColumn->generator_type = generator_type;
	if(generator_type == AUTOINCREMENTAL)
		NewColumn->generator = clone_sequence(DEFAULT_SEQUENCE);
	else
		NewColumn->generator = generator;
	strcpy(NewColumn->name, name);
	return NewColumn;
}

compare_result_t compare_column(column_t column, const char *name) {
	return strcmp(column.name, name);
}

char *column_to_string(column_t column) {
	char *string = (char*) malloc(STRINGIFY_OBJECT_SIZE);
	sprintf(string, "Col[Type:%d]{%s, %d}", column.type, column.name, column.modifiers);
	return string;
}

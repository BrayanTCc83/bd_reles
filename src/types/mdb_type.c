#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "../utils/utils.h"

extern const char*INSUFICIENT_MEMORY;
extern data_type_t DB_DATE;

mdb_type_t *new_mdb_type(data_type_t *type, void *value) {
	if(value == NULL)
		return NULL;

	mdb_type_t *reference = (mdb_type_t*) malloc(sizeof(mdb_type_t));
	if(reference == NULL)
		PrintError(INSUFICIENT_MEMORY, "struct mdb_type_t");

	reference->o = MDB_TYPE;
	reference->type = type;
	switch(type->group) {
		case BYTE_VALUE:
		case CHAR_VALUE:
			reference->char_value = ((Char*)value)->value; break;
		case SHORT_VALUE:
			reference->short_value = ((Short*)value)->value; break;
		case INT_VALUE:
			reference->int_value = ((Int*)value)->value; break;
		case LONG_VALUE:
			reference->long_value = ((Long*)value)->value; break;
		case FLOAT_VALUE:
			reference->float_value = ((Float*)value)->value; break;
		case DOUBLE_VALUE:
			reference->double_value = ((Double*)value)->value; break;
		case STRING_VALUE:
		case FORMAT_VALUE:
			strcpy(reference->string_value, ((String*)value)->value); break;
		case TEXT_VALUE:
			strcpy(reference->text_value, ((String*)value)->value); break;
		default:
			PrintError("El tipo %i no existe.", type->group);
	}
	return reference;
}

mdb_type_t *clone_mdb_type(mdb_type_t instance) {
	mdb_type_t *reference = (mdb_type_t*) malloc(sizeof(mdb_type_t));
	if(reference == NULL)
		PrintError(INSUFICIENT_MEMORY, "struct mdb_type_t");

	reference->type = instance.type;
	switch(instance.type->group) {
		case BYTE_VALUE:
		case CHAR_VALUE:
			reference->char_value = instance.char_value; break;
		case SHORT_VALUE:
			reference->short_value = instance.short_value; break;
		case INT_VALUE:
			reference->int_value = instance.int_value; break;
		case LONG_VALUE:
			reference->long_value = instance.long_value; break;
		case FLOAT_VALUE:
			reference->float_value = instance.float_value; break;
		case DOUBLE_VALUE:
			reference->double_value = instance.double_value; break;
		case STRING_VALUE:
		case FORMAT_VALUE:
			strcpy(reference->string_value, instance.string_value); break;
		case TEXT_VALUE:
			strcpy(reference->text_value, instance.text_value); break;
		default:
			PrintError("El tipo %i no existe.", instance.type->group);
	}
	return reference;
}

compare_result_t compare_mdb_value(mdb_type_t *value1, mdb_type_t *value2) {
	if(value1->type != value2->type)
		return DIFFERENT;

	bool result = true;
	switch(value1->type->group) {
		case SHORT_VALUE:
			return value1->short_value == value2->short_value ? EQUALS
				: value1->short_value > value2->short_value ? GREAT : LESS;
		case INT_VALUE:
			return value1->int_value == value2->int_value ? EQUALS
				: value1->int_value > value2->int_value ? GREAT : LESS;
		case LONG_VALUE:
			return value1->long_value == value2->long_value ? EQUALS
				: value1->long_value > value2->long_value ? GREAT : LESS;
		case BYTE_VALUE:
		case CHAR_VALUE:
			return value1->char_value == value2->char_value ? EQUALS
				: value1->char_value > value2->char_value ? GREAT : LESS;
		case FLOAT_VALUE:
			return value1->float_value == value2->float_value ? EQUALS
				: value1->float_value > value2->float_value ? GREAT : LESS;
		case DOUBLE_VALUE:
			return value1->double_value == value2->double_value ? EQUALS
				: value1->double_value > value2->double_value ? GREAT : LESS;
		case STRING_VALUE:
			return strcmp(value1->string_value, value2->string_value);
		case TEXT_VALUE:
			return strcmp(value1->text_value, value2->text_value);
		case FORMAT_VALUE:
			if(value1->type == &DB_DATE)
				return compare_date_type(value1->string_value, value2->string_value);
			return DIFFERENT;
	}
}

void delete_mdb_type(mdb_type_t *instance) {
	instance->type = NULL;
	free(instance);
}

char *mdb_type_to_string(mdb_type_t instance) {
	char *string = (char*) malloc(STRINGIFY_OBJECT_SIZE);
	int i = 0;

	switch(instance.type->group) {
		case SHORT_VALUE:
			sprintf(string, "%i", instance.short_value); break;
		case INT_VALUE:
			sprintf(string, "%d", instance.int_value); break;
		case LONG_VALUE:
			sprintf(string, "%ld", instance.long_value); break;
		case BYTE_VALUE:
			sprintf(string, "%i", instance.char_value); break;
		case CHAR_VALUE:
			sprintf(string, "%c", instance.char_value); break;
		case FLOAT_VALUE:
			sprintf(string, "%f", instance.float_value); break;
		case DOUBLE_VALUE:
			sprintf(string, "%lf", instance.double_value); break;
		case STRING_VALUE:
			sprintf(string, "\"%s\"", instance.string_value); break;
		case TEXT_VALUE:
			sprintf(string, "T\"%s\"", instance.text_value); break;
		case FORMAT_VALUE:
			sprintf(string, "'%s'", instance.string_value); break;
		default:
			PrintError("El tipo %i no existe.", instance.type->group);
	}
	return string;
}

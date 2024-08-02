#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include "models.h"
#include "../utils/utils.h"
#include "../struct/structs.h"

extern const char *INSUFICIENT_MEMORY, *NO_ERROR;
extern data_type_t DB_INT, DB_DOUBLE;

static int table_id = 0;

table_t *new_table(const char *name, int column_count, column_t **columns) {
	table_t *NewTable = (table_t*) malloc(sizeof(table_t));
	if(NewTable == NULL)
		PrintError(INSUFICIENT_MEMORY, "struct table_t");

	NewTable->o = TABLE;
	strcpy(NewTable->name, name);
	NewTable->id = ++table_id;
	NewTable->column_count = column_count;
	NewTable->columns = (column_t**) malloc(column_count * sizeof(column_t*));
	NewTable->rows = new_set(HASH_MAP);
	NewTable->indexes = NULL;

	for(int i = 0; i < column_count; i++)
		NewTable->columns[i] = columns[i];
	return NewTable;
}

static void *_generate_value_with_sequence(column_t *column) {
	result_t *result = use_sequence((sequence_t*)column->generator);
	if(!result->isSuccess)
		return result;

	Double *original = result->value;
	void *value = original;
	switch(column->type->group) {
		case SHORT_VALUE:
			value = new_short((short)original->value); break;
		case INT_VALUE:
			value = new_int((int)original->value); break;
		case LONG_VALUE:
			value = new_long((long)original->value); break;
		case CHAR_VALUE:
		case BYTE_VALUE:
			value = new_char((char)original->value); break;
		case FLOAT_VALUE:
			value = new_float((float)original->value); break;
	}
	return new_pair(column->name, new_mdb_type(column->type, value));
}

static void *_generate_default_value(column_t *column) {
	void *value = NULL;
	object_t *generator = (object_t*) column->generator;
	if(generator->o == FUNCTION)
		value = ((function_t*)generator)->execute(NULL)->value;
	else
		value = clone_object(generator);
	return new_pair(column->name, value);
}

static bool _table_validate_columns(table_t table, hash_map_t *row) {
	for(int i = 0; i < table.column_count; i++) {
		column_t *column = table.columns[i];
		result_t *result = hash_map_find(*row, column->name);
		if(result->isSuccess && ((pair_t*)result->value)->value)
			continue;

		if(column->modifiers & MOD_AUTOGENERATED) {
			if(column->generator == NULL)
				return false;
			generator_t g = column->generator_type;
			void *value = NULL;
			if(g == USE_SEQUENCE || g == AUTOINCREMENTAL)
				value = _generate_value_with_sequence(column);
			else if(g == DEFAULT_VALUE)
				value = _generate_default_value(column);

			if(value == NULL)
				return false;
			else if(((object_t*)value)->o == RESULT)
				return false;
			else
				hash_map_replace(row, (pair_t*)value);
			continue;
		}

		if(~column->modifiers & MOD_OPTIONAL)
			return false;
	}
	return true;
}

result_t *table_insert(table_t *table, int row_count, hash_map_t **rows) {
	result_t *result = new_result();
	if(!row_count)
		return result_set_error(result, "No hay columnas afectadas.");

	bool isValid = true;
	for(int i = 0; i < row_count && isValid; i++)
		isValid = _table_validate_columns(*table, rows[i]);

	if(!isValid)
		return result_set_error(result, "Las columnas no coinciden en los datos a insertar");

	for(int i = 0; i < row_count; i ++) {
		result_t *res_insert = set_insert(table->rows, rows[i]);
		if(!res_insert->isSuccess)
			return res_insert;
	}
	return result_set_value(result, new_int(row_count));
}

static void _table_row_update(hash_map_t *row, linked_list_t *replaces) {
	simple_node_t *reference = replaces->begin;
	while(reference != NULL) {
		hash_map_replace(row, (pair_t*) reference->value);
		reference = reference->next;
	}
}

result_t *table_update(table_t *table, condition_t *conditions, linked_list_t *replaces) {
	result_t *result = new_result();
	int row_count = 0;

	simple_node_t *row = table->rows->list->begin;
	while(row != NULL) {
		hash_map_t *_row = ((hash_map_t*)row->value);
		if(eval_condition(*conditions, *_row)) {
			_table_row_update(_row, replaces);
			row_count++;
		}
		row = row->next;
	}
	return result_set_value(result, new_int(row_count));
}

result_t *table_delete(table_t *table, condition_t *conditions) {
	result_t *result = new_result();
	int row_count = 0, total = table->rows->list->size;
	simple_node_t *row = table->rows->list->begin;
	set_t *rows = new_set(HASH_MAP);
	while(row != NULL) {
		hash_map_t *_row = ((hash_map_t*)row->value);
		row = row->next;
		if(!eval_condition(*conditions, *_row)) {
			set_insert(rows, _row);
			row_count++;
		}
	}
	delete_set(table->rows);
	table->rows = rows;
	return result_set_value(result, new_int(total - row_count));
}

void show_table_select(const select_table_t selected) {
	int size = 0;
	for(int group = 0; group < selected.group_count; group++)
		size += selected.rows[group]->list->size;
	
	printf("Se han seleccionado %d filas.\n", size);
	char *string = (char*) malloc(4000);
	int i = sprintf(string, "|");
	for(int x = 0; x < selected.column_count; x++)
		i += sprintf(string + i, " %20s |", selected.columns[x]->name);
	int y = i;
	i += sprintf(string + i, "\n");
	for(int group = 0; group < selected.group_count; group++) {
		for(int x = 0; x < y; x++)
			i += sprintf(string + i, "-");
		i += sprintf(string + i, "\n");
		simple_node_t *row = selected.rows[group]->list->begin;
		while(row != NULL) {
			i += sprintf(string + i, "|");
			for(int x = 0; x < selected.column_count; x++) {
				result_t *result = hash_map_find(*(hash_map_t*)row->value, selected.columns[x]->name);
				void *value = ((pair_t*)result->value)->value;
				i += sprintf(string + i, " %20s |", value ? to_string(value) : "null");
			}
			i += sprintf(string + i, "\n");
			row = row->next;
		}
	}
	sprintf(string + i, "\n");
	printf(string);
}

char *table_to_string(table_t table) {
	char *string = (char*) malloc(500);
	int i = sprintf(string, "Table[%d]{name: %s, columns: [", table.id, table.name);
	for(int x = 0; x < table.column_count; ) {
		i += sprintf(string + i, "%s:%d", table.columns[x]->name, table.columns[x]->modifiers);
		if(++x < table.column_count)
			i += sprintf(string + i, ", ");
	}
	i += sprintf(string + i, "], rows_count: %d}", table.rows->list->size);
	return string;
}
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include "models.h"
#include "../utils/utils.h"
#include "../struct/structs.h"
#include "../pattern/pattern.h"

extern const char *INSUFICIENT_MEMORY;
extern data_type_t DB_INT, DB_STRING;

result_t *_execute_functions(const select_table_t selected, linked_list_t columns) {
	result_t *result = new_result();
	linked_list_t *functions = new_linked_list(FUNCTION);
	linked_list_t *cols = new_linked_list(STRING);
	for(simple_node_t *column = columns.begin; column; column = column->next) {
		types_t o = ((object_t*)column->value)->o;
		if(o == FUNCTION)
			linked_list_push(functions, column->value);
		else
			linked_list_push(cols, column->value);
	}

	select_table_t *proyected = (select_table_t*) malloc(sizeof(select_table_t));
	if(proyected == NULL)
		PrintError(INSUFICIENT_MEMORY, "struct select_table_t");
	proyected->group_count = selected.group_count;
	proyected->rows = (set_t**) malloc(sizeof(set_t*));
	for(int g = 0; g < selected.column_count; g++)
		proyected->rows[g] = new_set(HASH_MAP);
	proyected->column_count = columns.size;
	proyected->columns = (column_t**) malloc(columns.size * sizeof(column_t*));

	for(int g = 0; g < proyected->group_count; g++) {
		int x = 0;
		hash_map_t *newRow = new_hash_map();
		for(simple_node_t *fn = functions->begin; fn; fn = fn->next) {
			function_t *function = fn->value;
			if(function->args->size != 1)
				return result_set_error(result, "Los argumentos no coinciden con la funcion.");

			linked_list_t *args = clone_object(function->args);
			select_table_t *nSelect = (select_table_t*) malloc(sizeof(select_table_t));
			nSelect->rows = &selected.rows[g];
			nSelect->columns = selected.columns;
			nSelect->column_count = selected.column_count;
			linked_list_push(args, nSelect);
			result = function->execute(args);
			if(!result->isSuccess)
				return result;
			proyected->columns[x++] = new_column(to_string(function), MOD_REQUIRED, NULL, NO_GENERATOR, NULL);
			hash_map_insert(newRow, new_pair(to_string(function), result->value));
		}
		set_insert(proyected->rows[g], newRow);
	}
	return result_set_value(result, proyected);
}

result_t *table_select(table_t table, condition_t *conditions) {
	result_t *result = new_result();
	simple_node_t *row = table.rows->list->begin;

	select_table_t *selected = (select_table_t*) malloc(sizeof(select_table_t));
	if(selected == NULL)
		PrintError(INSUFICIENT_MEMORY, "struct select_table_t");

	selected->rows = (set_t**) malloc(sizeof(set_t*));
	if(selected->rows == NULL)
		PrintError(INSUFICIENT_MEMORY, "struct set_t[]");
	selected->rows[0] = new_set(HASH_MAP);
	selected->group_count = 1;
	selected->column_count = table.column_count;
	selected->columns = table.columns;
	while(row != NULL) {
		if(eval_condition(*conditions, *((hash_map_t*)row->value)))
			set_insert(selected->rows[0], row->value);
		row = row->next;
	}
	return result_set_value(result, selected);
}

result_t *table_group(select_table_t selected, const char *group_by) {
	result_t *result = new_result();
	select_table_t *grouped = (select_table_t*) malloc(sizeof(select_table_t));
	if(grouped == NULL)
		PrintError(INSUFICIENT_MEMORY, "struct select_table_t");

	grouped->rows = (set_t**) malloc(sizeof(set_t*));
	if(grouped->rows == NULL)
		PrintError(INSUFICIENT_MEMORY, "struct set_t[]");
	for(int i = 0; i < selected.group_count; i++)
		grouped->rows[i] = new_set(HASH_MAP);

	grouped->column_count = 1;
	grouped->columns = (column_t**) malloc(sizeof(column_t*));
	for(int i = 0; i < selected.column_count; i++) {
		column_t *column = selected.columns[i];
		if(strcmp(column->name, group_by))
			continue;
		
		grouped->columns[0] = column;
		break;
	}

	iterator_t *iterator = new_iterator(selected.rows[0]);
	hash_map_t *diffs = new_hash_map();
	while(iterator_has_more(*iterator)) {
		hash_map_t *_group = iterator_get_next(iterator)->value;
		iterator_t *itFilters = new_iterator(&group_by);
		result = hash_map_find(*_group, group_by);
		if(!result->isSuccess) return result;
		pair_t *pair = result->value;
		if(!pair->value)
			pair->value = new_mdb_type(&DB_STRING, new_string("null"));
		result = hash_map_find(*diffs, to_string(pair->value));
		if(!result->isSuccess) {
			printf("%s\n", to_string(pair->value));
			hash_map_insert(diffs, new_pair(to_string(pair->value), new_mdb_type(&DB_INT, new_int(1))));
		}
	}

	grouped->group_count = diffs->size;	
	iterator = new_iterator(diffs);
	int i = 0;
	while(iterator_has_more(*iterator)) {
		hash_map_t *map = new_hash_map();
		set_t *set = new_set(HASH_MAP);
		pair_t *pair = iterator_get_next(iterator)->value;
		hash_map_insert(map, new_pair(group_by, new_mdb_type(&DB_STRING, new_string(pair->key))));
		set_insert(set, map);
		grouped->rows[i++] = set;
	}
	return result_set_value(result, grouped);
}

result_t *table_having(select_table_t, const linked_list_t) {
	return new_result();
}

result_t *table_proyect(const select_table_t selected, linked_list_t columns) {
	if(columns.type == UNKNOWN_TYPE)
		return _execute_functions(selected, columns);

	result_t *result = new_result();
	select_table_t *proyected = (select_table_t*) malloc(sizeof(select_table_t));
	if(proyected == NULL)
		PrintError(INSUFICIENT_MEMORY, "struct select_table_t");

	proyected->rows = (set_t**) malloc(sizeof(set_t*));
	if(proyected->rows == NULL)
		PrintError(INSUFICIENT_MEMORY, "struct set_t[]");
	for(int i = 0; i < selected.group_count; i++)
		proyected->rows[i] = new_set(HASH_MAP);

	proyected->column_count = columns.size;
	proyected->columns = (column_t**) malloc(columns.size * sizeof(column_t*));

	int x = 0;
	for(simple_node_t *column = columns.begin; column; column = column->next) {
		for(int i = 0; i < selected.column_count; i++) {
			column_t *col = selected.columns[i];
			if(!strcmp(col->name, ((String*)column->value)->value)) {
				proyected->columns[x++] = col;
				break;
			}
		}
	}

	for(int i = 0; i < selected.group_count; i++) {
		for(simple_node_t *row = selected.rows[i]->list->begin; row; row = row->next) {
			hash_map_t *_row = (hash_map_t*) row->value;
			hash_map_t *newRow = new_hash_map();
			for(simple_node_t *column = columns.begin; column; column = column->next) {
				result = hash_map_find(*_row, ((String*)column->value)->value);
				if(!result->isSuccess) return result;
				result = hash_map_insert(newRow, result->value);
				if(!result->isSuccess) return result;
			}
			set_insert(proyected->rows[i], newRow);
		}
	}
	return result_set_value(result, proyected);
}

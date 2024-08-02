#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "models.h"
#include "../types/types.h"
#include "../utils/utils.h"
#include "../struct/structs.h"

#define DATE_LENGHT 11
#define BEGIN_YEAR 1900
extern const char *INSUFICIENT_MEMORY;
extern data_type_t DB_DATE, DB_INT, DB_DOUBLE, DB_STRING;

function_t *clone_function(const function_t function) {
	function_t *Function = (function_t*) malloc(sizeof(function_t));
	if(!Function)
		PrintError(INSUFICIENT_MEMORY, "struct function_t");

	Function->o = FUNCTION;
	Function->args = NULL;
	Function->alias = NULL;
	strcpy(Function->name, function.name);
	Function->execute = function.execute;
	return Function;
}

void function_set_alias(function_t *function, const char *alias) {
	function->alias = (char*) malloc(_MDB_DB_OBJECT_NAME_SIZE);
	strcpy(function->alias, alias);
}

char *function_to_string(const function_t function) {
	char *string = malloc(STRINGIFY_OBJECT_SIZE);
	if(function.alias) {
		sprintf(string, "%s", function.alias);
		return string;
	}

	int i = sprintf(string, "%s%s", function.name, to_string(function.args));
	string[strlen(function.name)] = '(';
	string[i-1] = ')';
	return string;
}

result_t *_get_now(linked_list_t *none) {
	result_t *result = new_result();
	return result;
}

result_t *_get_today(linked_list_t *none) {
	result_t *result = new_result();
	time_t current_time;
	struct tm *timeinfo;
	time(&current_time);
	timeinfo = localtime(&current_time);
	char *string = (char*) malloc(DATE_LENGHT);
	sprintf(string, "%02d-%02d-%04d", timeinfo->tm_mday, timeinfo->tm_mon, timeinfo->tm_year + BEGIN_YEAR);
	return result_set_value(result, new_mdb_type(&DB_DATE, new_string(string)));
}

result_t *_count(linked_list_t *args) {
	result_t *result = new_result();
	String *column = args->begin->value;
	select_table_t *selected = args->begin->next->value;
	int total = selected->rows[0]->list->size;
	if(!strcmp(column->value, "*")) {
		return result_set_value(result,
			new_mdb_type(&DB_INT, new_int(total)));
	}

	mdb_type_t *value = NULL;
	int difference = 0;
	for(int i = 0; i < selected->column_count; i++) {
		if(strcmp(column->value, selected->columns[i]->name)) continue;
		for(simple_node_t *row = selected->rows[0]->list->begin; row; row = row->next) {
			hash_map_t *_row = row->value;
			result = hash_map_find(*_row, column->value);
			if(!result->isSuccess) break;
			pair_t *pair = result->value;
			if(!pair->value) ++difference;
		}
		value = new_mdb_type(&DB_INT, new_int(total - difference));
		break;
	}

	if(!value)
		return result_set_error(result, "Las columnas no coinciden.");
	return result_set_value(result, value);
}

static void *_get_value(mdb_type_t *t) {
	switch(t->type->group) {
		case BYTE_VALUE:
		case CHAR_VALUE:
			return new_double(t->char_value);
		case SHORT_VALUE:
			return new_double(t->short_value);
		case INT_VALUE:
			return new_double(t->int_value);
		case LONG_VALUE:
			return new_double(t->long_value);
		case FLOAT_VALUE:
			return new_double(t->float_value);
		case DOUBLE_VALUE:
			return new_double(t->double_value);
	}
	result_t *result = new_result();
	return result_set_error(result, "El tipo de dato es inesperado.");
}

result_t *_sum(linked_list_t *args) {
	result_t *result = new_result();
	String *column = args->begin->value;
	select_table_t *selected = args->begin->next->value;

	mdb_type_t *value = NULL;
	double sum = 0;
	for(int i = 0; i < selected->column_count; i++) {
		if(strcmp(column->value, selected->columns[i]->name)) continue;
		for(simple_node_t *row = selected->rows[0]->list->begin; row; row = row->next) {
			hash_map_t *_row = row->value;
			result = hash_map_find(*_row, column->value);
			if(!result->isSuccess) continue;
			pair_t *pair = result->value;
			object_t *res = _get_value(pair->value);
			if(res->o == RESULT) return (result_t*) res;
			sum += ((Double*)res)->value;
		}
		value = new_mdb_type(&DB_DOUBLE, new_double(sum));
		break;
	}

	if(!value)
		return result_set_error(result, "Las columnas no coinciden.");
	return result_set_value(result, value);
}

result_t *_min(linked_list_t *args) {
	result_t *result = new_result();
	String *column = args->begin->value;
	select_table_t *selected = args->begin->next->value;

	mdb_type_t *value = NULL;
	Double *min = NULL;
	for(int i = 0; i < selected->column_count; i++) {
		if(strcmp(column->value, selected->columns[i]->name)) continue;
		for(simple_node_t *row = selected->rows[0]->list->begin; row; row = row->next) {
			hash_map_t *_row = row->value;
			result = hash_map_find(*_row, column->value);
			if(!result->isSuccess) continue;
			pair_t *pair = result->value;
			object_t *res = _get_value(pair->value);
			if(res->o == RESULT) return (result_t*) res;
			if(!min || ((Double*)res)->value < min->value)
				min = (Double*) res;
		}
		value = new_mdb_type(&DB_DOUBLE, min);
		break;
	}

	if(!value)
		return result_set_error(result, "Las columnas no coinciden.");
	return result_set_value(result, value);
}

result_t *_max(linked_list_t *args) {
	result_t *result = new_result();
	String *column = args->begin->value;
	select_table_t *selected = args->begin->next->value;

	mdb_type_t *value = NULL;
	Double *max = NULL;
	for(int i = 0; i < selected->column_count; i++) {
		if(strcmp(column->value, selected->columns[i]->name)) continue;
		for(simple_node_t *row = selected->rows[0]->list->begin; row; row = row->next) {
			hash_map_t *_row = row->value;
			result = hash_map_find(*_row, column->value);
			if(!result->isSuccess) continue;
			pair_t *pair = result->value;
			object_t *res = _get_value(pair->value);
			if(res->o == RESULT) return (result_t*) res;
			if(!max || ((Double*)res)->value > max->value)
				max = (Double*) res;
		}
		value = new_mdb_type(&DB_DOUBLE, max);
		break;
	}

	if(!value)
		return result_set_error(result, "Las columnas no coinciden.");
	return result_set_value(result, value);
}

result_t *_avg(linked_list_t *args) {
	result_t *result = new_result();
	String *column = args->begin->value;
	select_table_t *selected = args->begin->next->value;

	mdb_type_t *value = NULL;
	int n = 0;
	double sum = 0;
	for(int i = 0; i < selected->column_count; i++) {
		if(strcmp(column->value, selected->columns[i]->name)) continue;
		for(simple_node_t *row = selected->rows[0]->list->begin; row; row = row->next) {
			hash_map_t *_row = row->value;
			result = hash_map_find(*_row, column->value);
			if(!result->isSuccess) continue;
			pair_t *pair = result->value;
			object_t *res = _get_value(pair->value);
			if(res->o == RESULT) return (result_t*) res;
			sum += ((Double*)res)->value;
			n++;
		}
		value = new_mdb_type(&DB_DOUBLE, new_double(sum/n));
		break;
	}

	if(!value)
		return result_set_error(result, "Las columnas no coinciden.");
	return result_set_value(result, value);
}

result_t *_mode(linked_list_t *args) {
	result_t *result = new_result();
	String *column = args->begin->value;
	select_table_t *selected = args->begin->next->value;

	mdb_type_t *value = NULL;
	int m = 0;
	hash_map_t *map = new_hash_map();
	for(int i = 0; i < selected->column_count; i++) {
		if(strcmp(column->value, selected->columns[i]->name)) continue;
		for(simple_node_t *row = selected->rows[0]->list->begin; row; row = row->next) {
			hash_map_t *_row = row->value;
			result = hash_map_find(*_row, column->value);
			if(!result->isSuccess) continue;
			pair_t *pair = result->value;
			if(!pair->value) continue;
			result = hash_map_find(*map, to_string(pair->value));
			int k = 0;
			if(!result->isSuccess)
				hash_map_insert(map, new_pair(to_string(pair->value),
					new_mdb_type(&DB_INT, new_int(1))));
			else
				k = ++((mdb_type_t*)((pair_t*)result->value)->value)->int_value;
			if(k > m) m = k;
		}
		break;
	}

	linked_list_t *mode = new_linked_list(STRING);
	linked_list_t *mapList = hash_map_to_list(*map);
	for(simple_node_t *reg = mapList->begin; reg; reg = reg->next) {
		if(m == ((mdb_type_t*)((pair_t*)reg->value)->value)->int_value)
			linked_list_push(mode, new_string(((pair_t*)reg->value)->key));
	}
	value = new_mdb_type(&DB_STRING, new_string(to_string(mode)));

	if(!value)
		return result_set_error(result, "Las columnas no coinciden.");
	return result_set_value(result, value);
}

result_t *_median(linked_list_t *args) {
	result_t *result = new_result();
	String *column = args->begin->value;
	select_table_t *selected = args->begin->next->value;

	mdb_type_t *value = NULL;
	double sum = 0;
	for(int i = 0; i < selected->column_count; i++) {
		if(strcmp(column->value, selected->columns[i]->name)) continue;
		for(simple_node_t *row = selected->rows[0]->list->begin; row; row = row->next) {
			hash_map_t *_row = row->value;
			result = hash_map_find(*_row, column->value);
			if(!result->isSuccess) continue;
			pair_t *pair = result->value;
			object_t *res = _get_value(pair->value);
			if(res->o == RESULT) return (result_t*) res;
			sum += ((Double*)res)->value;
		}
		value = new_mdb_type(&DB_DOUBLE, new_double(sum));
		break;
	}

	if(!value)
		return result_set_error(result, "Las columnas no coinciden.");
	printf("MEDIANA %s\n", to_string(value));
	return result_set_value(result, value);
}

const function_t NOW = {
	.o = FUNCTION,
	.name = "ahora",
	.alias = NULL,
	.args = NULL,
	.execute = _get_now
};

const function_t TODAY = {
	.o = FUNCTION,
	.name = "hoy",
	.alias = NULL,
	.args = NULL,
	.execute = _get_today
};

const function_t COUNT = {
	.o = FUNCTION,
	.name = "contar",
	.alias = NULL,
	.args = NULL,
	.execute = _count
};

const function_t SUM = {
	.o = FUNCTION,
	.name = "suma",
	.alias = NULL,
	.args = NULL,
	.execute = _sum
};

const function_t MAX = {
	.o = FUNCTION,
	.name = "max",
	.alias = NULL,
	.args = NULL,
	.execute = _max
};

const function_t MIN = {
	.o = FUNCTION,
	.name = "min",
	.alias = NULL,
	.args = NULL,
	.execute = _min
};

const function_t AVG = {
	.o = FUNCTION,
	.name = "promedio",
	.alias = NULL,
	.args = NULL,
	.execute = _avg
};

const function_t MEDIAN = {
	.o = FUNCTION,
	.name = "mediana",
	.alias = NULL,
	.args = NULL,
	.execute = _median
};

const function_t MODE = {
	.o = FUNCTION,
	.name = "moda",
	.alias = NULL,
	.args = NULL,
	.execute = _mode
};

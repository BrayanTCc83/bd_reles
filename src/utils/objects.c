#include <string.h>
#include "../models/models.h"
#include "../struct/structs.h"
#include "utils.h"

static char *DEFAULT_STRING = "Object{}";

void *clone_object(void *instance) {
	types_t type = ((object_t*)instance)->o;
	switch(type) {
		case CHAR:
			return new_char(*((char*)instance));
		case SHORT:
			return new_short(*((short*)instance));
		case INT:
			return new_int(*((int*)instance));
		case FLOAT:
			return new_float(*((float*)instance));
		case DOUBLE:
			return new_double(*((double*)instance));
		case STRING:
			return new_string(((char*)instance));
		case MDB_TYPE:
			return clone_mdb_type(*((mdb_type_t*)instance));
		case SIMPLE_NODE:
			return clone_simple_node(*((simple_node_t*)instance));
		case BINARY_NODE:
			return clone_binary_node(*((binary_node_t*)instance));
		case N_NODE:
			return clone_node(*((node_t*)instance));
		case PAIR:
			return clone_pair(*((pair_t*)instance));
		case TUPLE:
			return clone_tuple(*((tuple_t*)instance));
		case LINKED_LIST:
			return clone_linked_list(*((linked_list_t*)instance));
		case HASH_MAP:
			return clone_hash_map(*((hash_map_t*)instance));
		case SET:
			return clone_set(*((set_t*)instance));
		case BINARY_TREE:
			return clone_binary_tree(*((binary_tree_t*)instance));
		case GRAPH:
			return clone_graph(*((graph_t*)instance));
		case FUNCTION:
			return clone_function(*((function_t*)instance));
		case SEQUENCE:
			return clone_sequence(*((sequence_t*)instance));
		default:;
	}
	return NULL;
}

char *to_string(void *instance) {
	types_t type = ((object_t*)instance)->o;
	switch(type) {
		case CHAR:
			return char_to_string(((Char*)instance)->value);
		case SHORT:
			return short_to_string(((Short*)instance)->value);
		case INT:
			return int_to_string(((Int*)instance)->value);
		case FLOAT:
			return float_to_string(((Float*)instance)->value);
		case DOUBLE:
			return double_to_string(((Double*)instance)->value);
		case STRING:
			return ((String*)instance)->value;
		case SIMPLE_NODE:
			return simple_node_to_string(*((simple_node_t*)instance));
		case BINARY_NODE:
			return binary_node_to_string(*((binary_node_t*)instance));
		case N_NODE:
			return node_to_string(*((node_t*)instance));
		case MDB_TYPE:
			return mdb_type_to_string(*((mdb_type_t*)instance));
		case PAIR:
			return pair_to_string(*((pair_t*)instance));
		case TUPLE:
			return tuple_to_string(*((tuple_t*)instance));
		case LINKED_LIST:
			return linked_list_to_string(*((linked_list_t*)instance));
		case HASH_MAP:
			return hash_map_to_string(*((hash_map_t*)instance));
		case SET:
			return set_to_string(*((set_t*)instance));
		case BINARY_TREE:
			return binary_tree_to_string(*((binary_tree_t*)instance));
		case GRAPH:
			return graph_to_string(*((graph_t*)instance));
		case FUNCTION:
			return function_to_string(*((function_t*)instance));
		case CONDITION:
			return condition_to_string(*((condition_t*)instance));
		case COLUMN:
			return column_to_string(*((column_t*)instance));
		case TABLE:
			return table_to_string(*((table_t*)instance));
		default:;
	}
	return DEFAULT_STRING;
}

void *to_list(object_t *instance) {
	types_t type = ((object_t*)instance)->o;
	switch(type) {
		case LINKED_LIST:
			return instance;
		case HASH_MAP:
			return hash_map_to_list(*(hash_map_t*)instance);
		case SET:
			return ((set_t*)instance)->list;
	}
	linked_list_t *list = new_linked_list(instance->o);
	linked_list_push(list, instance);
	return list;
}

compare_result_t compare_objects(void *obj1, void *obj2) {
	types_t type = ((object_t*)obj1)->o;
	switch(type) {
		case CHAR:
			return compare_char(*(char*)obj1, *(char*)obj2);
		case SHORT:
			return compare_short(*(short*)obj1, *(short*)obj2);
		case INT:
			return compare_int(*(int*)obj1, *(int*)obj2);
		case FLOAT:
			return compare_float(*(float*)obj1, *(float*)obj2);
		case DOUBLE:
			return compare_double(*(double*)obj1, *(double*)obj2);
		case STRING:
			return strcmp(((String*)obj1)->value, ((String*)obj2)->value);
		case MDB_TYPE:
			return compare_mdb_value((mdb_type_t*)obj1, (mdb_type_t*)obj2);
		case SIMPLE_NODE:
			return compare_simple_node(*(simple_node_t*)obj1, *(simple_node_t*)obj2);
		case BINARY_NODE:
			return compare_binary_node(*(binary_node_t*)obj1, *(binary_node_t*)obj2);
		case N_NODE:
			return compare_node((node_t*)obj1, *((int*)obj2));
		case PAIR:
			return compare_pair((pair_t*)obj1, (const char*)obj2);
		case TUPLE:
			return compare_tuple(*(tuple_t*)obj1, *(tuple_t*)obj2);
		case LINKED_LIST:
			return compare_linked_list(*(linked_list_t*)obj1, *(linked_list_t*)obj2);
		case HASH_MAP:
			return compare_hash_map((hash_map_t*)obj1, (hash_map_t*)obj2);
		case SET:
			return compare_set(*(set_t*)obj1, *(set_t*)obj2);
		case BINARY_TREE:
			return compare_binary_tree(*(binary_tree_t*)obj1, *(binary_tree_t*)obj2);
		case GRAPH:
			return compare_graph(*(graph_t*)obj1, *(graph_t*)obj2);
		case COLUMN:
			return compare_column(*(column_t*)obj1, (const char*)obj2);
		default:;
	}
	return DIFFERENT;
}

types_t get_merged_type(types_t originalType) {
	switch(originalType) {
		case LINKED_LIST:
		case HASH_MAP:
			return originalType;
		default:
			return TUPLE;
	}
}

void *merge_objects(void *object1, void *object2) {
	types_t type = ((object_t*)object1)->o;
	switch(type) {
		case LINKED_LIST:
			return linked_list_join(*((linked_list_t*)object1), *((linked_list_t*)object2));
		case HASH_MAP:
			return hash_map_join(*((hash_map_t*)object1), *((hash_map_t*)object2));
		default:
			return new_tuple(type, object1, object2);
	}
}

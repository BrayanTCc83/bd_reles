#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "../utils/utils.h"

extern const char *INSUFICIENT_MEMORY, *SET_TYPE, *NO_ERROR, *ELEMENT_EXISTS_YET;
extern bool *FALSE, *TRUE;

static char *SET_INVALID_TYPES = "No es posible unir dos conjuntos con tipos diferentes (%d, %d)";

set_t *new_set(types_t type) {
	set_t *Set = (set_t*) malloc(sizeof(set_t));

	if(Set == NULL)
		PrintError(INSUFICIENT_MEMORY, SET_TYPE);

	Set->type = type;
	Set->list = new_linked_list(type);
	return Set;
}

set_t *clone_set(set_t set) {
	set_t *Set = new_set(set.type);
	simple_node_t *reference = set.list->begin;
	while(reference != NULL) {
		linked_list_push(Set->list, reference->value);
		reference = reference->next;
	}
	return Set;
}

compare_result_t compare_set(set_t set1, set_t set2) {
	return compare_linked_list(*set1.list, *set2.list);
}

void delete_set(set_t *set) {
	set->type = UNKNOWN_TYPE;
	delete_linked_list(set->list);
	free(set);
}

result_t *set_insert(set_t *set, void *value) {
	result_t *result = new_result(TRUE, true, NO_ERROR);

	simple_node_t *reference = set->list->begin;
	while(reference != NULL && compare_objects(set->type, reference->value, value) != EQUALS)
		reference = reference->next;

	if(reference != NULL) {
		result->value = FALSE;
		return result;
	}
	return linked_list_push(set->list, value);
}

result_t *set_delete(set_t *set, void *value) {
	return linked_list_delete(set->list, value);
}

set_t *set_union(set_t set1, set_t set2) {
	if(set1.type != set2.type)
		PrintError(SET_INVALID_TYPES, set1.type, set2.type);

	set_t *NewSet = new_set(set1.type);
	simple_node_t *reference = set1.list->begin;
	while(reference != NULL) {
		set_insert(NewSet, reference->value);
		reference = reference->next;
	}
	reference = set2.list->begin;
	while(reference != NULL) {
		set_insert(NewSet, reference->value);
		reference = reference->next;
	}
	return NewSet;
}

set_t *set_intersection(set_t set1, set_t set2) {
	if(set1.type != set2.type)
		PrintError(SET_INVALID_TYPES, set1.type, set2.type);

	set_t *NewSet = new_set(set1.type);
	simple_node_t *reference = set1.list->begin;
	while(reference != NULL) {
		result_t *result = linked_list_get(*set2.list, reference->value);
		if(result->isSuccess)
			set_insert(NewSet, reference->value);
		reference = reference->next;
	}
	return NewSet;
}

set_t *set_diference(set_t set1, set_t set2) {
	if(set1.type != set2.type)
		PrintError(SET_INVALID_TYPES, set1.type, set2.type);

	set_t *NewSet = clone_set(set1);
	simple_node_t *reference = set1.list->begin;
	while(reference != NULL) {
		result_t *result = linked_list_get(*set2.list, reference->value);
		reference = reference->next;
		if(result->isSuccess)
			set_delete(NewSet, result->value);
	}
	return NewSet;
}

set_t *set_exclusive_diference(set_t set1, set_t set2) {
	if(set1.type != set2.type)
		PrintError(SET_INVALID_TYPES, set1.type, set2.type);

	set_t *NewSet1 = set_diference(set1, set2);
	set_t *NewSet2 = set_diference(set2, set1);
	return set_union(*NewSet1, *NewSet2);
}

set_t *set_cross_product(set_t set1, set_t set2) {
	if(set1.type != set2.type)
		PrintError(SET_INVALID_TYPES, set1.type, set2.type);

	set_t *NewSet = new_set(get_merged_type(set1.type));
	simple_node_t *node1 = set1.list->begin;
	while(node1 != NULL) {
		simple_node_t *node2 = set2.list->begin;
		while(node2 != NULL) {
			set_insert(NewSet, merge_objects(set1.type, node1->value, node2->value));
			node2 = node2->next;
		}
		node1 = node1->next;
	}
	return NewSet;
}

char *set_to_string(set_t set) {
	char *string = linked_list_to_string(*set.list);
	string[0] = '(';
	string[strlen(string) - 1] = ')';
	return string;
}
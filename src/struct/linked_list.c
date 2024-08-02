#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "structs.h"

extern const char *INSUFICIENT_MEMORY, *LINKED_LIST_TYPE, *NO_ERROR, *INVALID_REFERENCE;

linked_list_t *new_linked_list(types_t type) {
	linked_list_t *List = (linked_list_t*) malloc(sizeof(linked_list_t));

	if(List == NULL)
		PrintError(INSUFICIENT_MEMORY, LINKED_LIST_TYPE);

	List->o = LINKED_LIST;
	List->begin = List->end = NULL;
	List->type = type;
	List->size = 0;

	return List;
}

linked_list_t *clone_linked_list(linked_list_t list) {
	linked_list_t *List = new_linked_list(list.type);
	simple_node_t *reference = list.begin;
	while(reference != NULL) {
		linked_list_push(List, reference->value);
		reference = reference->next;
	}
	return List;
}

compare_result_t compare_linked_list(linked_list_t obj1, linked_list_t obj2) {
	if(obj1.type != obj2.type)
		return DIFFERENT;
	if(obj1.size != obj2.size)
		return obj1.size > obj2.size ? GREAT : LESS;

	simple_node_t *ref1 = obj1.begin, *ref2 = obj2.begin;
	while(ref1 != NULL) {
		if(compare_objects(ref1->value, ref2->value) != EQUALS)
			return DIFFERENT;
		ref2 = ref2->next;
		ref1 = ref1->next;
	}
	return EQUALS;
}

void delete_linked_list(linked_list_t *list) {
	simple_node_t *reference = list->begin;
	while(reference != NULL) {
		list->begin = reference->next;
		delete_simple_node(reference);
		reference = list->begin;
	}
	list->begin = list->end = NULL;
	list->size = 0;
}

result_t *linked_list_push(linked_list_t *list, void* value) {
	simple_node_t *node = new_simple_node(value, list->type);
	result_t *result = new_result();
	if(list->size == 0) {
		list->begin = list->end = node;
		list->size++;
		return result;
	}

	list->end->next = node;
	list->end = node;
	list->size++;
	return result;
}

result_t *linked_list_replace(linked_list_t *list, void *compare, void *value) {
	simple_node_t *node = new_simple_node(value, list->type);
	result_t *result = new_result();
	if(list->size == 0)
		return result_set_error(result, "La lista esta vacia.");

	simple_node_t *previus = NULL;
	simple_node_t *next = list->begin;
	while(next != NULL && compare_objects(next->value, compare) != EQUALS) {
		previus = next;
		next = next->next;
	}

	if(next == NULL)
		return result_set_error(result, INVALID_REFERENCE);

	if(previus != NULL)
		previus->next = node;
	else
		list->begin = node;

	node->next = next->next;
	next->next = NULL;
	result_set_value(result, clone_object(next->value));
	delete_simple_node(next);
	return result;
}

result_t *linked_list_delete(linked_list_t *list, void *compare) {
	result_t *result = new_result();
	if(list->size == 0)
		return result_set_error(result, INVALID_REFERENCE);

	simple_node_t *previus = NULL;
	simple_node_t *next = list->begin;
	while(next != NULL && !compare_objects(next->value, compare)) {
		previus = next;
		next = next->next;
	}

	if(next == NULL)
		return result_set_error(result, INVALID_REFERENCE);

	list->size--;
	if(previus != NULL)
		previus->next = next->next;
	else
		list->begin = next->next;

	next->next = NULL;
	result_set_value(result, clone_object(next->value));
	delete_simple_node(next);
	return result;
}

result_t *linked_list_get(linked_list_t list, void *compare) {
	result_t *result = new_result();

	if(list.size == 0)
		return result_set_error(result, INVALID_REFERENCE);

	simple_node_t *reference = list.begin;
	simple_node_t *next = list.begin->next;
	while(reference != NULL && compare_objects(reference->value, compare) != EQUALS) {
		reference = next;
		if(next)
			next = next->next;
	}
	if(reference == NULL)
		return result_set_error(result, INVALID_REFERENCE);
	return result_set_value(result, reference->value);
}

linked_list_t *linked_list_join(linked_list_t list1, linked_list_t list2) {
	linked_list_t *List = clone_linked_list(list1);
	simple_node_t *reference = list2.begin;
	while(reference != NULL) {
		linked_list_push(List, reference->value);
		reference = reference->next;
	}
	return List;
}

char *linked_list_to_string(linked_list_t list) {
	char *cadena = (char*) malloc(STRINGIFY_OBJECT_SIZE);
	simple_node_t *reference = list.begin;
	int i = 0;

	cadena[i++] = '[';
	while(reference != NULL) {
		i += sprintf(cadena + i, "%s", reference->value ? to_string(reference->value) : "null");
		reference = reference->next;
		if(reference != NULL) {
			cadena[i++] = ',';
			cadena[i++] = ' ';
		}
	}
	cadena[i++] = ']';
	cadena[i] = '\0';
	return cadena;
}

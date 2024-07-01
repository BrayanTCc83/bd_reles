#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "structs.h"

extern const char *INSUFICIENT_MEMORY, *LINKED_LIST_TYPE, *NO_ERROR, *INVALID_REFERENCE;
extern const bool *TRUE, *FALSE;

linked_list_t *new_linked_list(types_t type) {
	linked_list_t *List = (linked_list_t*) malloc(sizeof(linked_list_t));

	if(List == NULL)
		PrintError(INSUFICIENT_MEMORY, LINKED_LIST_TYPE);

	List->begin = NULL;
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
		if(compare_objects(ref1->type, ref1->value, ref2->value) != EQUALS)
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
	list->begin = NULL;
	list->size = 0;
}

result_t *linked_list_push(linked_list_t *list, void* value) {
	simple_node_t *node = new_simple_node(value, list->type);
	result_t *result = new_result((void*)TRUE, true, NO_ERROR);
	if(list->size == 0) {
		list->begin = node;
		list->size++;
		return result;
	}

	simple_node_t *reference = list->begin;
	while(reference->next != NULL)
		reference = reference->next;

	if(reference == NULL) {
		strcpy(result->error, INVALID_REFERENCE);
		result->isSuccess = false;
		return result;
	}

	reference->next = node;
	list->size++;
	return result;
}

result_t *linked_list_delete(linked_list_t *list, void *compare) {
	result_t *result = new_result(NULL, true, NO_ERROR);

	if(list->size == 0) {
		result->isSuccess = false;
		strcpy(result->error, INVALID_REFERENCE);
		return result;
	}

	simple_node_t *previus = NULL;
	simple_node_t *next = list->begin;
	while(next != NULL && !compare_objects(list->type, next->value, compare)) {
		previus = next;
		next = next->next;
	}

	if(next == NULL) {
		result->isSuccess = false;
		strcpy(result->error, INVALID_REFERENCE);
		return result;
	}

	list->size--;
	if(previus != NULL)
		previus->next = next->next;
	else
		list->begin = next->next;

	next->next = NULL;
	result->value = clone_object(next->type, next->value);
	delete_simple_node(next);
	return result;
}

result_t *linked_list_get(linked_list_t list, void *compare) {
	result_t *result = new_result(NULL, true, NO_ERROR);

	if(list.size == 0) {
		result->isSuccess = false;
		strcpy(result->error, INVALID_REFERENCE);
		return result;
	}

	simple_node_t *reference = list.begin;
	simple_node_t *next = list.begin->next;
	while(reference != NULL && compare_objects(list.type, reference->value, compare) != EQUALS) {
		reference = next;
		if(next != NULL)
			next = next->next;
	}


	if(reference == NULL) {
		result->isSuccess = false;
		strcpy(result->error, INVALID_REFERENCE);
		return result;
	}
	result->value = reference->value;
	return result;
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
		i += sprintf(cadena + i, "%s", to_string(list.type, reference->value));
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "structs.h"
#include "../utils/utils.h"

extern const char *INSUFICIENT_MEMORY, *N_NODE_TYPE, *NO_ERROR;
extern bool *TRUE, *FALSE;

node_t *new_node(types_t type, bool isTerminal, int id) {
	node_t *Node = (node_t*) malloc(sizeof(node_t));
	if(Node == NULL)
		PrintError(INSUFICIENT_MEMORY, N_NODE_TYPE);

	Node->children = new_linked_list(PAIR);
	Node->type = type;
	Node->isTerminal = isTerminal;
	Node->id = id;

	return Node;
}

compare_result_t compare_node(node_t *node, int id) {
	return node->id == id ? EQUALS : node->id < id ? LESS : GREAT;
}

node_t *clone_node(const node_t node) {
	node_t *Node = new_node(node.type, node.isTerminal, node.id);

	Node->children = clone_linked_list(*node.children);
	return Node;
}

void delete_node(node_t *node) {
	delete_linked_list(node->children);
	node->children = NULL;
	free(node);
}

result_t *node_insert_child(node_t *node, pair_t *child) {
	result_t *result = new_result(NULL, true, NO_ERROR);
	linked_list_push(node->children, child);
	return result;
}

result_t *node_remove_child_by_weight(node_t *node, void *weight) {
	result_t *result = new_result(NULL, true, NO_ERROR);
	if(!node->children->size) {
		result->isSuccess = false;
		strcpy(result->error, "El nodo no contiene hijos.");
		return result;
	}

	return linked_list_delete(node->children, weight);
}

result_t *node_remove_child_by_id(node_t *node, int id) {
	result_t *result = new_result(NULL, true, NO_ERROR);
	if(node->children == 0) {
		result->isSuccess = false;
		strcpy(result->error, "El nodo no contiene hijos.");
		return result;
	}

	int i = 0;
	simple_node_t *reference = node->children->begin;
	while(!reference) {
		if(*((int*)((pair_t*)reference->value)->value) == id)
			break;
		reference == reference->next;
	}

	if(reference == NULL) {
		result->isSuccess = false;
		strcpy(result->error, "El nodo no tiene ningun hijo con la id especificada.");
	}

	return linked_list_delete(node->children, ((pair_t*)reference->value)->key);
}

result_t *node_clear_children(node_t *node) {
	result_t *result = new_result(TRUE, true, NO_ERROR);
	if(!node->children) {
		result->value = FALSE;
		return result;
	}
	delete_linked_list(node->children);
	node->children = new_linked_list(PAIR);
	return result;
}

char *node_to_string(node_t node) {
	char *string = (char*) malloc(STRINGIFY_OBJECT_SIZE);
	sprintf(string, "%d:%s -> %s", 
		node.id, 
		node.isTerminal ? "T" : "NT", 
		linked_list_to_string(*node.children)
	);
	return string;
}

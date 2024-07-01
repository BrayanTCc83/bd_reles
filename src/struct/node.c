#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "structs.h"
#include "../utils/utils.h"

extern const char *INSUFICIENT_MEMORY, *N_NODE_TYPE, *TUPLE_TYPE, *NO_ERROR;
extern bool *TRUE, *FALSE;

node_t *new_node(types_t type, bool isTerminal, int id) {
	node_t *Node = (node_t*) malloc(sizeof(node_t));
	if(Node == NULL)
		PrintError(INSUFICIENT_MEMORY, N_NODE_TYPE);

	Node->nodes = (tuple_t**) malloc(N_NODE_MIN_CHILDREN_SIZE * sizeof(tuple_t));
	if(Node == NULL)
		PrintError(INSUFICIENT_MEMORY, TUPLE_TYPE);

	Node->type = type;
	Node->isTerminal = isTerminal;
	Node->children = 0;
	Node->id = id;

	return Node;
}

compare_result_t compare_node(node_t *node, int id) {
	return node->id == id ? EQUALS : node->id < id ? LESS : GREAT;
}

node_t *clone_node(const node_t node) {
	node_t *Node = new_node(node.type, node.isTerminal, node.id);

	int children = node.children;
	Node->children = children;
	for(int i = 0; i < children; i++)
		Node->nodes[i] = clone_tuple(*node.nodes[i]);
	return Node;
}

void delete_node(node_t *node) {
	int children = node->children;
	node->children = 0;
	for(int i = 0; i < children; i++) {
		delete_tuple(node->nodes[i]);
		node->nodes[i] = NULL;
	}
	free(node);
}

result_t *node_insert_child(node_t *node, tuple_t *child) {
	result_t *result = new_result(NULL, true, NO_ERROR);
	if(node->children == N_NODE_MIN_CHILDREN_SIZE) {
		result->isSuccess = false;
		strcpy(result->error, "Se ha alcanzado el maximo de enlaces del nodo.");
		return result;
	}
	node->nodes[node->children++] = child;
	return result;
}

static void _node_remove_child(node_t *node, int i, result_t *result) {
	tuple_t *tuple = node->nodes[i++];
	for(; i < node->children - 1; i++)
		node->nodes[i - 1] = node->nodes[i];

	node->children--;
	result->value = clone_tuple(*tuple);
	delete_tuple(tuple);
}

result_t *node_remove_child_by_weight(node_t *node, void *weight) {
	result_t *result = new_result(NULL, true, NO_ERROR);
	if(node->children == 0) {
		result->isSuccess = false;
		strcpy(result->error, "El nodo no contiene hijos.");
		return result;
	}

	int i = 0;
	tuple_t *tuple = node->nodes[i];
	while(compare_objects(tuple->type, tuple->value1, weight) != EQUALS) {
		if(i == node->children) {
			tuple = NULL;
			break;
		}
		tuple = node->nodes[++i];
	}

	if(tuple == NULL) {
		result->isSuccess = false;
		strcpy(result->error, "El nodo no tiene ningun hijo con el peso especificado.");
	}

	_node_remove_child(node, i, result);
	return result;
}

result_t *node_remove_child_by_id(node_t *node, int id) {
	result_t *result = new_result(NULL, true, NO_ERROR);
	if(node->children == 0) {
		result->isSuccess = false;
		strcpy(result->error, "El nodo no contiene hijos.");
		return result;
	}

	int i = 0;
	tuple_t *tuple = node->nodes[i];
	while(*((int*)tuple->value2) != id) {
		if(i == node->children) {
			tuple = NULL;
			break;
		}
		tuple = node->nodes[++i];
	}

	if(tuple == NULL) {
		result->isSuccess = false;
		strcpy(result->error, "El nodo no tiene ningun hijo con la id especificada.");
	}

	_node_remove_child(node, i, result);
	return result;
}

result_t *node_clear_children(node_t *node) {
	result_t *result = new_result(TRUE, true, NO_ERROR);
	if(!node->children) {
		result->value = FALSE;
		return result;
	}
	for(int i = 0; i < node->children; i++)
		node->nodes[i] = NULL;
	node->children = 0;
	return result;
}

char *node_to_string(node_t node) {
	char *string = (char*) malloc(STRINGIFY_OBJECT_SIZE);
	int i = sprintf(string, "%d:%s -> []", node.id, node.isTerminal ? "T" : "NT");
	//int children = node.children;
	/*for(int i = 0; i < children; i++) {
		i += sprintf(string + i, "{.weight = %s, .next = %p}",
			to_string(node.type, node.nodes[i]->value1), node.nodes[i]->value2);
		if(i < children)
			i += sprintf(string + i, ", ");
	}
	string[i++] = ']';
	string[i] = '\0';*/
	return string;
}

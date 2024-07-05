#include <stdlib.h>
#include <stdio.h>
#include "structs.h"
#include "../utils/utils.h"

extern const char *INSUFICIENT_MEMORY, *SIMPLE_NODE_TYPE;

simple_node_t *new_simple_node(void *value, types_t type) {
	simple_node_t *SimpleNode = (simple_node_t*) malloc(sizeof(simple_node_t));

	if(SimpleNode == NULL)
		PrintError(INSUFICIENT_MEMORY, SIMPLE_NODE_TYPE);

	SimpleNode->o = SIMPLE_NODE;
	SimpleNode->next = NULL;
	SimpleNode->value = value;
	SimpleNode->type = type;

	return SimpleNode;
}

simple_node_t *clone_simple_node(const simple_node_t node) {
	simple_node_t *SimpleNode = new_simple_node(clone_object(node.value), node.type);
	SimpleNode->next = node.next;
	return SimpleNode;
}

compare_result_t compare_simple_node(simple_node_t obj1, simple_node_t obj2) {
	if(obj1.type != obj2.type)
		return DIFFERENT;
	return compare_objects(obj1.value, obj2.value);
}

void delete_simple_node(simple_node_t *node) {
	node->next = NULL;
	if(node->type == PAIR)
		delete_pair((pair_t*)node->value);
	free(node);
}

char *simple_node_to_string(simple_node_t instance) {
	char *string = (char*) malloc(STRINGIFY_OBJECT_SIZE);
	char *pair = to_string(instance.value);
	sprintf(string, "%s -> %p", pair, instance.next);
	return string;
}

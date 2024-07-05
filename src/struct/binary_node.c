#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

extern const char *INSUFICIENT_MEMORY, *BINARY_NODE_TYPE;

binary_node_t *new_binary_node(void *value, types_t type) {
	binary_node_t *BinaryNode = (binary_node_t*) malloc(sizeof(binary_node_t));
	if(BinaryNode == NULL)
		PrintError(INSUFICIENT_MEMORY, BINARY_NODE_TYPE);

	BinaryNode->o = BINARY_NODE;
	BinaryNode->type = type;
	BinaryNode->value = value;
	BinaryNode->left = BinaryNode->right = NULL;
	return BinaryNode;
}

binary_node_t *clone_binary_node(const binary_node_t node) {
	binary_node_t *BinaryNode = new_binary_node(clone_object(node.value), node.type);
	BinaryNode->left = node.left;
	BinaryNode->right = node.right;
	return BinaryNode;
}

compare_result_t compare_binary_node(binary_node_t node1, binary_node_t node2) {
	if(node1.type != node2.type)
		return DIFFERENT;

	return compare_objects(node1.value, node2.value);
}

void delete_binary_node(binary_node_t *node) {
	node->left = node->right = NULL;
	node->value = NULL;
	free(node);
}

char *binary_node_to_string(binary_node_t node) {
	char *string = (char*) malloc(STRINGIFY_OBJECT_SIZE);
	sprintf(string, "%s -> N<%p, %p>", to_string(node.value), node.left, node.right);
	return string;
}

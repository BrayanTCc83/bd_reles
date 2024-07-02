#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "structs.h"
#include "../utils/utils.h"

extern const char *INSUFICIENT_MEMORY, *BINARY_TREE_TYPE, *NO_ERROR;
extern bool *TRUE, *FALSE;

static char *TREE_IS_VOID = "El arbol no tiene elementos.";
static char *TREE_NOT_CONTAINS_ITEM = "El elemento no esta contenido dentro del arbol.";

binary_tree_t *new_binary_tree(types_t type) {
	binary_tree_t *BinaryTree = (binary_tree_t*) malloc(sizeof(binary_tree_t));

	if(BinaryTree == NULL)
		PrintError(INSUFICIENT_MEMORY, BINARY_TREE_TYPE);

	BinaryTree->type = type;
	BinaryTree->root = NULL;
	BinaryTree->size = 0;
	return BinaryTree;
}

static void _binary_tree_clone_recursive(binary_tree_t *tree, binary_node_t node) {
	if(node.left != NULL) {
		binary_tree_insert(tree, node.left);
		_binary_tree_clone_recursive(tree, *node.left);
	}
	if(node.right != NULL) {
		binary_tree_insert(tree, node.right);
		_binary_tree_clone_recursive(tree, *node.right);
	}
}

binary_tree_t *clone_binary_tree(const binary_tree_t tree) {
	binary_tree_t *BinaryTree = new_binary_tree(tree.type);
	if(!tree.root)
		return BinaryTree;
	binary_tree_insert(BinaryTree, tree.root->value);
	_binary_tree_clone_recursive(BinaryTree, *tree.root);
	return BinaryTree;
}

binary_tree_t *binary_tree_join(binary_tree_t tree1, binary_tree_t tree2) {
	binary_tree_t *Tree = clone_binary_tree(tree1);
	if(tree2.root)
		_binary_tree_clone_recursive(Tree, *tree2.root);
	return Tree;
}

static void _delete_binary_node_recursive(binary_node_t *node) {
	if(node->left)
		delete_binary_node(node->left);
	if(node->right)
		delete_binary_node(node->right);
	delete_binary_node(node);
}

void delete_binary_tree(binary_tree_t *tree) {
	_delete_binary_node_recursive(tree->root);
	tree->root = NULL;
	tree->size = 0;
	free(tree);
}

static void _binary_tree_to_list_recursive(linked_list_t *list, binary_node_t node) {
	if(node.left != NULL) {
		linked_list_push(list, node.left->value);
		_binary_tree_to_list_recursive(list, *node.left);
	}
	if(node.right != NULL) {
		linked_list_push(list, node.right->value);
		_binary_tree_to_list_recursive(list, *node.right);
	}
}

linked_list_t *binary_tree_to_list(binary_tree_t tree) {
	linked_list_t *List = new_linked_list(tree.type);
	if(tree.root != NULL)
		linked_list_push(List, tree.root->value);
	_binary_tree_to_list_recursive(List, *tree.root);
	return List;
}

static compare_result_t _compare_binary_node_recursive(binary_node_t node1, binary_node_t node2) {
	compare_result_t result = compare_binary_node(node1, node2);
	if(result != EQUALS)
		return result;

	result = _compare_binary_node_recursive(*node1.left, *node2.left);
	if(result != EQUALS)
		return result;

	result = _compare_binary_node_recursive(*node1.right, *node2.right);
	if(result != EQUALS)
		return result;
	
	return result;
}

compare_result_t compare_binary_tree(binary_tree_t tree1, binary_tree_t tree2) {
	if(tree1.type != tree2.type)
		return DIFFERENT;
	if(tree1.size != tree2.size)
		return tree1.size > tree2.size ? GREAT : LESS;

	return _compare_binary_node_recursive(*tree1.root, *tree2.root);
}

result_t *binary_tree_insert(binary_tree_t *tree, void *value) {
	result_t *result = new_result(TRUE, true, NO_ERROR);

	binary_node_t *newNode = new_binary_node(value, tree->type);
	if(tree->root == NULL) {
		tree->root = newNode;
		return result;
	}

	binary_node_t *reference = tree->root, *previus = NULL;
	compare_result_t cmp = DIFFERENT;
	
	while(reference != NULL) {
		cmp = compare_objects(tree->type, reference->value, value);
		previus = reference;
		if(cmp == LESS)
			reference = reference->right;
		else if(cmp == GREAT)
			reference = reference->left;
		else {
			result->isSuccess = false;
			strcmp(result->error, "El elemento ya se encuentra dentro del arbol.");
			return result;
		}
	}

	if(previus == NULL) {
		result->value = FALSE;
		return result;
	}

	if(cmp == GREAT)
		previus->left = newNode;
	else
		previus->right = newNode;

	tree->size++;
	return result;
}

result_t *binary_tree_find(binary_tree_t tree, void *value) {
	result_t *result = new_result(NULL, true, NO_ERROR);
	if(tree.size == 0) {
		result->isSuccess = false;
		strcpy(result->error, TREE_IS_VOID);
		return result;
	}

	binary_node_t *reference = tree.root;
	compare_result_t cmp = DIFFERENT;
	while(reference != NULL && (cmp = compare_objects(tree.type, reference->value, value)) != EQUALS) {
		if(cmp == LESS) reference = reference->right;
		else reference = reference->left;
	}

	if(reference == NULL) {
		result->isSuccess = false;
		strcpy(result->error, TREE_NOT_CONTAINS_ITEM);
		return result;
	}

	result->value = clone_object(tree.type, reference->value);
	return result;
}

static void _binary_tree_sort_recursive(binary_node_t *parent, binary_node_t *child) {
	if(child == NULL)
		return;

	binary_node_t *left = parent->left, *right = parent->right;
	compare_result_t cmp = compare_objects(parent->type, parent->value, child->value);
	if(cmp == GREAT) {
		if(right == NULL) {
			parent->right = child;
			return;
		}
		_binary_tree_sort_recursive(right, child);
		return;
	}

	if(left == NULL)
		parent->left = child;
	_binary_tree_sort_recursive(left, child);
}

result_t *binary_tree_delete(binary_tree_t *tree, void *value) {
	result_t *result = new_result(NULL, true, NO_ERROR);
	if(tree->size == 0) {
		result->isSuccess = false;
		strcpy(result->error, TREE_IS_VOID);
		return result;
	}

	binary_node_t *reference = tree->root, *parent = NULL;
	compare_result_t cmp = DIFFERENT;
	while(reference != NULL && (cmp = compare_objects(tree->type, reference->value, value)) != EQUALS) {
		parent = reference;
		if(cmp == LESS) reference = reference->right;
		else reference = reference->left;
	}

	if(reference == NULL) {
		result->isSuccess = false;
		strcpy(result->error, TREE_NOT_CONTAINS_ITEM);
		return result;
	}

	binary_node_t *child = reference->left;
	if(parent == NULL)
		parent = reference->right;
	else if(parent->right == reference) {
		parent->right = reference->right;
		parent = parent->right;
	} else { // parent->left == reference
		child = reference->right;
		parent->left = reference->left;
		parent = parent->left;
	}
	reference->left = reference->right = NULL;
	_binary_tree_sort_recursive(parent, child);

	result->value = clone_object(tree->type, reference->value);
	delete_binary_node(reference);
	return result;
}

static char *_binary_node_recursive_to_string(binary_node_t *node) {
	char *string = (char*) malloc(STRINGIFY_OBJECT_SIZE);
	sprintf(string, "%s -> (%s, %s)", to_string(node->type, node->value),
		node->left != NULL ?
		_binary_node_recursive_to_string(node->left)
		: "_",
		node->right != NULL ?
		_binary_node_recursive_to_string(node->right)
		: "_"
	);
	return string;
}

char *binary_tree_to_string(binary_tree_t tree) {
	char *string = (char*) malloc(STRINGIFY_OBJECT_SIZE);
	int i = sprintf(string, "T(");
	if(tree.root != NULL)
		i += sprintf(string, "%s", _binary_node_recursive_to_string(tree.root));
	string[i++] = ')';
	string[i] = '\0';
	return string;
}

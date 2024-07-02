#ifndef _MDB_STRUCTS
#define _MDB_STRUCTS
#include <stdbool.h>
#include "../utils/utils.h"
#include "../types/types.h"

// Size
#define KEY_MAX_SIZE 100
#define HASH_MAP_SIZE 255
#define TYPE_STRING_NAME_SIZE 100
#define N_NODE_MIN_CHILDREN_SIZE 10

// Structs
// TUPLE
typedef struct Tuple_t {
	types_t type;
	void *value1;
	void *value2;
} tuple_t;
// PAIR / TUPLE
typedef struct Pair_t {
	char key[KEY_MAX_SIZE];
	mdb_type_t *value;
} pair_t;
// SIMPLE NODE
typedef struct Simple_node_t simple_node_t;
struct Simple_node_t {
	types_t type;
	simple_node_t *next;
	void *value;
};
// SIMPLE LINKED LIST
typedef struct {
	types_t type;
	simple_node_t *begin;
	int size;
} linked_list_t;
// HASH MAP
typedef struct Hash_map_t {
	linked_list_t **list;
	int size;
} hash_map_t;
// BINARY NODE
typedef struct Binary_node_t binary_node_t;
struct Binary_node_t {
	types_t type;
	binary_node_t *left, *right;
	void *value;
};
// BINARY TREE
typedef struct Binary_tree_t {
	types_t type;
	binary_node_t *root;
	int size;
} binary_tree_t;
// SET
typedef struct Set_t {
	types_t type;
	linked_list_t *list;
} set_t;
// N NODE
typedef struct Node_t node_t;
struct Node_t {
	types_t type;
	bool isTerminal;
	int id;
	linked_list_t *children; // {type, weight, next}
};
// GRAPH
typedef struct Graph_t {
	types_t type;
	int size;
	node_t *last;
	linked_list_t *nodes;
} graph_t;

// Function types
typedef result_t *(*command_callback)(void*);

// Functions
// TUPLE
tuple_t *new_tuple(types_t, void*, void*);
compare_result_t compare_tuple(tuple_t, tuple_t);
tuple_t *clone_tuple(const tuple_t);
void delete_tuple(tuple_t*);
char *tuple_to_string(tuple_t); 		// T<A, B>
// PAIR
pair_t *new_pair(const char*, mdb_type_t*);
compare_result_t compare_pair(pair_t*, const char*);
pair_t *clone_pair(const pair_t);
void delete_pair(pair_t*);
char *pair_to_string(pair_t);			// <A, B>
// SIMPLE NODE
simple_node_t *new_simple_node(void*, types_t);
simple_node_t *clone_simple_node(const simple_node_t);
compare_result_t compare_simple_node(simple_node_t, simple_node_t);
void delete_simple_node(simple_node_t*);
char *simple_node_to_string(simple_node_t);	// (A) -> B
// LINKED LIST
linked_list_t *new_linked_list(types_t);
linked_list_t *clone_linked_list(const linked_list_t);
compare_result_t compare_linked_list(linked_list_t, linked_list_t);
void delete_linked_list(linked_list_t*);
result_t *linked_list_push(linked_list_t*, void*);
result_t *linked_list_delete(linked_list_t*, void*);
result_t *linked_list_get(linked_list_t, void*);
linked_list_t *linked_list_join(linked_list_t, linked_list_t);
char *linked_list_to_string(linked_list_t);	// [A, B, ...]
// HASH MAP
hash_map_t *new_hash_map();
compare_result_t compare_hash_map(hash_map_t*, hash_map_t*);
hash_map_t *clone_hash_map(const hash_map_t);
void delete_hash_map(hash_map_t*);
result_t *hash_map_insert(hash_map_t*, pair_t*);
result_t *hash_map_delete(hash_map_t*, const char*);
result_t *hash_map_find(hash_map_t, const char*);
hash_map_t *hash_map_filter(hash_map_t, int, ...);
hash_map_t *hash_map_join(hash_map_t, hash_map_t);
char *hash_map_to_string(hash_map_t);		// { key1: value1, key2: value2, ... }
// SET
set_t *new_set(types_t type);
set_t *clone_set(const set_t);
compare_result_t compare_set(set_t, set_t);
void delete_set(set_t*);
result_t *set_insert(set_t*, void*);
result_t *set_delete(set_t*, void*);
set_t *set_union(set_t, set_t);
set_t *set_intersection(set_t, set_t);
set_t *set_diference(set_t, set_t);
set_t *set_exclusive_diference(set_t, set_t);
set_t *set_cross_product(set_t, set_t);
char *set_to_string(set_t);			// (A, B, ...)
// BINARY NODE
binary_node_t *new_binary_node(void*, types_t);
binary_node_t *clone_binary_node(const binary_node_t);
compare_result_t compare_binary_node(binary_node_t, binary_node_t);
void delete_binary_node(binary_node_t*);
char *binary_node_to_string(binary_node_t);	// (A) -> {.left = B, .right = C}
// BINARY SEARCH TREE
binary_tree_t *new_binary_tree(types_t);
binary_tree_t *clone_binary_tree(const binary_tree_t);
compare_result_t compare_binary_tree(binary_tree_t, binary_tree_t);
binary_tree_t *binary_tree_join(binary_tree_t, binary_tree_t);
linked_list_t *binary_tree_to_list(binary_tree_t);
void delete_binary_tree(binary_tree_t*);
result_t *binary_tree_insert(binary_tree_t*, void*);
result_t *binary_tree_find(binary_tree_t, void*);
result_t *binary_tree_delete(binary_tree_t*, void*);
char *binary_tree_to_string(binary_tree_t);	// A -> (B, C)
// N NODE
node_t *new_node(types_t, bool, int);
compare_result_t compare_node(node_t*, int);
node_t *clone_node(const node_t);
void delete_node(node_t*);
result_t *node_insert_child(node_t*, pair_t*);
result_t *node_remove_child_by_weight(node_t*, void*);
result_t *node_remove_child_by_id(node_t*, int);
result_t *node_clear_children(node_t*);
char *node_to_string(node_t);			// (A) -> [B, C, ...]
// GRAPH
graph_t *new_graph(types_t);
graph_t *clone_graph(const graph_t);
compare_result_t compare_graph(graph_t, graph_t);
void delete_graph(graph_t*);
result_t *graph_insert_node(graph_t*, bool);			// {id:(T|NT)} -> []
result_t *graph_insert_edge(graph_t*, int, int, const char*);		// {id1 -(W)-> id2}
result_t *graph_remove_node(graph_t*, int);
result_t *graph_remove_edge(graph_t*, int, int);
result_t *graph_get_node(const graph_t, int);
char *graph_to_string(const graph_t);		// G[| A -> [(w1,B), (w2,C), ...]|, | B -> [(w3,C), (w4,D), ...]|, ...]

#endif

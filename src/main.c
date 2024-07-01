#include <stdio.h>
#include <stdbool.h>
#include "types/types.h"
#include "models/models.h"
#include "struct/structs.h"
#include "utils/utils.h"

#define SUCCESS 0

extern data_type_t DB_BYTE, DB_SHORT, DB_INT, DB_LONG, DB_FLOAT, DB_DOUBLE;
extern data_type_t DB_CHAR, DB_STRING, DB_TEXT, DB_DATE, DB_TIME;

int main() {
	mdb_type_t *byte_value = new_mdb_type(&DB_BYTE, new_char(234));
	mdb_type_t *short_value = new_mdb_type(&DB_SHORT, new_short(54300));
	mdb_type_t *int_value = new_mdb_type(&DB_INT, new_int(999999));
	mdb_type_t *long_value = new_mdb_type(&DB_LONG, new_long(1020304235));
	mdb_type_t *char_value = new_mdb_type(&DB_CHAR, new_char('C'));
	mdb_type_t *float_value = new_mdb_type(&DB_FLOAT, new_float(13.45));
	mdb_type_t *double_value = new_mdb_type(&DB_DOUBLE, new_double(1234.5412));
	mdb_type_t *string_value = new_mdb_type(&DB_STRING, new_string("Hola mundo."));
	mdb_type_t *text_value = new_mdb_type(&DB_TEXT, new_string("En la vida hay retos que no pueden ser superados solos, necesitamos el apoyo de alguien que este dispuesto a ayudarnos."));
	mdb_type_t *date_value = new_mdb_type(&DB_DATE, new_string("12-01-2024"));
	mdb_type_t *time_value = new_mdb_type(&DB_TIME, new_string("12:00:450"));

	char *mdb_type_string1 = to_string(MDB_TYPE, byte_value);
	printf("Byte Value: %s\n", mdb_type_string1);
	char *mdb_type_string2 = to_string(MDB_TYPE, short_value);
	printf("Short Value: %s\n", mdb_type_string2);
	char *mdb_type_string3 = to_string(MDB_TYPE, int_value);
	printf("Int Value: %s\n", mdb_type_string3);
	char *mdb_type_string4 = to_string(MDB_TYPE, long_value);
	printf("Long Value: %s\n", mdb_type_string4);
	char *mdb_type_string5 = to_string(MDB_TYPE, char_value);
	printf("Char Value: %s\n", mdb_type_string5);
	char *mdb_type_string6 = to_string(MDB_TYPE, float_value);
	printf("Float Value: %s\n", mdb_type_string6);
	char *mdb_type_string7 = to_string(MDB_TYPE, double_value);
	printf("Double Value: %s\n", mdb_type_string7);
	char *mdb_type_string8 = to_string(MDB_TYPE, string_value);
	printf("String Value: %s\n", mdb_type_string8);
	char *mdb_type_string9 = to_string(MDB_TYPE, text_value);
	printf("%s\n", mdb_type_string9);
	char *mdb_type_string10 = to_string(MDB_TYPE, date_value);
	printf("Date Value: %s\n", mdb_type_string10);
	char *mdb_type_string11 = to_string(MDB_TYPE, time_value);
	printf("Time Value: %s\n", mdb_type_string11);

	const char *key = "KEY";
	pair_t *pair = new_pair(key, date_value);
	char *pair_string = to_string(PAIR, pair);

	simple_node_t *node = new_simple_node(pair, PAIR);
	char *node_string = to_string(SIMPLE_NODE, node);
	printf("Simple Node: %s\n", node_string);

	linked_list_t *list = new_linked_list(PAIR);
	for(int i = 0; i < 10; i++)
		linked_list_push(list, new_pair(key, new_mdb_type(&DB_INT, new_int(i))));
	char *list_string = to_string(LINKED_LIST, list);
	printf("Linked List: %s\n", list_string);

	hash_map_t *hashMap = new_hash_map();
	char *keys[10] = { "id", "age", "a", "b", "c", "d", "name", "lastname", "email", "bornday" };
	for(int i = 0; i < 6; i++)
		hash_map_insert(hashMap, new_pair(keys[i], new_mdb_type(&DB_INT, new_int(i))));
	char *strings[4] = {"Brayan", "Tellez", "btellez@example.com", "10-02-2003"};
	for(int i = 6; i < 10; i++)
		hash_map_insert(hashMap, new_pair(keys[i], new_mdb_type(&DB_STRING, new_string(strings[i-6]))));
	char *hash_map_string = to_string(HASH_MAP, hashMap);
	printf("Hash Map: %s\n", hash_map_string);

	hash_map_t *hashMapFiltered = hash_map_filter(*hashMap, 2, "a", "b");
	char *hash_map2_string = to_string(HASH_MAP, hashMapFiltered);
	printf("Hash Map Filtered: %s\n", hash_map2_string);

	hash_map_t *hashMapFiltered2 = hash_map_filter(*hashMap, 2, "c", "d");
	hash_map_t *hashMapFiltered3 = hash_map_filter(*hashMap, 2, "a", "d");
	hash_map_t *hashMapJoined = hash_map_join(*hashMapFiltered, *hashMapFiltered2);
	char *hash_map3_string = to_string(HASH_MAP, hashMapJoined);
	printf("Hash Map Joined: %s\n", hash_map3_string);

	printf("Los Hash Maps %s y %s son %s\n",
		to_string(HASH_MAP, hashMapFiltered2), to_string(HASH_MAP, hashMapFiltered3),
		compare_hash_map(hashMapFiltered2, hashMapFiltered3) == EQUALS ? "Iguales" : "Diferentes");

	set_t *set1 = new_set(HASH_MAP);
	set_insert(set1, hashMapFiltered);
	set_insert(set1, hashMapFiltered2);
	char *set1_string = to_string(SET, set1);
	printf("Set 1: %s\n", set1_string);

	hash_map_t *hashMapFiltered4 = hash_map_filter(*hashMap, 2, "a", "b");
	hash_map_t *hashMapFiltered5 = hash_map_filter(*hashMap, 2, "c", "d");
	hash_map_t *hashMapFiltered6 = hash_map_filter(*hashMap, 2, "a", "c");

	set_t *set2 = new_set(HASH_MAP);
	set_insert(set2, hashMapFiltered3);
	set_insert(set2, hashMapFiltered4);
	char *set2_string = to_string(SET, set2);
	printf("Set 2: %s\n", set2_string);

	set_t *setUnion = set_union(*set1, *set2);
	char *set_union_string = to_string(SET, setUnion);
	printf("Set Union: %s\n", set_union_string);

	set_t *setIntersection = set_intersection(*set1, *set2);
	char *set_intersection_string = to_string(SET, setIntersection);
	printf("Set Intersection: %s\n", set_intersection_string);

	set_t *setDiference = set_diference(*set1, *set2);
	char *set_diference_string = to_string(SET, setDiference);
	printf("Set diference: %s\n", set_diference_string);

	set_t *setExclusiveDiference = set_exclusive_diference(*set1, *set2);
	char *set_exclusive_diference_string = to_string(SET, setExclusiveDiference);
	printf("Set exclusive diference: %s\n", set_exclusive_diference_string);

	set_t *setCrossProduct = set_cross_product(*set1, *set2);
	char *set_cross_product_string = to_string(SET, setCrossProduct);
	printf("Set cross_product: %s\n", set_cross_product_string);

	binary_tree_t *tree = new_binary_tree(MDB_TYPE);
	char *values[10] = { "j", "x", "c", "m", "d", "k", "u", "l", "y", "f" };
	for(int i = 0; i < 10; i++)
		binary_tree_insert(tree, new_mdb_type(&DB_STRING, values[i]));
	char *tree_string = to_string(BINARY_TREE, tree);
	printf("Binary Tree: %s\n", tree_string);

	result_t *resultFindBT = binary_tree_find(*tree, new_mdb_type(&DB_STRING, values[6]));
	if(resultFindBT->isSuccess)
		printf("Find in Binary Tree\n");
	else
		printf("%s\n", resultFindBT->error);

	resultFindBT = binary_tree_find(*tree, new_mdb_type(&DB_STRING, "j"));
	if(resultFindBT->isSuccess)
		printf("Find in Binary Tree\n");
	else
		printf("%s\n", resultFindBT->error);

	binary_tree_delete(tree, new_mdb_type(&DB_STRING, values[6]));
	char *tree_string_deleted = to_string(BINARY_TREE, tree);
	printf("Binary Tree: %s\n", tree_string_deleted);

	binary_tree_delete(tree, new_mdb_type(&DB_STRING, values[4]));
	char *tree_string_deleted2 = to_string(BINARY_TREE, tree);
	printf("Binary Tree: %s\n", tree_string_deleted2);

	binary_tree_delete(tree, new_mdb_type(&DB_STRING, values[8]));
	char *tree_string_deleted3 = to_string(BINARY_TREE, tree);
	printf("Binary Tree: %s\n", tree_string_deleted3);

	graph_t *graph = new_graph(STRING);
	for(int i = 0; i < 9; i++)
		graph_insert_node(graph, false);
	graph_insert_node(graph, true);

	int begin[10] = { 1, 1, 2, 3, 4, 4, 5, 6, 7, 9 };
	int end[10] = { 2, 3, 4, 4, 5, 6, 7, 7, 9, 10 };
	for(int i = 0; i < 10; i++)
		graph_insert_edge(graph, begin[i], end[i], values[i]);
	char *graph_string = to_string(GRAPH, graph);
	printf("Graph: %s\n", graph_string);

	return SUCCESS;
}

#include <stdlib.h>
#include <stdarg.h>
#include "structs.h"
#include "../utils/utils.h"
#include "../crypto/crypto.h"

extern const char *INSUFICIENT_MEMORY, *HASH_MAP_TYPE, *NO_ERROR;
extern bool *TRUE, *FALSE;

hash_map_t *new_hash_map() {
	hash_map_t *HashMap = (hash_map_t*) malloc(sizeof(hash_map_t));

	if(HashMap == NULL)
		PrintError(INSUFICIENT_MEMORY, HASH_MAP_TYPE);

	HashMap->o = HASH_MAP;
	HashMap->list = (linked_list_t**) malloc(HASH_MAP_SIZE * sizeof(linked_list_t));
	HashMap->size = 0;
	int i = 0;
	for(; i < HASH_MAP_SIZE; i++)
		HashMap->list[i] = new_linked_list(PAIR);

	return HashMap;
}

compare_result_t compare_hash_map(hash_map_t *map1, hash_map_t *map2) {
	compare_result_t equals = EQUALS;
	if(map1->size != map2->size)
		return map1->size > map2->size ? GREAT : LESS;

	for(int i = 0; i < HASH_MAP_SIZE && !equals; i++) {
		simple_node_t *reference = map1->list[i]->begin;
		while(reference != NULL && !equals) {
			pair_t *pair = (pair_t*)reference->value;
			result_t *result = hash_map_find(*map2, pair->key);
			reference = reference->next;
			if(!result->isSuccess || compare_mdb_value(pair->value, ((pair_t*)result->value)->value))
				equals = DIFFERENT;
		}
		reference = map2->list[i]->begin;
		while(reference != NULL && !equals) {
			pair_t *pair = (pair_t*)reference->value;
			result_t *result = hash_map_find(*map1, pair->key);
			reference = reference->next;
			if(!result->isSuccess || compare_mdb_value(pair->value, ((pair_t*)result->value)->value))
				equals = DIFFERENT;
		}
	}
	return equals;
}

hash_map_t *clone_hash_map(const hash_map_t map) {
	hash_map_t *HashMap = new_hash_map();
	for(int i = 0; i < HASH_MAP_SIZE; i++)
		if(map.list[i]->size != 0)
			HashMap->list[i] = clone_linked_list(*map.list[i]);
	return HashMap;
}

void delete_hash_map(hash_map_t *map) {
	for(int i = 0; i < HASH_MAP_SIZE; i++)
		delete_linked_list(map->list[i]);
	map->size = 0;
	free(map);
}

result_t *hash_map_insert(hash_map_t *map, pair_t *pair) {
	int hash = string_hash(pair->key);
	linked_list_t *list = map->list[hash % HASH_MAP_SIZE];
	result_t *result = linked_list_push(list, pair);
	if(result->isSuccess)
		map->size++;
	return result;
}

result_t *hash_map_delete(hash_map_t *map, const char *key) {
	int hash = string_hash(key);
	linked_list_t *list = map->list[hash % HASH_MAP_SIZE];
	result_t *result = linked_list_delete(list, (char*)key);
	if(result->isSuccess)
		map->size--;
	return result;
}

result_t *hash_map_find(hash_map_t map, const char *key) {
	int hash = string_hash(key);
	linked_list_t *list = map.list[hash % HASH_MAP_SIZE];
	return linked_list_get(*list, (char*)key);
}

hash_map_t *hash_map_filter(hash_map_t map, int n,...) {
	hash_map_t *newHashMap = new_hash_map();
	va_list keys;
	va_start(keys, n);

	for(int i = 0; i < n; i++) {
		const char *key = va_arg(keys, const char*);
		result_t *result = hash_map_find(map, key);
		if(!result->isSuccess)
			PrintError(result->error, "");

		hash_map_insert(newHashMap, ((pair_t*)result->value));
	}

	va_end(keys);
	return newHashMap;
}

hash_map_t *hash_map_join(hash_map_t map1, hash_map_t map2) {
	hash_map_t *newHashMap = new_hash_map();

	for(int j = 0; j < HASH_MAP_SIZE; j++) {
		linked_list_t *list = map1.list[j];
		simple_node_t *reference = list->begin;
		while(reference != NULL) {
			hash_map_insert(newHashMap, reference->value);
			reference = reference->next;
		}
	}

	for(int j = 0; j < HASH_MAP_SIZE; j++) {
		linked_list_t *list = map2.list[j];
		simple_node_t *reference = list->begin;
		while(reference != NULL) {
			hash_map_insert(newHashMap, reference->value);
			reference = reference->next;
		}
	}

	return newHashMap;
}

char *hash_map_to_string(hash_map_t instance) {
	char *string = (char*) malloc(STRINGIFY_OBJECT_SIZE);
	int i = 0, len = 0;

	string[i++] = '{';
	for(int j = 0; j < HASH_MAP_SIZE; j++) {
		linked_list_t *list = instance.list[j];
		simple_node_t *reference = list->begin;
		while(reference != NULL) {
			pair_t *pair = (pair_t*)reference->value;
			i += sprintf(string + i, "\"%s\": %s", pair->key, to_string(pair->value));
			reference = reference->next;
			if(++len < instance.size) {
				i += sprintf(string + i, ", ");
			}
		}
	}
	string[i++] = '}';
	string[i] = '\0';

	return string;
}

#ifndef _MDB_PATTERNS
#define _MDB_PATTERNS
#include <stdbool.h>
#include "../utils/utils.h"
#include "../struct/structs.h"

// Struct
typedef struct Iterator_t {
    types_t o;
    linked_list_t *iterable;
    simple_node_t *current;
} iterator_t;

// Function
// ITERATOR
iterator_t *new_iterator(void*);
bool iterator_has_more(iterator_t);
result_t *iterator_get_next(iterator_t*);

#endif
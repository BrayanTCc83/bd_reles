#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pattern.h"
#include "../utils/utils.h"
#include "../struct/structs.h"

extern const char *INSUFICIENT_MEMORY;

iterator_t *new_iterator(void *iterable) {
    iterator_t *This = (iterator_t*) malloc(sizeof(iterator_t));
    if(!This)
        PrintError(INSUFICIENT_MEMORY, "struct iterator_t");

    This->iterable = to_list(iterable);
    This->current = This->iterable->begin;
    This->o = ITERATOR;
    return This;
}

bool iterator_has_more(iterator_t this) {
    return this.current;
}

result_t *iterator_get_next(iterator_t *this) {
    result_t *result = new_result();
    if(!iterator_has_more(*this))
        return result_set_error(result, "El iterador no tiene un elemento siguiente a iterar.");

    if(!this->current)
        return result_set_error(result, "El iterador ha llegado al final.");
    return result_set_value(result, this->current->value);
}
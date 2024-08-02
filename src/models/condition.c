#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "models.h"
#include "../utils/utils.h"

extern const char *INSUFICIENT_MEMORY, *NO_ERROR;

condition_t *new_condition(operator_t op, void *left, void *right) {
	condition_t *NewCondition = (condition_t*) malloc(sizeof(column_t));
	if(NewCondition == NULL)
		PrintError(INSUFICIENT_MEMORY, "struct condition_t");

	NewCondition->o = CONDITION;
	NewCondition->op = op;
	NewCondition->left = left;
	NewCondition->right = right;
	return NewCondition;
}

// \ indica un literal
// _ ignora un solo caracter
// % ignora todos los caracteres hasta que llegue al final
static compare_result_t _eval_regex(const char *regex, const char *string) {
	char literal;
	bool isValid = true, isLiteral = false;
	int i = 0, ri = 0, len = strlen(string), reglen = strlen(regex);
	for(; i < len; i++) {
		literal = regex[ri];
		if(literal == '\\') isLiteral = true;
		else if(literal == '%' && !isLiteral) {
			literal = ri < reglen ? regex[++ri] : 0;
			while(i < len && string[i] != literal) i++;
			isLiteral = false;
		} else if(literal == '_' && !isLiteral)
			isLiteral = false;
		else if(literal != string[i]) {
			printf("%c %c\n", literal, string[i]);
			isValid = false;
			break;
		}
		ri++;
	}
	if((i >= len) && (ri >= reglen) && isValid) return CONTAINS;
	return DIFFERENT;
}

bool eval_condition(const condition_t condition, const hash_map_t row) {
	if(condition.op == NO_OP) return true;

	types_t t = ((object_t*) condition.left)->o;
	compare_result_t cmp = DIFFERENT;
	if(t == STRING && condition.op < OP_NOT) {
		result_t *result = hash_map_find(row, ((String*)condition.left)->value);
		if(!result->isSuccess)
			return false;
		mdb_type_t *value = (mdb_type_t*) ((pair_t*)result->value)->value;
		if(condition.op == OP_LIKE)
			cmp = _eval_regex(((mdb_type_t*)condition.right)->string_value, value->string_value);
		else cmp = compare_mdb_value(value, (mdb_type_t*)condition.right);
	}
	switch(condition.op) {
		case OP_LESS:
			return cmp == LESS;
		case OP_GREAT:
			return cmp == GREAT;
		case OP_EQUALS:
			return cmp == EQUALS;
		case OP_LESS_EQUALS:
			return cmp == LESS || cmp == EQUALS;
		case OP_GREAT_EQUALS:
			return cmp == GREAT || cmp == EQUALS;
		case OP_DIFFERENT:
			return cmp != EQUALS;
		case OP_LIKE:
			return cmp == CONTAINS;
		case OP_NOT:
			return !eval_condition(*((condition_t*)condition.left), row);
		case OP_OR:
			return eval_condition(*((condition_t*)condition.left), row)
			    || eval_condition(*((condition_t*)condition.right), row);
		case OP_AND:
			return eval_condition(*((condition_t*)condition.left), row)
			    && eval_condition(*((condition_t*)condition.right), row);
		case OP_XOR:;
			bool left = eval_condition(*((condition_t*)condition.left), row);
			bool right = eval_condition(*((condition_t*)condition.right), row);
			return (left && !right) || (!left && right);
	}
	return true;
}

static char *operator_to_string(operator_t op) {
	switch(op) {
		case OP_EQUALS: return "=";
		case OP_LESS: return "<";
		case OP_GREAT: return ">";
		case OP_LESS_EQUALS: return "<=";
		case OP_GREAT_EQUALS: return ">=";
		case OP_LIKE: return "LIKE";
		case OP_DIFFERENT: return "<>";
		case OP_NOT: return "NOT";
		case OP_AND: return "AND";
		case OP_OR: return "OR";
		case OP_XOR: return "XOR";
	}
	return "x";
}

char *condition_to_string(condition_t condition) {
	char *string = (char*) malloc(STRINGIFY_OBJECT_SIZE);
	int i = 0;
	if(condition.op == NO_OP)
		return "";

	if(condition.op == OP_NOT)
		sprintf(string, "%s%s", operator_to_string(condition.op), to_string(condition.left));
	else
		sprintf(string, "(%s %s %s)", to_string(condition.left), operator_to_string(condition.op), to_string(condition.right));
	return string;
}

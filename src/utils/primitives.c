#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

extern const char *INSUFICIENT_MEMORY, *CHAR_TYPE, *SHORT_TYPE, *INTEGER_TYPE;
extern const char *LONG_TYPE, *FLOAT_TYPE, *DOUBLE_TYPE, *STRING_TYPE;

Char *new_char(char byte) {
	Char *NewChar = (Char*) malloc(sizeof(Char));
	if(NewChar == NULL)
		PrintError(INSUFICIENT_MEMORY, CHAR_TYPE);
	NewChar->o = CHAR;
	NewChar->value = byte;
	return NewChar;
}

Short *new_short(short integer) {
	Short *NewShort = (Short*) malloc(sizeof(Short));
	if(NewShort == NULL)
		PrintError(INSUFICIENT_MEMORY, INTEGER_TYPE);
	NewShort->o = SHORT;
	NewShort->value = integer;
	return NewShort;
}

Int *new_int(int integer) {
	Int *NewInt = (Int*) malloc(sizeof(Int));
	if(NewInt == NULL)
		PrintError(INSUFICIENT_MEMORY, INTEGER_TYPE);
	NewInt->o = INT;
	NewInt->value = integer;
	return NewInt;
}

Long *new_long(long integer) {
	Long *NewLong = (Long*) malloc(sizeof(Long));
	if(NewLong == NULL)
		PrintError(INSUFICIENT_MEMORY, INTEGER_TYPE);
	NewLong->o = LONG;
	NewLong->value = integer;
	return NewLong;
}

Float *new_float(float real) {
	Float *NewFloat = (Float*) malloc(sizeof(Float));
	if(NewFloat == NULL)
		PrintError(INSUFICIENT_MEMORY, INTEGER_TYPE);
	NewFloat->o = FLOAT;
	NewFloat->value = real;
	return NewFloat;
}

Double *new_double(double real) {
	Double *NewDouble = (Double*) malloc(sizeof(Double));
	if(NewDouble == NULL)
		PrintError(INSUFICIENT_MEMORY, INTEGER_TYPE);
	NewDouble->o = DOUBLE;
	NewDouble->value = real;
	return NewDouble;
}

String *new_string(const char *string) {
	if(string == NULL || strlen(string) == 0)
		return NULL;

	String *NewString = (String*) malloc(sizeof(String));
	if(NewString == NULL)
		PrintError(INSUFICIENT_MEMORY, INTEGER_TYPE);
	NewString->o = STRING;
	NewString->value = (char*) malloc(strlen(string) + 1);
	strcpy(NewString->value, string);
	return NewString;
}

char *char_to_string(char v) {
	char *string = (char*) malloc(STRINGIFY_OBJECT_SIZE_SMALL);
	sprintf(string, "%d", v);
	return string;
}

char *short_to_string(short v) {
	char *string = (char*) malloc(STRINGIFY_OBJECT_SIZE_SMALL);
	sprintf(string, "%d", v);
	return string;
}

char *int_to_string(int v) {
	char *string = (char*) malloc(STRINGIFY_OBJECT_SIZE_SMALL);
	sprintf(string, "%d", v);
	return string;
}

char *long_to_string(long v) {
	char *string = (char*) malloc(STRINGIFY_OBJECT_SIZE_SMALL);
	sprintf(string, "%ld", v);
	return string;
}

char *float_to_string(float v) {
	char *string = (char*) malloc(STRINGIFY_OBJECT_SIZE_SMALL);
	sprintf(string, "%f", v);
	return string;
}

char *double_to_string(double v) {
	char *string = (char*) malloc(STRINGIFY_OBJECT_SIZE_SMALL);
	sprintf(string, "%lf", v);
	return string;
}

compare_result_t compare_char(char v1, char v2) {
	return v1 < v2 ? LESS : v1 > v2 ? GREAT : EQUALS;
}

compare_result_t compare_short(short v1, short v2) {
	return v1 < v2 ? LESS : v1 > v2 ? GREAT : EQUALS;
}

compare_result_t compare_int(int v1, int v2) {
	return v1 < v2 ? LESS : v1 > v2 ? GREAT : EQUALS;
}

compare_result_t compare_long(long v1, long v2) {
	return v1 < v2 ? LESS : v1 > v2 ? GREAT : EQUALS;
}

compare_result_t compare_float(float v1, float v2) {
	return v1 < v2 ? LESS : v1 > v2 ? GREAT : EQUALS;
}

compare_result_t compare_double(double v1, double v2) {
	return v1 < v2 ? LESS : v1 > v2 ? GREAT : EQUALS;
}

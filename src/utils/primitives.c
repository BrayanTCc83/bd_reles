#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

extern const char *INSUFICIENT_MEMORY, *CHAR_TYPE, *SHORT_TYPE, *INTEGER_TYPE;
extern const char *LONG_TYPE, *FLOAT_TYPE, *DOUBLE_TYPE, *STRING_TYPE;

char *new_char(char byte) {
	char *NewChar = (char*) malloc(sizeof(char));
	if(NewChar == NULL)
		PrintError(INSUFICIENT_MEMORY, CHAR_TYPE);
	*NewChar = byte;
	return NewChar;
}

short *new_short(short integer) {
	short *NewShort = (short*) malloc(sizeof(short));
	if(NewShort == NULL)
		PrintError(INSUFICIENT_MEMORY, INTEGER_TYPE);
	*NewShort = integer;
	return NewShort;
}

int *new_int(int integer) {
	int *NewInt = (int*) malloc(sizeof(int));
	if(NewInt == NULL)
		PrintError(INSUFICIENT_MEMORY, INTEGER_TYPE);
	*NewInt = integer;
	return NewInt;
}

long *new_long(long integer) {
	long *NewLong = (long*) malloc(sizeof(long));
	if(NewLong == NULL)
		PrintError(INSUFICIENT_MEMORY, INTEGER_TYPE);
	*NewLong = integer;
	return NewLong;
}

float *new_float(float real) {
	float *NewFloat = (float*) malloc(sizeof(float));
	if(NewFloat == NULL)
		PrintError(INSUFICIENT_MEMORY, INTEGER_TYPE);
	*NewFloat = real;
	return NewFloat;
}

double *new_double(double real) {
	double *NewDouble = (double*) malloc(sizeof(double));
	if(NewDouble == NULL)
		PrintError(INSUFICIENT_MEMORY, INTEGER_TYPE);
	*NewDouble = real;
	return NewDouble;
}

char *new_string(const char *string) {
	char *NewString = (char*) malloc(strlen(string) + 1);
	if(NewString == NULL)
		PrintError(INSUFICIENT_MEMORY, INTEGER_TYPE);
	strcpy(NewString, string);
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
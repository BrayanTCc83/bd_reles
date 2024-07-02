#ifndef _MDB_UTILS
#define _MDB_UTILS
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Sizes
#define ERROR_SIZE 300
#define STRINGIFY_OBJECT_SIZE_SMALL 12
#define STRINGIFY_OBJECT_SIZE 400

// Enums
// COMPARATION RESULT
typedef enum Compare_result_t {
	LESS = -1,
	EQUALS = 0,
	GREAT = 1,
	DIFFERENT
} compare_result_t;
// MDB TYPES
typedef enum Types_t {
	UNKNOWN_TYPE,
	// Primitives
	CHAR, SHORT, INT, LONG, FLOAT, DOUBLE, BOOL,
	STRING,
	// Basics
	MDB_TYPE,
	// Struct basics
	SIMPLE_NODE, BINARY_NODE, N_NODE,
	TUPLE, PAIR,
	// Data Structs
	LINKED_LIST, HASH_MAP, SET,
	BINARY_TREE, GRAPH
} types_t;

// Macros
#define PrintError(format, ...)  {printf(format, __VA_ARGS__); exit(1);}
#define Stringify(type) 	 #type
#define Typify(x) 		 _Generic((x), 					\
		bool: BOOL,			char: CHAR, 			\
		short: SHORT, 			int: INT, 			\
		long: LONG,			float: FLOAT,			\
		double: DOUBLE, 		char*: STRING,			\
		result_t*: RESULT,		mdb_type_t*: MDB_TYPE,		\
		pair_t*: PAIR,			simple_node_t*: SIMPLE_NODE, 	\
		linked_list_t*: LINKED_LIST, 	hash_map_t*: HASH_MAP,		\
		set_t*: SET,			default: UNKNOWN_TYPE)

// Structs
typedef struct Result_t {
	void *value;
	char error[ERROR_SIZE];
	bool isSuccess;
} result_t;

// Functions
// RESULT
result_t *new_result(void*, bool, const char*);
// OBJECT METHODS
compare_result_t compare_objects(types_t, void*, void*);
void *merge_objects(types_t, void*, void*);
types_t get_merged_type(types_t);
void *clone_object(types_t, void*);
char *to_string(types_t, void*);
// PRIMITIVES
char *new_char(char);
short *new_short(short);
int *new_int(int);
long *new_long(long);
float *new_float(float);
double *new_double(double);
char *new_string(const char*);
// PRIMITIVES TO STRING
char *char_to_string(char);
char *short_to_string(short);
char *int_to_string(int);
char *long_to_string(long);
char *float_to_string(float);
char *double_to_string(double);
// PRIMITIVES COMPARE
compare_result_t compare_char(char, char);
compare_result_t compare_short(short, short);
compare_result_t compare_int(int, int);
compare_result_t compare_long(long, long);
compare_result_t compare_float(float, float);
compare_result_t compare_double(double, double);

#endif

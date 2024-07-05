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
	// Result
	RESULT,
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
// DEFAULT OBJECT TYPE
typedef struct Object_t {
	types_t o;
} object_t;
// RESULT
typedef struct Result_t {
	types_t o;
	void *value;
	char error[ERROR_SIZE];
	bool isSuccess;
} result_t;
// PRIMITIVES
// CHAR
typedef struct Char_t {
	types_t o;
	char value;
} Char;
// SHORT
typedef struct Short_t {
	types_t o;
	short value;
} Short;
// INT
typedef struct Int_t {
	types_t o;
	int value;
} Int;
// LONG
typedef struct Long_t {
	types_t o;
	long value;
} Long;
// FLOAT
typedef struct Float_t {
	types_t o;
	float value;
} Float;
// DOUBLE
typedef struct Double_t {
	types_t o;
	double value;
} Double;
// STRING
typedef struct String_t {
	types_t o;
	char *value;
} String;

// Functions
// RESULT
result_t *new_result(void*, bool, const char*);
// OBJECT METHODS
compare_result_t compare_objects(void*, void*);
void *merge_objects(void*, void*);
types_t get_merged_type(types_t);
void *clone_object(void*);
char *to_string(void*);
// PRIMITIVES
Char *new_char(char);
Short *new_short(short);
Int *new_int(int);
Long *new_long(long);
Float *new_float(float);
Double *new_double(double);
String *new_string(const char*);
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

#ifndef _MDB_TYPES
#define _MDB_TYPES
#include <stdbool.h>
#include "../utils/utils.h"

// Sizes
#define NAME_MAX_SIZE 100
#define FORMAT_MAX_SIZE 100
#define STRING_MAX_SIZE 100
#define TEXT_MAX_SIZE 1000

// Enums
typedef enum Group_t {
	SHORT_VALUE, INT_VALUE, LONG_VALUE, BYTE_VALUE, CHAR_VALUE,
	FLOAT_VALUE, DOUBLE_VALUE,
	STRING_VALUE, TEXT_VALUE, FORMAT_VALUE
} group_t;

// Struct
typedef struct Data_type_t {
	char name[NAME_MAX_SIZE];
	char format[FORMAT_MAX_SIZE];
	group_t group;
} data_type_t;

typedef struct Mdb_type_t {
	data_type_t *type;
	union {
		short short_value;
		int int_value;
		long int long_value;
		float float_value;
		double double_value;
		bool bool_value;
		char char_value;
		char string_value[STRING_MAX_SIZE];
		char text_value[TEXT_MAX_SIZE];
	};
} mdb_type_t;

// Functions
mdb_type_t *new_mdb_type(data_type_t*, void*);
compare_result_t compare_mdb_value(mdb_type_t*, mdb_type_t*);
mdb_type_t *clone_mdb_type(mdb_type_t);
void delete_mdb_type(mdb_type_t*);
char *mdb_type_to_string(mdb_type_t);	// 	A

#endif

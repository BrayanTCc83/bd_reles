#include "types.h"

data_type_t DB_BYTE = {
	.name = "int 8",
	.format = "%i",
	.group = BYTE_VALUE
};

data_type_t DB_SHORT = {
	.name = "int 16",
	.format = "%i",
	.group = SHORT_VALUE
};

data_type_t DB_INT = {
	.name = "int 32",
	.format = "%d",
	.group = INT_VALUE
};

data_type_t DB_LONG = {
	.name = "int 64",
	.format = "%ld",
	.group = LONG_VALUE
};

data_type_t DB_CHAR = {
	.name = "char",
	.format = "%c",
	.group = CHAR_VALUE
};

data_type_t DB_FLOAT = {
	.name = "float",
	.format = "%f",
	.group = FLOAT_VALUE
};

data_type_t DB_DOUBLE = {
	.name = "double",
	.format = "%lf",
	.group = DOUBLE_VALUE
};

data_type_t DB_STRING = {
	.name = "string",
	.format = "%100s",
	.group = STRING_VALUE
};

data_type_t DB_TEXT = {
	.name = "text",
	.format = "%s",
	.group = TEXT_VALUE
};

data_type_t DB_DATE = {
	.name = "date",
	.format = "%2d-%2d-%2d",
	.group = FORMAT_VALUE
};

data_type_t DB_TIME = {
	.name = "time",
	.format = "%2d:%2d:%3d",
	.group = FORMAT_VALUE
};

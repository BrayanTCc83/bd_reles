#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "../utils/utils.h"

compare_result_t compare_date_type(const char *date1, const char *date2) {
	int day1, month1, year1;
	int day2, month2, year2;
	compare_result_t cmp = EQUALS;

	sscanf(date1, "%2d-%2d-%4d", &day1, &month1, &year1);
	sscanf(date2, "%2d-%2d-%4d", &day2, &month2, &year2);

	if(day1 < day2)
		cmp = LESS;
	else if(day1 > day2)
		cmp = GREAT;

	if(month1 < month2)
		cmp = LESS;
	else if(month1 > month2)
		cmp = GREAT;

	if(year1 < year2)
		cmp = LESS;
	else if(year1 > year2)
		cmp = GREAT;
	return cmp;
}

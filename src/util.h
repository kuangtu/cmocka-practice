#ifndef __UTIL_H
#define __UTIL_H
#include <stdint.h>

typedef struct TAG_LINE_COLUMNS
{
	uint32_t u32Size;
	uint32_t u32Num;
	char **szColumns;
} LINE_COLUMNS_T;

LINE_COLUMNS_T *
GetLineColumns(char *szLine, uint32_t u32ColNum, char *szSep);

void
FreeLineColumns(LINE_COLUMNS_T *ptLineColumns);

#endif //__UTIL_H

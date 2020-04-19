#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "util.h"

LINE_COLUMNS_T *
GetLineColumns(char *szLine, uint32_t u32ColNum, char *szSep)
{
	char *token, *dupLine, *freeLine, *szField;
	LINE_COLUMNS_T *ptLineColumns = NULL;
	size_t len;

	ptLineColumns = malloc(sizeof(LINE_COLUMNS_T));
	if (!ptLineColumns)
	{

		exit(EXIT_FAILURE);
	}

	memset(ptLineColumns, 0, sizeof(LINE_COLUMNS_T));
	ptLineColumns->u32Size = u32ColNum;
	ptLineColumns->u32Num = 0;
	ptLineColumns->szColumns = malloc(sizeof(char*) * u32ColNum);
	if (!ptLineColumns->szColumns)
	{
		free(ptLineColumns);

		exit(EXIT_FAILURE);
		
	}

	memset(ptLineColumns->szColumns, 0, sizeof(char*) * u32ColNum);
	
	freeLine = dupLine = strdup(szLine);

	while((token = strsep(&dupLine, szSep)) != NULL)
	{
		len = strlen(token);

		if (!len)
		{
			continue;
		}

		szField = malloc(sizeof(char) * (len + 1));
		if(!szField)
		{

			exit(EXIT_FAILURE);
		}
		memset(szField, 0 , len + 1);
		memcpy(szField, token, len);
		ptLineColumns->szColumns[ptLineColumns->u32Num++] = szField;

		//不要超过需要读取的列数
		if (ptLineColumns->u32Num >= ptLineColumns->u32Size)
		{
			break;
		}

	}

	free(freeLine);
	
	return ptLineColumns;
}		

void
FreeLineColumns(LINE_COLUMNS_T *ptLineColumns)
{
	uint32_t u32Cnt = 0;
	char *ptColumn;
	if (!ptLineColumns)
	{
		return ;
	}

	for (u32Cnt = 0; u32Cnt < ptLineColumns->u32Num; u32Cnt++)
	{
		ptColumn = ptLineColumns->szColumns[u32Cnt];
		if (ptColumn)
		{
			free(ptColumn);
		}
	}

	free(ptLineColumns->szColumns);
	free(ptLineColumns);
	
	return ;
}		

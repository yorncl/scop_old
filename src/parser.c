#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

const char whitespace[] = " \f\n\r\t\v";

int parsing_error(char *msg, size_t lineNum)
{
	fprintf(stderr, "Parsing error on line : %ld\n", lineNum);
	fprintf(stderr, "%s\n", msg);
	return EXIT_FAILURE;
}

int parse_line(char *line, size_t lineNum)
{
	(void) lineNum;
	if (strncmp("#", line, 1) == 0)
		return EXIT_SUCCESS;
	else if (strncmp("v ", line, 2) == 0)
	{

	}
	else if (strncmp("f ", line, 2) == 0)
	{

	}
	return 0;
}

int parse_file(char* filename)
{
	FILE* f = fopen(filename, "r");
	if (!f)
	{
		perror("Error opening file ");
		return EXIT_FAILURE;
	}
	size_t n, lineNum = 0;
	ssize_t r;
	char* ptr, *line = NULL;
	while ((r = getline(&line, &n, f)) > 0)
	{
		lineNum++;
		ptr = line + strspn(line, whitespace);
		// only whitespaces or empty
		if (*ptr == 0)
			continue;
		if ((r = parse_line(ptr, lineNum)) != EXIT_SUCCESS)	
			break;
	}
	free(line);
	return 0;
}

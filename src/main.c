#include <stdio.h>
#include <stdlib.h>
#include "obj.h"
#include "parser.h"

void print_usage()
{
	printf("Usage : ./scop OBJFILE\n");
}

int main(int ac, char** av)
{
	if (ac != 2)
	{
		print_usage();
		return 0;
	}
	int r = parse_file(av[1]);
	if (r == EXIT_FAILURE)
		return r;

}

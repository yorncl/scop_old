#include <stdio.h>
#include <stdlib.h>
#include "obj.hpp"
#include "parser.hpp"

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
	int r = parser(av[0]);
	if (r == EXIT_FAILURE)
		return r;
}

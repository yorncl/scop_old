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

int parse_line(char *line, size_t lineNum, Obj* obj)
{
	int n = 0;
	char tmp;
	
	if (strncmp("#", line, 1) == 0)
		return EXIT_SUCCESS;
	else if (strncmp("v ", line, 2) == 0)
	{
		vertex* v = ft_calloc(1, sizeof(vertex));
		n = sscanf(line, " %c %lf %lf %lf %lf ", &tmp, &v->x, &v->y, &v->z, &v->w);
		if (n < 4)
		{
			free(v);
			return parsing_error("Malformed vertex", lineNum);
		}
		if (n == 4)
			v->w = 1.0;
		if (ft_lstaddnew_back(&obj->vertices, v) == NULL)
		{
			free(v);
			return EXIT_FAILURE;
		}
	}
	else if (strncmp("f ", line, 2) == 0)
	{
		face* f = ft_calloc(1, sizeof(face));
		n = sscanf(line, " %c %llu %llu %llu  ", &tmp, &f->v1, &f->v2, &f->v3);
		if (n != 4)
		{
			free(f);
			return parsing_error("Malformed face", lineNum);
		}
		if (ft_lstaddnew_back(&obj->faces, f) == NULL)
		{
			free(f);
			return EXIT_FAILURE;
		}
	}
	else if (strncmp("o ", line, 2) == 0)
	{
		if (obj->name)
			return parsing_error("More than one object name entry", lineNum);
		char *name = line + 2 + strspn(line, whitespace);
		line[strlen(line) - 1] = 0; // A bit hacky but should work
		if (*name)
		{
			obj->name = strdup(name);
			if (obj->name == NULL)
				return EXIT_FAILURE;
		}
		else
		{
			return parsing_error("Name not specified", lineNum);
		}
	}
	return 0;
}

Obj* parse_file(char* filename)
{
	FILE* f = fopen(filename, "r");
	if (!f)
	{
		perror("Error opening file ");
		return 0;
	}
	size_t n, lineNum = 0;
	ssize_t r;
	char* ptr, *line = NULL;
	Obj* obj = ft_calloc(1, sizeof(Obj));
	obj->name = NULL;
	if (obj == NULL)
		return 0;
	printf("Beginning parsing\n");
	while ((r = getline(&line, &n, f)) > 0)
	{
		lineNum++;
		ptr = line + strspn(line, whitespace);
		// only whitespaces or empty
		if (*ptr == 0)
			continue;
		if ((r = parse_line(ptr, lineNum, obj)) != EXIT_SUCCESS)	
		{
			clear_obj(obj);
			free(line);
			return NULL;
		}
	}
	free(line);
	printf("Name of the object : %s\n", obj->name ? obj->name : "NO NAME GIVEN");
	printf("Number of vertices : %u\n", ft_lstsize(obj->vertices));
	printf("Number of faces : %u\n", ft_lstsize(obj->faces));
	return obj;
}

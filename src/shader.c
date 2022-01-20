#include "shader.h"
#include <stdio.h>
#include <libft.h>

Shader* load_shader(char *filename)
{
	Shader* shader;
	printf("Getting shader in file : %s\n", filename);
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		perror("Opening failed");
		fprintf(stderr, "Cannot open the shader files\n");
		return NULL;
	}
	shader = ft_calloc(1, sizeof(Shader));
	if (shader == NULL)
		return NULL;
	size_t r = fread(shader->src, sizeof(char), SOURCE_MAX_SIZE, file);
	if (ferror(file) != 0)
	{
		perror("Reading failed");
		fprintf(stderr, "Error while reading the shader files\n");
		fclose(file);
		destroy_shader(shader);
		return NULL;
	}
	shader->src[r] = '\0';
	fclose(file);
	return shader;
}

void destroy_shader(Shader* shader)
{
	free(shader);
}

#ifndef SHADER_H
#define SHADER_H

#define SOURCE_MAX_SIZE 10000 // this subject is not about memory managmeent so I don't care ^^

typedef struct s_shader {
	char src[SOURCE_MAX_SIZE]; } Shader;

Shader* load_shader(char *file);
void destroy_shader(Shader* shader);

#endif


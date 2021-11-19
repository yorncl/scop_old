#ifndef RENDER_H
#define RENDER_H

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "obj.h"

#define WINDOW_INIT_HEIGHT 800
#define WINDOW_INIT_WIDTH 800

typedef struct buff_st {
	size_t size;
	void* content;
} buff_st;

typedef struct ctx_st {
	Obj* obj;
	GLFWwindow* window;
	GLuint buff_vertex;
	GLuint buff_index;
	buff_st vertex_data;
	buff_st index_data;
} Context;

int render(Obj* obj);

#endif

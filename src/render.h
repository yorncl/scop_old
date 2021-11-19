#ifndef RENDER_H
#define RENDER_H

#include "obj.h"

#define WINDOW_INIT_HEIGHT 800
#define WINDOW_INIT_WIDTH 800

typedef struct buff_st {
	size_t size;
	void* content;
} buff_st;

typedef struct ctx_st {
	Obj* obj;
	buff_st vertex_buffer;
	buff_st index_buffer;
} Context;

int render(Obj* obj);

#endif

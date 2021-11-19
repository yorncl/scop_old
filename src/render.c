#include <stdio.h>
#include "render.h"
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include "shader.h"

static GLFWwindow* init_window()
{
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	return glfwCreateWindow(WINDOW_INIT_WIDTH, WINDOW_INIT_HEIGHT, "LearnOpenGL", NULL, NULL);
}

static void render_loop(GLFWwindow* window)
{
	while(!glfwWindowShouldClose(window))
	{
	    glfwSwapBuffers(window);
	    glfwPollEvents();    
	}
}

static int obj_to_vertex_buffer(Obj* obj, Context* ctx)
{
	size_t nv = ft_lstsize(obj->vertices);
	float* buff = ft_calloc(1, nv * sizeof(GL_FLOAT) * 3); // times 3 causes 3 float attribute per vertex

	if(buff == NULL)
		return EXIT_FAILURE;
	t_list* el = obj->vertices;
	vertex* v;
	int i = 0;
	while (el)
	{
		v = (vertex*)el->content;
		el = el->next;
		buff[i] = v->x;
		buff[i + 1] = v->y;
		buff[i + 2] = v->z;
		i++;	
	}
	ctx->vertex_buffer.content = buff;
	ctx->vertex_buffer.size = nv * GL_FLOAT * 3;
	return EXIT_SUCCESS;
}

static void init_context(Obj* obj, Context *ctx)
{
	ctx->obj = obj;
	ctx->vertex_buffer.content = NULL;
	ctx->vertex_buffer.size = 0;
	ctx->index_buffer.content = NULL;
	ctx->index_buffer.size = 0;

}

static void clear_context(Context* ctx)
{
	if (ctx->vertex_buffer.content)
		free(ctx->vertex_buffer.content);
	if (ctx->index_buffer.content)
		free(ctx->index_buffer.content);
}

// Returns EXIT_FAILURE after printing msg on stderr and clearing context
static int render_error(const char* msg, Context* ctx)
{
        fprintf(stderr, "%s", msg);
	clear_context(ctx);
        glfwTerminate();
	return EXIT_FAILURE;
}

int render(Obj* obj)
{
	// Init context
	Context ctx;
	init_context(obj, &ctx);

	GLFWwindow* window = init_window();
        if (window == NULL)
          return render_error("Failed to init window\n", &ctx);

        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
          return render_error("Cannot instantiate glad\n", &ctx);

	if(obj_to_vertex_buffer(obj, &ctx))
		return EXIT_FAILURE;
        render_loop(window);
	clear_context(&ctx);
	return EXIT_SUCCESS;
}

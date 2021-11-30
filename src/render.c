#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shader.h"
#include "render.h"

static void glfw_error_callback(int code, const char* msg)
{
	fprintf(stderr, "GLFW ERROR %d : %s\n", code, msg);
}

static GLFWwindow* init_window()
{
        glfwInit();
	glfwSetErrorCallback(glfw_error_callback);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	return glfwCreateWindow(WINDOW_INIT_WIDTH, WINDOW_INIT_HEIGHT, "LearnOpenGL", NULL, NULL);
}

static void render_loop(GLFWwindow* window)
{
	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();    
	}
}

static int obj_to_vertex_data(Obj* obj, Context* ctx)
{
	size_t nv = ft_lstsize(obj->vertices);
	float* buff = ft_calloc(1, nv * sizeof(GL_FLOAT) * 3); // times 3 causes 3 float attribute per vertex

	if(buff == NULL)
		return EXIT_FAILURE;
	t_list* el = obj->vertices;
	vertex* v;
	int i = 0;
	while (el) {
		v = (vertex*)el->content;
		el = el->next;
		buff[i] = v->x;
		buff[i + 1] = v->y;
		buff[i + 2] = v->z;
		i += 3;	
	}
	ctx->vertex_data.content = buff;
	ctx->vertex_data.size = nv * sizeof(GL_FLOAT) * 3;
	return EXIT_SUCCESS;
}

static int obj_to_index_data(Obj* obj, Context* ctx)
{
	size_t nv = ft_lstsize(obj->faces);
	uint32_t* buff = ft_calloc(1, nv * sizeof(GL_INT) * 3); // times 3 causes 3 float attribute per vertex

	if(buff == NULL)
		return EXIT_FAILURE;
	t_list* el = obj->faces;
	face* f;
	int i = 0;
	while (el)
	{
		f = (face*)el->content;
		el = el->next;
		buff[i] = f->v1;
		buff[i + 1] = f->v2;
		buff[i + 2] = f->v3;
		i += 3;	
	}
	ctx->index_data.content = buff;
	ctx->index_data.size = nv * sizeof(GL_INT) * 3;
	return EXIT_SUCCESS;
}

static void init_context(Obj* obj, GLFWwindow* window, Context *ctx)
{
	ctx->obj = obj;
	ctx->window = window;
	ctx->vertex_data.content = NULL;
	ctx->vertex_data.size = 0;
	ctx->index_data.content = NULL;
	ctx->index_data.size = 0;

}

static void clear_context(Context* ctx)
{
	if (ctx->vertex_data.content)
		free(ctx->vertex_data.content);
	if (ctx->index_data.content)
		free(ctx->index_data.content);
}

// Returns EXIT_FAILURE after printing msg on stderr and clearing context
static int render_error(const char* msg, Context* ctx)
{
        fprintf(stderr, "%s", msg);
	clear_context(ctx);
        glfwTerminate();
	return EXIT_FAILURE;
}

static int compile_shader(GLuint type, const char* const src, unsigned int* sid)
{
	*sid = glCreateShader(type);
	glShaderSource(*sid, 1, &src, NULL);
	glCompileShader(*sid);

	int res;
	glGetShaderiv(*sid, GL_COMPILE_STATUS, &res);
	char infoLog[512]; // TODO change this 
	glGetShaderInfoLog(*sid, 512, NULL, infoLog);
	infoLog[511] = 0; // TODO very hacky
	fprintf(stderr, "%s", infoLog);
	return res == GL_FALSE ? EXIT_FAILURE : EXIT_SUCCESS;
}

static int create_shader_program()
{
	unsigned int program = glCreateProgram();

	char* vertex = 
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"\n"
		"void main() {\n"
		" gl_Position = vec4(position.xyz, 1.0);\n"
		"}\n";

	char* fragment = 
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"void main() {\n"
		" color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";

	unsigned int vs = 0, fs = 0;
	if (compile_shader(GL_VERTEX_SHADER, (const char* const)vertex, &vs) == EXIT_SUCCESS &&
		compile_shader(GL_FRAGMENT_SHADER, (const char* const)fragment, &fs) == EXIT_SUCCESS)
	{
		printf("Shader compilation successfull !\n");
		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);

		int success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512]; // TODO change this 
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			infoLog[511] = 0; // TODO very hacky
			fprintf(stderr, "%s", infoLog);
		}
		glValidateProgram(program);
		glUseProgram(program);
	}
	glDeleteShader(vs); // TODO might crash ?
	if (fs)
		glDeleteShader(fs);
	return EXIT_SUCCESS; // TODO error handling
}

int render(Obj* obj)
{
	// Init context
	Context ctx;
	memset(&ctx, 0, sizeof(Context));

	GLFWwindow* window = init_window();
        if (window == NULL)
          return render_error("Failed to init window\n", &ctx);

        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
          return render_error("Cannot instantiate glad\n", &ctx);

	init_context(obj, window, &ctx);

	if(obj_to_vertex_data(obj, &ctx))
		return EXIT_FAILURE;
	if(obj_to_index_data(obj, &ctx))
		return EXIT_FAILURE;
	
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glGenBuffers(1, &ctx.buff_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, ctx.buff_vertex);
	glBufferData(GL_ARRAY_BUFFER, ctx.vertex_data.size, ctx.vertex_data.content, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
        glEnableVertexAttribArray(0);
	create_shader_program();
        render_loop(window);
	clear_context(&ctx);
	return EXIT_SUCCESS;
}

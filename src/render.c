#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shader.h"
#include "render.h"
#include "stb_image.h"

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

static int create_shader_program(Context* ctx)
{
	unsigned int program = glCreateProgram();

	Shader* vertex = load_shader("src/shaders/vertex.glsl");
	Shader* fragment = load_shader("src/shaders/fragment.glsl"); // TODO a way to bundle assets after runtime 
	if (vertex == NULL || fragment == NULL)
		return EXIT_FAILURE;

	unsigned int vs = 0, fs = 0;
	if (compile_shader(GL_VERTEX_SHADER, (const char* const)vertex->src, &vs) == EXIT_SUCCESS &&
		compile_shader(GL_FRAGMENT_SHADER, (const char* const)fragment->src, &fs) == EXIT_SUCCESS)
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
	destroy_shader(vertex);
	destroy_shader(fragment);
	ctx->vertexShaderId = vs;
	ctx->fragmentShaderId = fs;
	return program; // TODO error handling
}

int render(Obj* obj)
{
	// Init context
	Context ctx; memset(&ctx, 0, sizeof(Context));

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
	

	glGenVertexArrays(1, &ctx.VAO);
	glGenBuffers(1, &ctx.VBO);

	// binding the vertex array object and its buffer
	glBindVertexArray(ctx.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, ctx.VBO);
	glBufferData(GL_ARRAY_BUFFER, ctx.vertex_data.size, ctx.vertex_data.content, GL_STATIC_DRAW);
	// vertex attrib pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	// texture stuff
	int width, height, nrChannels;
	unsigned char *data = stbi_load("resources/cobble.jpg", &width, &height, &nrChannels, 0);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	// faces
	if(ctx.index_data.size > 0)
	{
		glGenBuffers(1, &ctx.EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ctx.index_data.size, ctx.index_data.content, GL_STATIC_DRAW);
	}
        glEnableVertexAttribArray(0);

	unsigned int shader_prog = create_shader_program(&ctx);

	while(!glfwWindowShouldClose(window))
	{


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		glUseProgram(shader_prog);

		float time = glfwGetTime();
		unsigned int angleLoc = glGetUniformLocation(shader_prog, "angle");
		glUniform1f(angleLoc, time);

		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawElements(GL_TRIANGLES, ctx.index_data.size/4, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents(); 
	}
	clear_context(&ctx);
	return EXIT_SUCCESS;
}

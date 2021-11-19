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

int render(Obj* obj)
{
	(void) obj;
	GLFWwindow* window = init_window();
        if (window == NULL) {
          fprintf(stderr, "Failed to create window");
          glfwTerminate();
          return EXIT_FAILURE;
        }
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
          glfwTerminate();
          fprintf(stderr, "Cannot instantiate glad\n");
          return EXIT_FAILURE;
        }
        render_loop(window);
	return EXIT_SUCCESS;
}

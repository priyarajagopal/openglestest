#include <stdafx.h>
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "viewer.h"

using namespace renderlib;

GLFWwindow* window = 0;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void init()
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(640, 480, "Test renderLib", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	
	// get version info
	const GLubyte* renderer = glGetString (GL_RENDERER); 
	const GLubyte* version = glGetString (GL_VERSION); 
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);
}

int main()
{
	init();

    Viewer* viewer = new Viewer();
    viewer->init();
	viewer->set_viewport(0, 0, 640, 480);
    
	while (!glfwWindowShouldClose(window))
    {
        viewer->draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
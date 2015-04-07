#include <stdafx.h>
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "viewer.h"

using namespace renderlib;

GLFWwindow* window = 0;
const int window_w = 640;
const int window_h = 480;

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

	window = glfwCreateWindow(window_w, window_h, "Test renderLib", NULL, NULL);
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
	viewer->set_viewport(0, 0, window_w, window_h);
	viewer->fit_camera_to_box(-14.401161193847656,-9.856366157531738,-1.0,15.459482192993164,19.143634796142578,28.825000762939453);
	viewer->load_model("http://christian-test.s3.amazonaws.com/SampleHouseTest/14.0.json");
    
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

#define GLEW_STATIC
#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#ifdef EMSCRIPTEN
	#include <emscripten/emscripten.h>
#endif
#include "errorHandling.h"
#include "shaders.h"

using namespace std;

const GLuint WIN_WIDTH = 500, WIN_HEIGHT = 500;
const int CIRCLE_RESOLUTION = 100;

// http://stackoverflow.com/questions/23177229/how-to-cast-int-to-const-glvoid
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void handleKeys(
		GLFWwindow* window, int key, int scancode, int action, int mode);
void renderFrame();

GLuint VBO;
GLFWwindow* window;

int main()
{
    // Init GLFW
    if(!glfwInit())
	{
		handleErrors(0, "Unable to initialize GLFW");
	}

    // Create window (and setup for ogl)
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(
			WIN_WIDTH, WIN_HEIGHT, "C++ to WebGL Prototype", NULL, NULL);
	if(!window)
	{
		handleErrors(0, "Unable to create GLFW window");
	}
    glfwMakeContextCurrent(window);
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

	// Load shader system
	shaders_init();

    // Setup callbacks
	glfwSetErrorCallback(handleErrors);
    glfwSetKeyCallback(window, handleKeys);

	// Setup data for solid circle
    GLfloat solidCircle[(CIRCLE_RESOLUTION+1) * 2];
	solidCircle[0] = 0;
	solidCircle[1] = 0;
	const double phaseDistance = 2*M_PI/(CIRCLE_RESOLUTION-1);
	for(int i = 0; i < CIRCLE_RESOLUTION; i++)
	{
		solidCircle[i*2+2] = sin(i*phaseDistance) * 100;
		solidCircle[i*2+3] = cos(i*phaseDistance) * 100;
	}

	// Setup solid circle buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
			GL_ARRAY_BUFFER, sizeof(solidCircle), solidCircle, GL_STATIC_DRAW);
	/// Position attribute setup
	int attribute_position = shaders_getAttributeIndex_vertexPosition();	
    glVertexAttribPointer(
			attribute_position, 2, GL_FLOAT, GL_FALSE, 8, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(attribute_position);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Game loop
	#ifdef EMSCRIPTEN
		emscripten_set_main_loop(renderFrame, 0, 1);
	#else
	    while (!glfwWindowShouldClose(window))
		{
			renderFrame();
		}
	#endif

	// Cleanup
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void handleKeys(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Escape key quits
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

float rotationPhase = 0;

void renderFrame()
{
	glfwPollEvents();

	glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	float x = sin(rotationPhase) * 250;
	float y = cos(rotationPhase) * 250;
	
	/// Draw scene
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	shaders_setObjectPosition(x + 500, y + 500);
	shaders_setObjectScale(1);
	shaders_setObjectColor(.75f, .5f, 0);
	shaders_useBase();
	glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_RESOLUTION+1);

	shaders_setObjectPosition(-x + 500, -y + 500);
	shaders_setObjectScale(.65f);
	shaders_setObjectColor(0, .5f, .75f);
	shaders_useBase();
	glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_RESOLUTION+1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glfwSwapBuffers(window);
	
	rotationPhase += .05f;
}

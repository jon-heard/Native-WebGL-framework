
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

void handleMousePosition(GLFWwindow* window, double xpos, double ypos);
void handleMouseClick(GLFWwindow* window, int button, int action, int mods);
void renderFrame();

GLuint VBO;
GLFWwindow* window;
int mouseX = 0, mouseY = 0, mouseDown = 0;

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
	shaders_setSceneLayout(-2000, -2000, 2000, 2000);
	//shaders_setSceneLayout(-1000, -1000, 2000, 2000);

    // Setup callbacks
	glfwSetErrorCallback(handleErrors);
    glfwSetCursorPosCallback(window, handleMousePosition);
	glfwSetMouseButtonCallback(window, handleMouseClick);

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

void handleMousePosition(GLFWwindow* window, double xpos, double ypos)
{
	mouseX = xpos*2000/WIN_WIDTH-1000;
	mouseY = ypos*2000/WIN_HEIGHT-1000;
}
void handleMouseClick(GLFWwindow* window, int button, int action, int mods)
{
	mouseDown = (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS);
}


float rotationPhase = 0;

void renderFrame()
{
	glfwSwapBuffers(window);

	glfwPollEvents();

	glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	/// Draw scene
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

		float x = sin(rotationPhase) * 250;
	float y = cos(rotationPhase) * 250;
	
	shaders_setObjectPosition(sin(rotationPhase) * 250, cos(rotationPhase) * 250);
	shaders_setObjectScale(1);
	shaders_setObjectColor(.75f, .5f, 0);
	shaders_useBase();
	glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_RESOLUTION+1);

	shaders_setObjectPosition(sin(rotationPhase+1) * 250, cos(rotationPhase+1) * 250);
	shaders_setObjectScale(.65f);
	shaders_setObjectColor(0, .5f, .75f);
	shaders_useBase();
	glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_RESOLUTION+1);
	
	shaders_setObjectPosition(mouseX, mouseY);
	shaders_setObjectScale(.25f);
	if(mouseDown)
	{
		shaders_setObjectColor(1, 1, 0);
	}
	else
	{
		shaders_setObjectColor(0, 0, 0);
	}
	shaders_useBase();
	glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_RESOLUTION+1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	rotationPhase += .05f;
}

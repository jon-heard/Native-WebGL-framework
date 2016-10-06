
#include "platform.h"
#define GLEW_STATIC
#include <GL/glew.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#ifdef EMSCRIPTEN
	#include <emscripten/emscripten.h>
#endif
#include "errorHandling.h"
#include "shaders.h"

typedef struct
{
	float red; float blue; float green;
} Color;

const GLuint WIN_WIDTH = 500, WIN_HEIGHT = 500;
const int LEFT = -1000, RIGHT = 1000, TOP = -1000, BOTTOM = 1000; 
const int CIRCLE_RESOLUTION = 50;
const float LINE_WIDTH = 1;
const Color COLORS[] = {
	{1, 0, 0},			// Red
	{0, 1, 0},			// Green
	{0, 0, 1},			// Blue
	{.5f, 0, .5f},		// purple
	{.5f, .5f, .5f}	// gray
};

// http://stackoverflow.com/questions/23177229/how-to-cast-int-to-const-glvoid
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace platform
{
	void handleMousePosition(GLFWwindow* window, double xpos, double ypos);
	void handleMouseClick(GLFWwindow* window, int button, int action, int mods);
	void renderFrame();

	GLFWwindow* window;
	int mouseX = 0, mouseY = 0, mouseDown = 0;
	void (*main_init)();
	void (*main_frameLogic)();
	void (*main_cleanup)();
	GLuint circleBuffer = 0;

	void run(void (*init)(), void (*frameLogic)(), void (*cleanup)())
	{
		main_init = init;
		main_frameLogic = frameLogic;
		main_cleanup = cleanup;
	
	    // Init GLFW
	    if(!glfwInit())
		{
			handleErrors(0, "Unable to initialize GLFW");
		}

	    // Create window (and setup for ogl)
	    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	    window = glfwCreateWindow(
				WIN_WIDTH, WIN_HEIGHT, "<Untitled>", NULL, NULL);
		if(!window)
		{
			handleErrors(0, "Unable to create GLFW window");
		}
	    glfwMakeContextCurrent(window);
	    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
		platform::setBackgroundColor(.25f, .25f, .25f);
		glLineWidth(LINE_WIDTH);

		// Load shader system
		shaders_init();
		shaders_setSceneLayout(LEFT, TOP, RIGHT, BOTTOM);
		int attribute_position = shaders_getAttributeIndex_vertexPosition();

	    // Setup callbacks
		glfwSetErrorCallback(handleErrors);
	    glfwSetCursorPosCallback(window, handleMousePosition);
		glfwSetMouseButtonCallback(window, handleMouseClick);

		// Setup data for objects
		{
			GLfloat circleData[(CIRCLE_RESOLUTION) * 2];
			const double phaseDistance = 2*M_PI/(CIRCLE_RESOLUTION-1);
			for(int i = 0; i < CIRCLE_RESOLUTION; i++)
			{
				circleData[i*2] = sin(i*phaseDistance);
				circleData[i*2+1] = cos(i*phaseDistance);
			}
		
			glGenBuffers(1, &circleBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, circleBuffer);
			glBufferData(
					GL_ARRAY_BUFFER, sizeof(circleData), circleData, GL_STATIC_DRAW);

			glVertexAttribPointer(
					attribute_position, 2, GL_FLOAT, GL_FALSE, 8, BUFFER_OFFSET(0));
			glEnableVertexAttribArray(attribute_position);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	
		main_init();
	
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
		glDeleteBuffers(1, &circleBuffer);
	    glfwTerminate();
		main_cleanup();
	}

	void setTitle(const char* title)
	{
		glfwSetWindowTitle(window, title);
	}

	void setBackgroundColor(float red, float green, float blue)
	{
		glClearColor(red, green, blue, 1);
	}

	void drawCircle(
			bool diskOrCircle, int colorIndex, float x, float y, float radius)
	{
		shaders_setObjectPosition(x, y);
		shaders_setObjectScale(radius);
		Color c = COLORS[colorIndex];
		shaders_setObjectColor(c.red, c.green, c.blue);
		shaders_useBase();

		if(diskOrCircle)
		{
			glBindBuffer(GL_ARRAY_BUFFER, circleBuffer);
			glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_RESOLUTION);
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, circleBuffer);
			glDrawArrays(GL_LINE_STRIP, 0, CIRCLE_RESOLUTION);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	bool isMouseDown()
	{
		return mouseDown;
	}

	float getMouseX()
	{
		return mouseX;
	}

	float getMouseY()
	{
		return mouseY;
	}


	void handleMousePosition(GLFWwindow* window, double xpos, double ypos)
	{
		mouseX = xpos*(RIGHT-LEFT)/WIN_WIDTH+LEFT;
		mouseY = ypos*(BOTTOM-TOP)/WIN_HEIGHT+TOP;
	}

	void handleMouseClick(GLFWwindow* window, int button, int action, int mods)
	{
		mouseDown = (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS);
	}

	void renderFrame()
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		
		main_frameLogic();
	
		glfwSwapBuffers(window);
	}
}

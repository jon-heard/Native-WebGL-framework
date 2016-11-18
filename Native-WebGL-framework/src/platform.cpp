
#include "platform.h"
#define GLEW_STATIC
#include <GL/glew.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <sstream>
#ifdef EMSCRIPTEN
	#include <emscripten/emscripten.h>
#endif
#include "errorHandling.h"
#include "shaders.h"

#include <iostream>

using namespace std;

const GLuint WIN_WIDTH = 500, WIN_HEIGHT = 500;
const int LEFT = -1000, RIGHT = 1000, TOP = -1000, BOTTOM = 1000; 
//const int[] rotationMatrix = {
//	1,0,0, 0,1,0, 0,0,1
//}

GLuint drawCacheBuffer1_frameBuffer = 0;
GLuint drawCacheBuffer2_frameBuffer = 0;
GLuint drawCacheBuffer1_texture = 0;
GLuint drawCacheBuffer2_texture = 0;

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
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Load shader system
		shaders_init();
		shaders_setSceneLayout(LEFT, TOP, RIGHT, BOTTOM);

		// Load draw system
		draw_init();

	    // Setup callbacks
		glfwSetErrorCallback(handleErrors);
	    glfwSetCursorPosCallback(window, handleMousePosition);
		glfwSetMouseButtonCallback(window, handleMouseClick);


		glGenTextures(1, &drawCacheBuffer1_texture);
		glBindTexture(GL_TEXTURE_2D, drawCacheBuffer1_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGBA, GL_INT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenFramebuffers(1, &drawCacheBuffer1_frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, drawCacheBuffer1_frameBuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, drawCacheBuffer1_texture, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return;
		
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
		draw_shutdown();
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

//	void cacheDraws()
//	{
//		glBindFramebuffer(GL_FRAMEBUFFER, drawCacheBuffer1_frameBuffer);
//		glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
//		glClearColor(0, 0, 0, 0);
//		glClear(GL_COLOR_BUFFER_BIT);
//	}
//
//	void flushDrawCache()
//	{
//		glBindFramebuffer(GL_FRAMEBUFFER,0);
//		drawImage(LEFT, TOP, RIGHT-LEFT, BOTTOM-TOP, drawCacheBuffer1_texture);
//	}
//
//	void flushDrawCacheWithBlur()
//	{
//		glBindFramebuffer(GL_FRAMEBUFFER,0);
//
//		shaders_useBlur(LEFT, TOP, RIGHT-LEFT, BOTTOM-TOP, drawCacheBuffer1_texture, .003f);
//
//		glBindBuffer(GL_ARRAY_BUFFER, rectangleBuffer);
//
//		glVertexAttribPointer(
//				attribute_position, 2, GL_FLOAT, GL_FALSE, 8, BUFFER_OFFSET(0));
//		glEnableVertexAttribArray(attribute_position);
//
//		glDrawArrays(GL_TRIANGLES, 0, 12);
//
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//	}

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
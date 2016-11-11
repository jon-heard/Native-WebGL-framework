
#include "platform.h"
#define GLEW_STATIC
#include <GL/glew.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <map>
#ifdef EMSCRIPTEN
	#include <emscripten/emscripten.h>
#endif
#include "errorHandling.h"
#include "shaders.h"

using namespace std;

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
	{.5f, .5f, .5f}		// gray
};
map<const char*, int> images;
GLuint drawCacheBuffer1_frameBuffer;
GLuint drawCacheBuffer2_frameBuffer;
GLuint drawCacheBuffer1_texture;
GLuint drawCacheBuffer2_texture;

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
	GLuint rectangleBuffer = 0;
	int attribute_position = 0;

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
		glEnable(GL_TEXTURE_2D);

		// Load shader system
		shaders_init();
		shaders_setSceneLayout(LEFT, TOP, RIGHT, BOTTOM);
		attribute_position = shaders_getAttributeIndex_vertexPosition();

	    // Setup callbacks
		glfwSetErrorCallback(handleErrors);
	    glfwSetCursorPosCallback(window, handleMousePosition);
		glfwSetMouseButtonCallback(window, handleMouseClick);

		// Setup data for circles
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

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		// Setup data for rectangles
		{
			GLfloat rectangleData[] = { 0,0, 1,1, 1,0, 0,0, 1,1, 0,1 };
		
			glGenBuffers(1, &rectangleBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, rectangleBuffer);
			glBufferData(
					GL_ARRAY_BUFFER, sizeof(rectangleData), rectangleData, GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		
		GLuint drawCacheBuffer1_frameBuffer;
		GLuint drawCacheBuffer2_frameBuffer;
		glGenFramebuffers(1, &drawCacheBuffer1_frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, drawCacheBuffer1_frameBuffer);
		glGenTextures(1, &drawCacheBuffer1_texture);
		glBindTexture(GL_TEXTURE_2D, drawCacheBuffer1_texture);
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		GLuint drawCacheBuffer1_depthBuffer;
		glGenRenderbuffers(1, &drawCacheBuffer1_depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, drawCacheBuffer1_depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIN_WIDTH, WIN_HEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, drawCacheBuffer1_depthBuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, drawCacheBuffer1_frameBuffer, 0);
		GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, DrawBuffers);
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

	void drawCircle(float x, float y, float radius, int colorIndex, bool diskOrCircle)
	{
		Color c = COLORS[colorIndex];
		shaders_useColor(x, y, radius, radius, c.red, c.green, c.blue);

		glBindBuffer(GL_ARRAY_BUFFER, circleBuffer);
		
		glVertexAttribPointer(
				attribute_position, 2, GL_FLOAT, GL_FALSE, 8, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(attribute_position);

		glDrawArrays(diskOrCircle ? GL_TRIANGLE_FAN : GL_LINE_STRIP, 0, CIRCLE_RESOLUTION);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	int loadImage(const char* filename)
	{
		return SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT);
	}
	
	void drawImage(float x, float y, float sizeX, float sizeY, const char* filename)
	{
		if(images.find(filename) == images.end())
		{
			loadImage(filename);
		}
		drawImage(x, y, sizeX, sizeY, images[filename]);
	}

	void drawImage(float x, float y, float sizeX, float sizeY, int textureId)
	{
		shaders_useTexture(x, y, sizeX, sizeY, textureId);

		glBindBuffer(GL_ARRAY_BUFFER, rectangleBuffer);

		glVertexAttribPointer(
				attribute_position, 2, GL_FLOAT, GL_FALSE, 8, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(attribute_position);

		glDrawArrays(GL_TRIANGLES, 0, 12);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void cacheDraws()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, drawCacheBuffer1_frameBuffer);
		glViewport(0,0,WIN_WIDTH,WIN_HEIGHT);
	}

	void flushDrawCache()
	{
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		drawImage(LEFT, TOP, RIGHT, BOTTOM, drawCacheBuffer1_texture);
	}

	void flushDrawCacheWithBlur()
	{
		shaders_useBlur(LEFT, TOP, RIGHT-LEFT, BOTTOM-TOP, drawCacheBuffer1_texture, .01f);

		glBindBuffer(GL_ARRAY_BUFFER, rectangleBuffer);

		glVertexAttribPointer(
				attribute_position, 2, GL_FLOAT, GL_FALSE, 8, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(attribute_position);

		glDrawArrays(GL_TRIANGLES, 0, 12);

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

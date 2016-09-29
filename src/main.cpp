
#define GLEW_STATIC

#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#ifdef EMSCRIPTEN
	#include <emscripten/emscripten.h>
#endif
#include "errorHandling.h"
#include "shaders.h"

using namespace std;

const GLuint WIN_WIDTH = 800, WIN_HEIGHT = 600;

// http://stackoverflow.com/questions/23177229/how-to-cast-int-to-const-glvoid
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void handleKeys(
		GLFWwindow* window, int key, int scancode, int action, int mode);
void renderFrame();

GLuint VAO;
GLFWwindow* window;

int main()
{
    GLint result;

    // Init GLFW
    if(!glfwInit())
	{
		handleErrors(0, "Unable to initialize GLFW");
	}
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    window = glfwCreateWindow(
			WIN_WIDTH, WIN_HEIGHT, "C++ to WebGL Prototype", NULL, NULL);
	if(!window)
	{
		handleErrors(0, "Unable to create GLFW window");
	}
		
    glfwMakeContextCurrent(window);

    // Set the required callback functions
	glfwSetErrorCallback(handleErrors);
    glfwSetKeyCallback(window, handleKeys);

	shaders_init();
	
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

    GLfloat vertices[] = {
         0.0f, -1.0f,   1, 0, 0,  // top, white
         1.0f,  0.0f,   0, 1, 0,  // Right, green
        -1.0f,  0.0f,   0, 0, 1,  // left, blue
         1.0f,  0.0f,   0, 1, 0,  // Right, green
        -1.0f,  0.0f,   0, 0, 1,  // left, blue
         0.0f,  1.0f,   1, 0, 1   // bottom, magenta
    };
	
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	int attribute_position = shaders_getAttributeIndex_position();	
    glVertexAttribPointer(
			attribute_position, 2, GL_FLOAT, GL_FALSE, 20, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(attribute_position);

	int attribute_color = shaders_getAttributeIndex_color();
    glVertexAttribPointer(
			attribute_color, 3, GL_FLOAT, GL_FALSE, 20, BUFFER_OFFSET(8));
	glEnableVertexAttribArray(attribute_color);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Game loop
	#ifdef EMSCRIPTEN
		emscripten_set_main_loop(renderFrame, 0, 1);
	#else
	    while (!glfwWindowShouldClose(window))
		{
			renderFrame();
		}
	#endif

    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

void handleKeys(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

float rotationPhase = 0;

void renderFrame()
{
	glfwPollEvents();

	glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//glRotatef(rotationPhase, 0, 1, 0);
	rotationPhase += 1.0f;
	shaders_useBase();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glfwSwapBuffers(window);
}

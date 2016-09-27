
#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#ifdef EMSCRIPTEN
	#include <emscripten/emscripten.h>
#endif
#include "errorHandling.h"
#include "shaders.h"

const GLuint WIN_WIDTH = 800, WIN_HEIGHT = 600;

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
    // Set all the required options for GLFW
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
        -0.5f, -0.5f, 0.0f, // Left  
         0.5f, -0.5f, 0.0f, // Right 
         0.0f,  0.5f, 0.0f  // Top   
    };
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
			0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

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

void renderFrame()
{
	glfwPollEvents();

	glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shaders_useBase();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	glfwSwapBuffers(window);
}

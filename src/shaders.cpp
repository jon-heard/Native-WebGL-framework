
#define GLEW_STATIC

#include "shaders.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "errorHandling.h"

const GLchar* SHADER_BASE_VERTEX =
	"uniform mat4 layout;\n"
	"attribute vec2 position;\n"
	"attribute vec3 color;\n"
	"varying vec3 fragColor;\n"
	"void main()\n"
	"{\n"
	"	gl_Position =\n"
	"			layout *\n"
	"			vec4(position, 0.0, 1.0);\n"
	"	fragColor = color;\n"
	"}\n\0";
const GLchar* SHADER_BASE_FRAGMENT =
	"precision highp float;\n"
	"varying vec3 fragColor;\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor = vec4(fragColor, 1.0);\n"
	"}\n\0";

GLuint shaders_prog_base;
GLuint shaders_sceneLayout;

void shaders_init()
{
    GLint result;

    glewExperimental = GL_TRUE;
    result = glewInit();
	if(result != GLEW_OK)
	{
		handleErrors(result, (const char*)glewGetErrorString(result));
	}
	
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &SHADER_BASE_VERTEX, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
		GLchar infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		handleErrors(0, infoLog);
    }
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &SHADER_BASE_FRAGMENT, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
		GLchar infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		handleErrors(0, infoLog);
    }
    shaders_prog_base = glCreateProgram();
    glAttachShader(shaders_prog_base, vertexShader);
    glAttachShader(shaders_prog_base, fragmentShader);
    glLinkProgram(shaders_prog_base);
    glGetProgramiv(shaders_prog_base, GL_LINK_STATUS, &result);
    if (!result) {
		GLchar infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		handleErrors(0, infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
	
	shaders_sceneLayout = glGetUniformLocation(shaders_prog_base, "layout");
	shaders_setSceneLayout(0, 0, 1, 1);
}

void shaders_useBase()
{
	glUseProgram(shaders_prog_base);
}

void shaders_setSceneLayout(float left, float top, float width, float height)
{
	float sceneLayoutMatrix[16] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1};
	glUseProgram(shaders_prog_base);
	glUniformMatrix4fv(shaders_sceneLayout, 1, false, sceneLayoutMatrix);
}

GLuint shaders_getAttributeIndex_position()
{
	return glGetAttribLocation(shaders_prog_base, "position");
}

GLuint shaders_getAttributeIndex_color()
{
	return glGetAttribLocation(shaders_prog_base, "color");
}

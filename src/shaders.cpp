
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "errorHandling.h"

const GLchar* SHADER_BASE_VERTEX =
	"attribute vec4 position;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = position;\n"
	"}\n\0";
const GLchar* SHADER_BASE_FRAGMENT =
	"precision highp float;\n"
	"void main()\n"
	"{\n"
	"gl_FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
	"}\n\0";

GLuint shaderProg_base;

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
    shaderProg_base = glCreateProgram();
    glAttachShader(shaderProg_base, vertexShader);
    glAttachShader(shaderProg_base, fragmentShader);
    glLinkProgram(shaderProg_base);
    glGetProgramiv(shaderProg_base, GL_LINK_STATUS, &result);
    if (!result) {
		GLchar infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		handleErrors(0, infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void shaders_useBase()
{
	glUseProgram(shaderProg_base);
}

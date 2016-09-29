
#define GLEW_STATIC

#include "shaders.h"
#include <GL/glew.h>
#include "errorHandling.h"

const GLchar* SHADER_BASE_VERTEX =
	"uniform mat4 sceneTransform;\n"
	"uniform vec2 objectPosition;\n"
	"uniform float objectScale;\n"
	"attribute vec2 vertexPosition;\n"
	"void main()\n"
	"{\n"
	"	gl_Position =\n"
	"			sceneTransform *\n"
	"			vec4((objectPosition+vertexPosition)*objectScale, 0.0, 1.0);\n"
	"}\n\0";
const GLchar* SHADER_BASE_FRAGMENT =
	"precision highp float;\n"
	"uniform vec3 objectColor;\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor = vec4(objectColor, 1.0);\n"
	"}\n\0";

GLuint shaders_prog_base;
GLuint shaders_uniform_sceneTransform;
GLuint shaders_uniform_objectPosition;
GLuint shaders_uniform_objectScale;
GLuint shaders_uniform_objectColor;

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
	
	shaders_uniform_sceneTransform =
			glGetUniformLocation(shaders_prog_base, "sceneTransform");
	shaders_uniform_objectPosition =
			glGetUniformLocation(shaders_prog_base, "objectPosition");
	shaders_uniform_objectScale =
			glGetUniformLocation(shaders_prog_base, "objectScale");
	shaders_uniform_objectColor =
			glGetUniformLocation(shaders_prog_base, "objectColor");
	shaders_setSceneLayout(0, 0, 1000, 1000);
	shaders_setObjectPosition(500, 500);
	shaders_setObjectScale(1);
	shaders_setObjectColor(1, 1, 1);
}

void shaders_useBase()
{
	glUseProgram(shaders_prog_base);
}

void shaders_setObjectPosition(float x, float y)
{
	glUseProgram(shaders_prog_base);
	glUniform2f(shaders_uniform_objectPosition, x, y);
	glUseProgram(0);
}

void shaders_setObjectScale(float scale)
{
	glUseProgram(shaders_prog_base);
	glUniform1f(shaders_uniform_objectScale, scale);
	glUseProgram(0);
}

void shaders_setObjectColor(float red, float green, float blue)
{
	glUseProgram(shaders_prog_base);
	glUniform3f(shaders_uniform_objectColor, red, green, blue);
	glUseProgram(0);
}

void shaders_setSceneLayout(float left, float top, float width, float height)
{
	float sceneLayoutMatrix[16] = {
			2/width, 0, 0, 0,
			0, -2/height, 0, 0,
			0, 0, 1, 0,
			-1-left, 1+top, 0, 1};
	glUseProgram(shaders_prog_base);
	glUniformMatrix4fv(
			shaders_uniform_sceneTransform, 1, false, sceneLayoutMatrix);
	glUseProgram(0);
}

GLuint shaders_getAttributeIndex_vertexPosition()
{
	return glGetAttribLocation(shaders_prog_base, "vertexPosition");
}


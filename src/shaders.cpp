
#define GLEW_STATIC

#include "shaders.h"
#include <GL/glew.h>
#include "errorHandling.h"

#include <iostream>
using namespace std;

const GLchar* SHADER_VERTEX_BASE =
	"uniform mat4 sceneTransform;\n"
	"uniform vec2 objectPosition;\n"
	"uniform vec2 objectScale;\n"
	"attribute vec2 vertexPosition;\n"
	"varying vec2 UVs;\n"
	"void main()\n"
	"{\n"
	"	UVs = vertexPosition;\n"
	"	gl_Position =\n"
	"			sceneTransform *\n"
	"			vec4(vertexPosition*objectScale+objectPosition, 0.0, 1.0);\n"
	"}\n\0";

const GLchar* SHADER_FRAGMENT_COLOR =
	"varying vec2 UVs;\n"
	"uniform vec3 objectColor;\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor = vec4(objectColor, 1.0);\n"
	"}\n\0";

const GLchar* SHADER_FRAGMENT_TEXTURE =
	"varying vec2 UVs;\n"
	"uniform sampler2D mainTex;\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor = texture2D(mainTex, UVs);\n"
	"}\n\0";
	
const GLchar* SHADER_FRAGMENT_BLUR =
	"varying vec2 UVs;\n"
	"uniform sampler2D mainTex;\n"
	"uniform float blurAmount;\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);\n"
	"	float blurExtent = 5.0*blurAmount;"
	"	for(float y = -blurExtent; y < blurExtent; y += blurAmount)\n"
	"	{\n"
	"		for(float x = -blurExtent; x < blurExtent; x += blurAmount)\n"
	"		{\n"
	"			gl_FragColor += texture2D(mainTex, UVs + vec2(x, y));"
	"		}\n"
	"	}\n"
	"	gl_FragColor /= 100.0;\n"
	"}\n\0";

GLuint shaders_color;
GLuint shaders_color_uniform_sceneTransform;
GLuint shaders_color_uniform_objectPosition;
GLuint shaders_color_uniform_objectScale;
GLuint shaders_color_uniform_objectColor;
GLuint shaders_texture;
GLuint shaders_texture_uniform_sceneTransform;
GLuint shaders_texture_uniform_objectPosition;
GLuint shaders_texture_uniform_objectScale;
GLuint shaders_texture_uniform_mainTex;
GLuint shaders_blur;
GLuint shaders_blur_uniform_sceneTransform;
GLuint shaders_blur_uniform_objectPosition;
GLuint shaders_blur_uniform_objectScale;
GLuint shaders_blur_uniform_mainTex;
GLuint shaders_blur_uniform_blurAmount;

void shaders_init()
{
    GLint result;

    glewExperimental = GL_TRUE;
    result = glewInit();
	if(result != GLEW_OK)
	{
		handleErrors(result, "glewInit:", (const char*)glewGetErrorString(result));
	}
	
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &SHADER_VERTEX_BASE, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    //if (!result)
    {
		GLchar infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "vert: " << infoLog << endl;
		//handleErrors(0, "vertex shader:", infoLog);
    }

    GLuint colorShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(colorShader, 1, &SHADER_FRAGMENT_COLOR, NULL);
    glCompileShader(colorShader);
    glGetShaderiv(colorShader, GL_COMPILE_STATUS, &result);
    //if (!result)
    {
		GLchar infoLog[512];
		glGetShaderInfoLog(colorShader, 512, NULL, infoLog);
		cout << "color: " << infoLog << endl;
		//handleErrors(0, "color shader:", infoLog);
    }

	GLuint textureShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(textureShader, 1, &SHADER_FRAGMENT_TEXTURE, NULL);
    glCompileShader(textureShader);
    glGetShaderiv(textureShader, GL_COMPILE_STATUS, &result);
    //if (!result)
    {
		GLchar infoLog[512];
		glGetShaderInfoLog(textureShader, 512, NULL, infoLog);
		cout << "texture: " << infoLog << endl;
		//handleErrors(0, "texture shader:", infoLog);
    }
	
	GLuint blurShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(blurShader, 1, &SHADER_FRAGMENT_BLUR, NULL);
    glCompileShader(blurShader);
    glGetShaderiv(blurShader, GL_COMPILE_STATUS, &result);
    //if (!result)
    {
		GLchar infoLog[512];
		glGetShaderInfoLog(blurShader, 512, NULL, infoLog);
		cout << "blur: " << infoLog << endl;
		//handleErrors(0, "texture shader:", infoLog);
    }
	
    shaders_color = glCreateProgram();
    glAttachShader(shaders_color, vertexShader);
    glAttachShader(shaders_color, colorShader);
    glLinkProgram(shaders_color);
    glGetProgramiv(shaders_color, GL_LINK_STATUS, &result);
    //if (!result)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shaders_color, 512, NULL, infoLog);
		cout << "color program: " << infoLog << endl;
		//handleErrors(0, "color shader program:", infoLog);
    }
	
    shaders_texture = glCreateProgram();
    glAttachShader(shaders_texture, vertexShader);
    glAttachShader(shaders_texture, textureShader);
    glLinkProgram(shaders_texture);
    glGetProgramiv(shaders_texture, GL_LINK_STATUS, &result);
    //if (!result)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shaders_texture, 512, NULL, infoLog);
		cout << "texture program: " << infoLog << endl;
		//handleErrors(0, "color texture program:", infoLog);
    }

	shaders_blur = glCreateProgram();
    glAttachShader(shaders_blur, vertexShader);
    glAttachShader(shaders_blur, blurShader);
    glLinkProgram(shaders_blur);
    glGetProgramiv(shaders_blur, GL_LINK_STATUS, &result);
    //if (!result)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shaders_blur, 512, NULL, infoLog);
		cout << "blur program: " << infoLog << endl;
		//handleErrors(0, "texture shader program:", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(colorShader);
	//glDeleteShader(textureShader);
	glDeleteShader(blurShader);

	cout << "color: " << shaders_color << endl;
	cout << "texture: " << shaders_texture << endl;
	cout << "blur: " << shaders_blur << endl;

	shaders_color_uniform_sceneTransform =
			glGetUniformLocation(shaders_color, "sceneTransform");
	shaders_color_uniform_objectPosition =
			glGetUniformLocation(shaders_color, "objectPosition");
	shaders_color_uniform_objectScale =
			glGetUniformLocation(shaders_color, "objectScale");
	shaders_color_uniform_objectColor =
			glGetUniformLocation(shaders_color, "objectColor");

	shaders_texture_uniform_sceneTransform =
			glGetUniformLocation(shaders_texture, "sceneTransform");
	shaders_texture_uniform_objectPosition =
			glGetUniformLocation(shaders_texture, "objectPosition");
	shaders_texture_uniform_objectScale =
			glGetUniformLocation(shaders_texture, "objectScale");
	shaders_texture_uniform_mainTex =
			glGetUniformLocation(shaders_texture, "mainTex");

	shaders_blur_uniform_sceneTransform =
			glGetUniformLocation(shaders_blur, "sceneTransform");
	shaders_blur_uniform_objectPosition =
			glGetUniformLocation(shaders_blur, "objectPosition");
	shaders_blur_uniform_objectScale =
			glGetUniformLocation(shaders_blur, "objectScale");
	shaders_blur_uniform_mainTex =
			glGetUniformLocation(shaders_blur, "mainTex");
	shaders_blur_uniform_blurAmount =
			glGetUniformLocation(shaders_blur, "blurAmount");
}

void shaders_setSceneLayout(float left, float top, float right, float bottom)
{
	float sceneLayoutMatrix[16] = {
			2/(right-left), 0, 0, 0,
			0, -2/(bottom-top), 0, 0,
			0, 0, 1, 0,
			-1-left/(right-left)*2, 1+top/(bottom-top)*2, 0, 1};

	glUseProgram(shaders_color);
	glUniformMatrix4fv(
			shaders_color_uniform_sceneTransform, 1, false, sceneLayoutMatrix);

	glUseProgram(shaders_texture);
	glUniformMatrix4fv(
			shaders_color_uniform_sceneTransform, 1, false, sceneLayoutMatrix);

	glUseProgram(shaders_blur);
	glUniformMatrix4fv(
			shaders_blur_uniform_sceneTransform, 1, false, sceneLayoutMatrix);

	glUseProgram(0);
}

GLuint shaders_getAttributeIndex_vertexPosition()
{
	return glGetAttribLocation(shaders_color, "vertexPosition");
}

void shaders_useColor(float xPos, float yPos, float xScale, float yScale, float red, float green, float blue)
{
	glUseProgram(shaders_color);
	glUniform2f(shaders_color_uniform_objectPosition, xPos, yPos);
	glUniform2f(shaders_color_uniform_objectScale, xScale, yScale);
	glUniform3f(shaders_color_uniform_objectColor, red, green, blue);
}

void shaders_useTexture(float xPos, float yPos, float xScale, float yScale, int textureId)
{
	glUseProgram(shaders_texture);
	glUniform2f(shaders_texture_uniform_objectPosition, xPos, yPos);
	glUniform2f(shaders_texture_uniform_objectScale, xScale, yScale);
	glUniform1f(shaders_texture_uniform_mainTex, textureId);
}


void shaders_useBlur(float xPos, float yPos, float xScale, float yScale, int textureId, float blurAmount)
{
	glUseProgram(shaders_blur);
	glUniform2f(shaders_blur_uniform_objectPosition, xPos, yPos);
	glUniform2f(shaders_blur_uniform_objectScale, xScale, yScale);
	glUniform1f(shaders_blur_uniform_mainTex, textureId);
	glUniform1f(shaders_blur_uniform_blurAmount, blurAmount);
}

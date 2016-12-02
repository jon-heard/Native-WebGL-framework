
#include "Shader.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <streambuf>
#include "errorHandling.h"

#include <iostream>

using namespace std;
using namespace platform;

// http://stackoverflow.com/questions/23177229/how-to-cast-int-to-const-glvoid
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

bool Shader::inited = false;
std::map<string, Shader*> Shader::shaderCollection;
Shader* Shader::currentShader = NULL;
float Shader::left = 0, Shader::top = 0;
float Shader::right = 1, Shader::bottom = 1;

const GLchar* SHADER_VERTEX_BASE =
	"uniform mat4 sceneTransform;\n"
	"uniform mat4 rotationTransform;\n"
	"uniform vec2 objectPosition;\n"
	"uniform vec2 objectScale;\n"
	"uniform vec2 objectRotation;\n"
	"attribute vec2 vertexPosition;\n"
	"attribute vec2 vertexTexCoord;\n"
	"varying vec2 UVs;\n"
	"void main()\n"
	"{\n"
	"	UVs = vertexTexCoord;\n"
	"	vec2 v = vec2(\n"
	"		vertexPosition.x * objectRotation.y + vertexPosition.y * objectRotation.x,\n"
	"		vertexPosition.y * objectRotation.y - vertexPosition.x * objectRotation.x);\n"
	"	gl_Position = \n"
	"		sceneTransform *\n"
	"		vec4(v * objectScale + objectPosition, 0, 1);\n"
	"}\n\0";

bool Shader::Shader_Init()
{
	glewExperimental = GL_TRUE;
	int result = glewInit();
	if(result != GLEW_OK)
	{
		handleErrors(
				result, "glewInit:",
				(const char*)glewGetErrorString(result));
	}
	return true;
}

Shader::Shader(const char* fragSourceFile)
{
	if(!inited)
	{
		inited = true;
		Shader::Shader_Init();
	}

	programId = 0;
	ready = false;

	GLint result;

	// Load fragment shader source file
	ifstream t(fragSourceFile);
	string sourceCode(
			(istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
	const char *sourceCode_cstr = sourceCode.c_str();

	// Compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &SHADER_VERTEX_BASE, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		handleErrors(0, "vertex shader :: ", infoLog);
	}

	// Compile the fragment shader
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &sourceCode_cstr, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
		handleErrors(0, fragSourceFile, " :: ", infoLog);
	}

	// Link the shader program
	programId = glCreateProgram();
	glAttachShader(programId, vertexShader);
	glAttachShader(programId, fragShader);
	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	if (!result)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(programId, 512, NULL, infoLog);
		handleErrors(0, fragSourceFile, " :: ", infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	// Add to shaders collection
	shaderCollection[fragSourceFile] = this;

	// Gather all attributes and uniforms
	int count = 0;
	char name[256];
	GLenum type;
	int size = 0;
	glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &count);
	for (int i = 0; i < count; i++)
	{
		glGetActiveUniform(
				programId, i, 256, NULL, &size, &type, name);
		int id = glGetUniformLocation(programId, name);
		parameters[name] = ShaderParamInfo(id, type, true);
	}
	glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &count);
	for (int i = 0; i < count; i++)
	{
		glGetActiveAttrib(
				programId, i, 256, NULL, &size, &type, name);
		int id = glGetAttribLocation(programId, name);
		parameters[name] = ShaderParamInfo(id, type, false);
	}

	// Setup layout matrix
	float sceneLayoutMatrix[16] = {
			2/(right-left), 0, 0, 0,
			0, -2/(bottom-top), 0, 0,
			0, 0, 1, 0,
			-1-left/(right-left)*2, 1+top/(bottom-top)*2, 0, 1};
	glUseProgram(programId);
	glUniformMatrix4fv(
			parameters["sceneTransform"].id,
			1, false, sceneLayoutMatrix);

	ready = true;
}

Shader::~Shader()
{
	glDeleteProgram(programId);
	map<string, Shader*>::iterator itr = shaderCollection.begin();
	while (itr != shaderCollection.end())
	{
		Shader* curVal = (*itr).second;
		if (curVal == this)
		{
		   shaderCollection.erase(itr);
		   break;
		}
		++itr;
	}
}

bool Shader::useShader(const char* name)
{
	if(shaderCollection.find(name) == shaderCollection.end())
	{
		return false;
	}
	currentShader = shaderCollection[name];
	glUseProgram(currentShader->programId);

	map<string, ShaderParamInfo>* params =
			&(currentShader->parameters);
	map<string, ShaderParamInfo>::iterator i = params->begin();
	while(i != params->end())
	{
		if(!(*i).second.isUniform)
		{
			glDisableVertexAttribArray((*i).second.id);
		}
		i++;
	}

	return true;
}

const ShaderParamInfo* Shader::getParameterInfo(const char* name)
{
	if(currentShader == NULL)
	{
		return NULL;
	}
	map<string, ShaderParamInfo>* params =
			&(currentShader->parameters);

	if(params->find(name) == params->end())
	{
		return NULL;
	}
	return &(*params)[name];
}

bool Shader::setParameter_float(const char* name, float value1)
{
	const ShaderParamInfo* info = getParameterInfo(name);
	if(info == NULL || info->type != GL_FLOAT || !info->isUniform)
	{
		return false;
	}
	glUniform1f(info->id, value1);
	return true;
}
bool Shader::setParameter_vec2(const char* name, float value1, float value2)
{
	const ShaderParamInfo* info = getParameterInfo(name);
	if(info == NULL || info->type != GL_FLOAT_VEC2 || !info->isUniform)
	{
		return false;
	}
	glUniform2f(info->id, value1, value2);
	return true;
}
bool Shader::setParameter_vec3(const char* name,
		float value1, float value2, float value3)
{
	const ShaderParamInfo* info = getParameterInfo(name);
	if(info == NULL || info->type != GL_FLOAT_VEC3 || !info->isUniform)
	{
		return false;
	}
	glUniform3f(info->id, value1, value2, value3);
	return true;
}
bool Shader::setParameter_vec4(const char* name,
		float value1, float value2, float value3, float value4)
{
	const ShaderParamInfo* info = getParameterInfo(name);
	if(info == NULL || info->type != GL_FLOAT_VEC4 || !info->isUniform)
	{
		return false;
	}
	glUniform4f(info->id, value1, value2, value3, value4);
	return true;
}
bool Shader::setParameter_mat4(const char* name, float* value1)
{
	const ShaderParamInfo* info = getParameterInfo(name);
	if(info == NULL || info->type != GL_FLOAT_MAT4 || !info->isUniform)
	{
		return false;
	}
	glUniformMatrix4fv(info->id, 1, false, value1);
	return true;
}
bool Shader::setParameter_Texture1(const char* name, unsigned int textureId1)
{
	const ShaderParamInfo* info = getParameterInfo(name);
	if(info == NULL || info->type != GL_SAMPLER_2D || !info->isUniform)
	{
		return false;
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId1);
	glUniform1i(info->id, 0);
	return true;
}
bool Shader::setParameter_Texture2(
		const char* name1, unsigned int textureId1,
		const char* name2, unsigned int textureId2)
{
	const ShaderParamInfo* info1 = getParameterInfo(name1);
	if(info1 == NULL || info1->type != GL_SAMPLER_2D || !info1->isUniform)
	{
		return false;
	}
	const ShaderParamInfo* info2 = getParameterInfo(name2);
	if(info2 == NULL || info2->type != GL_SAMPLER_2D || !info2->isUniform)
	{
		return false;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId1);
	glUniform1i(info1->id, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureId2);
	glUniform1i(info2->id, 1);

	return true;
}

bool Shader::bindAttribute(
		const char* name, unsigned int attributeSize,
		unsigned int fragmentSize, unsigned int offset)
{
	const ShaderParamInfo* info = getParameterInfo(name);
	if(info == NULL || info->isUniform)
	{
		return false;
	}
	int vertexPosition = Shader::getParameterInfo("vertexPosition")->id;
	glVertexAttribPointer(
			info->id, attributeSize, GL_FLOAT,
			GL_FALSE, fragmentSize*4, BUFFER_OFFSET(offset*4));
	glEnableVertexAttribArray(info->id);
	return true;
}


void Shader::setSceneLayout(
		float left, float top, float right, float bottom)
{
	Shader::left = left;
	Shader::right = right;
	Shader::top = top;
	Shader::bottom = bottom;
	float sceneLayoutMatrix[16] = {
			2/(right-left), 0, 0, 0,
			0, -2/(bottom-top), 0, 0,
			0, 0, 1, 0,
			-1-left/(right-left)*2, 1+top/(bottom-top)*2, 0, 1};

	map<string, Shader*>::iterator itr = shaderCollection.begin();
	while (itr != shaderCollection.end())
	{
		Shader* curVal = (*itr).second;
		glUseProgram(curVal->programId);
		glUniformMatrix4fv(
				curVal->parameters["sceneTransform"].id,
				1, false, sceneLayoutMatrix);
		++itr;
	}
}

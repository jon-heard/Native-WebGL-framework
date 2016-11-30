
#include "draw.h"
#include "Platform.h"
#include <map>
#define _USE_MATH_DEFINES
#include <math.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include "errorHandling.h"
#include "Shader.h"

#include <iostream>

using namespace std;

// http://stackoverflow.com/questions/23177229/how-to-cast-int-to-const-glvoid
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace platform
{
//	typedef struct
//	{
//		float red; float green; float blue;
//	}
//	Color;
//
//	const Color COLORS[] = {
//		{1, 0, 0},			// 00 - Red
//		{0, 1, 0},			// 01 - Green
//		{0, 0, 1},			// 02 - Blue
//		{.5f, 0, .5f},		// 03 - purple
//		{.5f, .5f, .5f},	// 04 - gray
//		{0, 0, 0},			// 05 - black
//		{1, 1, 1},			// 06 - white
//	};

	unsigned int circleBuffer = 0;
	unsigned int rectangleBuffer = 0;

	map<const char*, int> images;

	Shader* colorShader = NULL;
	Shader* textureShader = NULL;
	Shader* coloredTextureShader = NULL;
	Shader* blurShader = NULL;

	int nextColor = -1;
	float nextOpacity = 1;
	float nextRotation = 0;
	bool nextShader = false;

	void draw_init()
	{
		int result = glewInit();
		if(result != GLEW_OK)
		{
			handleErrors(
					result, "glewInit:",
					(const char*)glewGetErrorString(result));
		}

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
					GL_ARRAY_BUFFER, sizeof(circleData),
					circleData, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		// Setup data for rectangles
		{
			GLfloat rectangleData[] =
			{
				-.5f, -.5f,		0,1,
				-.5f, .5f,		0,0,
				.5f, .5f,		1,0,
				.5f, -.5f,		1,1,
				-.5f, -.5f,		0,1
			};
			glGenBuffers(1, &rectangleBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, rectangleBuffer);
			glBufferData(
					GL_ARRAY_BUFFER, sizeof(rectangleData),
					rectangleData, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		// Setup shaders
		if(colorShader == NULL)
		{
			colorShader = new Shader("media/frag_color.txt");
			textureShader = new Shader("media/frag_texture.txt");
			coloredTextureShader = new Shader("media/frag_coloredtexture.txt");
			blurShader = new Shader("media/frag_blur.txt");
		}
	}

	void draw_shutdown()
	{
		glDeleteBuffers(1, &circleBuffer);
		glDeleteBuffers(1, &rectangleBuffer);
		delete colorShader;
		delete textureShader;
		delete coloredTextureShader;
		delete blurShader;
	}

	void drawCircle(float x, float y, float radius, bool filled)
	{
		if(!nextShader) Shader::useShader("media/frag_color.txt");
		if(nextColor == -1) nextColor = 6;
		Color c = COLORS[nextColor];
		Shader::setParameter_vec3("objectColor", c.red, c.green, c.blue);
		Shader::setParameter_vec2("objectPosition", x, y);
		Shader::setParameter_vec2("objectScale", radius, radius);
		Shader::setParameter_float("objectOpacity", nextOpacity);

		glBindBuffer(GL_ARRAY_BUFFER, circleBuffer);

		int vertexPosition = Shader::getParameterInfo("vertexPosition")->id;
		glVertexAttribPointer(
				vertexPosition, 2, GL_FLOAT,
				GL_FALSE, 0, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(vertexPosition);

		glDrawArrays(
				filled ? GL_TRIANGLE_FAN : GL_LINE_STRIP,
				0, CIRCLE_RESOLUTION);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		nextColor = -1;
		nextOpacity = 1;
		nextRotation = 0;
		nextShader = false;
	}

	void drawRectangle(float x, float y, float sizeX, float sizeY, bool filled)
	{
		if(!nextShader) Shader::useShader("media/frag_color.txt");
		if(nextColor == -1) nextColor = 6;
		Color c = COLORS[nextColor];
		Shader::setParameter_vec3("objectColor", c.red, c.green, c.blue);
		Shader::setParameter_vec2("objectPosition", x, y);
		Shader::setParameter_vec2("objectScale", sizeX, sizeY);
		Shader::setParameter_float("objectOpacity", nextOpacity);
		Shader::setParameter_vec2(
				"objectRotation",sin(nextRotation),cos(nextRotation));

		glBindBuffer(GL_ARRAY_BUFFER, rectangleBuffer);

		int vertexPosition = Shader::getParameterInfo("vertexPosition")->id;
		glVertexAttribPointer(
				vertexPosition, 2, GL_FLOAT,
				GL_FALSE, 16, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(vertexPosition);

		glDrawArrays(filled ? GL_TRIANGLE_FAN : GL_LINE_STRIP, 0, 5);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		nextColor = -1;
		nextOpacity = 1;
		nextRotation = 0;
		nextShader = false;
	}


	int loadImage(const char* filename)
	{
		int result = SOIL_load_OGL_texture(
				filename, SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		images[filename] = result;
		return result;
		return 0;
	}

	int drawImage(
			float x, float y, float sizeX, float sizeY, const char* filename)
	{
		if(images.find(filename) == images.end())
		{
			loadImage(filename);
		}
		int imageId = images[filename];
		drawImage(x, y, sizeX, sizeY, imageId);
		return imageId;
	}

	int drawImage(float x, float y, float sizeX, float sizeY, int imageId)
	{
		if(!nextShader)
		{
			if(nextColor == -1)
			{
				Shader::useShader("media/frag_texture.txt");
			}
			else
			{
				Shader::useShader("media/frag_coloredtexture.txt");
			}
		}
		Color c = COLORS[nextColor];
		Shader::setParameter_vec3("objectColor", c.red, c.green, c.blue);
		Shader::setParameter_vec2("objectPosition", x, y);
		Shader::setParameter_vec2("objectScale", sizeX, sizeY);
		Shader::setParameter_Texture1("mainTex", imageId);
		Shader::setParameter_float("objectOpacity", nextOpacity);
		Shader::setParameter_vec2(
				"objectRotation",sin(nextRotation),cos(nextRotation));

		glBindBuffer(GL_ARRAY_BUFFER, rectangleBuffer);

		int vertexPosition = Shader::getParameterInfo("vertexPosition")->id;
		glVertexAttribPointer(
				vertexPosition, 2, GL_FLOAT,
				GL_FALSE, 16, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(vertexPosition);

		int vertexTexCoord = Shader::getParameterInfo("vertexTexCoord")->id;
		glVertexAttribPointer(
				vertexTexCoord, 2, GL_FLOAT,
				GL_FALSE, 16, BUFFER_OFFSET(8));
		glEnableVertexAttribArray(vertexTexCoord);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

		glDisableVertexAttribArray(vertexPosition);
		glDisableVertexAttribArray(vertexTexCoord);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		nextColor = -1;
		nextOpacity = 1;
		nextRotation = 0;
		nextShader = false;

		return imageId;
	}

	void setNextDraw_color(int color)
	{
		nextColor = color;
	}

	void setNextDraw_opacity(float opacity)
	{
		nextOpacity = opacity;
	}

	void setNextDraw_rotation(float rotation)
	{
		nextRotation = rotation;
	}

	void setNextDraw_useCustomShader(bool useCustomShader)
	{
		nextShader = useCustomShader;
	}
}

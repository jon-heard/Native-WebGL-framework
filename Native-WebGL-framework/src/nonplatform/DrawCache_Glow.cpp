#include "DrawCache_Glow.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "../platform/Platform.h"
#include "../platform/Shader.h"

#include <iostream>

using namespace std;
using namespace platform;

unsigned int DrawCache_Glow::instanceCount = 0;
Shader* DrawCache_Glow::blurShader = NULL;
Shader* DrawCache_Glow::combineShader = NULL;
DrawCache* DrawCache_Glow::glow = NULL;
DrawCache* DrawCache_Glow::combined = NULL;

DrawCache_Glow::DrawCache_Glow()
{
	if(instanceCount == 0)
	{
		DrawCache_Glow::blurShader = new Shader("media/frag_blur.txt");
		DrawCache_Glow::combineShader = new Shader("media/frag_combine.txt");
		DrawCache_Glow::glow = new DrawCache();
		DrawCache_Glow::combined = new DrawCache();
	}
	instanceCount++;
}
DrawCache_Glow::~DrawCache_Glow()
{
	instanceCount--;
	if(instanceCount == 0)
	{
		delete DrawCache_Glow::blurShader;
		delete DrawCache_Glow::combineShader;
		delete DrawCache_Glow::glow;
		delete DrawCache_Glow::combined;
		DrawCache_Glow::blurShader = NULL;
		DrawCache_Glow::combineShader = NULL;
		DrawCache_Glow::glow = NULL;
		DrawCache_Glow::combined = NULL;
	}
}

void DrawCache_Glow::flush()
{
//	// Glow
	glow->start();
	Shader::useShader("media/frag_blur.txt");
	Shader::setParameter_float("blurAmount", 0.005f);
	Shader::setParameter_float("blurBrightness", 3);
	setNextDraw_useCustomShader(true);
	drawImage(0, 0, RIGHT-LEFT, BOTTOM-TOP, textureId);
	glow->stop();
	glow->flush();
	Shader::setParameter_float("blurBrightness", 1);

	drawImage(0, 0, RIGHT-LEFT, BOTTOM-TOP, textureId);
}


#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "platform/platform.h"
#include "platform/DrawCache.h"
#include "platform/Shader.h"

using namespace std;

void init();
void frameLogic();
void cleanup();

platform::DrawCache* drawCache = NULL;

int main()
{
	platform::run(init, frameLogic, cleanup);
}

void init()
{
	platform::setTitle("Base app");
	platform::setBackgroundColor(.25f, .25f, .25f);
	platform::setTextSize(40);
	drawCache = new platform::DrawCache();
}

float rotationPhase = 0;
void frameLogic()
{
	drawCache->start();

	platform::setNextDraw_color(3);
	platform::drawCircle(0, 0, 300, false);
	platform::setNextDraw_color(0);
	platform::drawCircle(0, 0, 150, false);
	platform::setNextDraw_color(0);
	platform::drawRectangle(0, 0, 150, 150, true);
	platform::setNextDraw_color(1);
	platform::drawRectangle(100, 0, 30, 50, false);
	platform::setNextDraw_color(1);
	platform::drawRectangle(-100, 0, 30, 50, true);
	platform::setNextDraw_color(1);
	platform::drawRectangle(0, 100, 50, 30, false);
	platform::setNextDraw_color(1);
	platform::setNextDraw_opacity(.5f);
	platform::drawRectangle(0, -100, 50, 30, true);

	platform::setNextDraw_color(1);
	platform::drawCircle(
			cos(rotationPhase) * 100, sin(rotationPhase) * 100, 30, false);
	rotationPhase += .025f;


	drawCache->stop();
	drawCache->flush();

	int color = 3;
	if(platform::isMouseDown())
	{
		color = 2;
	}
	//cout << platform::getMouseX() << ", " << platform::getMouseY() << endl;
	platform::setNextDraw_color(color);
	platform::drawCircle(
			platform::getMouseX(), platform::getMouseY(), 50, true);

	platform::drawImage(-150, -150, 150, 150, "media/colorTest.png");

	platform::Shader::useShader("media/frag_blur.txt");
	platform::Shader::setParameter_float("blurAmount", .005f);
	platform::setNextDraw_useCustomShader(true);
	platform::drawImage(150, 150, 150, 150, "media/flag.png");

	platform::setNextDraw_color(1);
	platform::setNextDraw_rotation(rotationPhase);
	platform::drawImage(0, 0, 64, 64, "media/ligands_3.png");

	platform::drawText(0,0,"hello world!");
}

void cleanup()
{
	delete drawCache;
}

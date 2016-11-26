
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "platform/platform.h"
#include "platform/DrawCache.h"

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
	platform::setTextSize(16);
//	drawCache = new platform::DrawCache();
}

float rotationPhase = 0;
void frameLogic()
{
//	drawCache->start();

	platform::drawCircle(0, 0, 300, 3, false);
	platform::drawCircle(0, 0, 150, 0, false);
	platform::drawRectangle(0, 0, 150, 150, 0, true);
	platform::drawRectangle(100, 0, 30, 50, 1, false);
	platform::drawRectangle(-100, 0, 30, 50, 1, true);
	platform::drawRectangle(0, 100, 50, 30, 1, false);
	platform::drawRectangle(0, -100, 50, 30, 1, true, .5f);
	platform::drawText(-225,0,5,"hello world!");

	platform::drawCircle(
			sin(-rotationPhase) * 100,
			cos(-rotationPhase) * 100,
			30, 1, false);
	rotationPhase += .025f;


//	drawCache->stop();
//	drawCache->flush();

	int color = 3;
	if(platform::isMouseDown())
	{
		color = 2;
	}
	//cout << platform::getMouseX() << ", " << platform::getMouseY() << endl;
	platform::drawCircle(
			platform::getMouseX(), platform::getMouseY(), 50, color, true);
	platform::drawImage(
			-150,-150,150,150,
			"media/colorTest.png");
	platform::drawImage(150,150,150,150, "media/flag.png");
}

void cleanup()
{
	delete drawCache;
}


#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "errorHandling.h"
#include "platform.h"

using namespace std;

void init();
void frameLogic();
void cleanup();

int main()
{
	platform::run(init, frameLogic, cleanup);
}

void init()
{
	platform::setTitle("Base app");
	platform::setBackgroundColor(.25f, .25f, .25f);
}

float rotationPhase = 0;
void frameLogic()
{
	platform::cacheDraws();
	platform::drawCircle(0, 0, 1000, 4, false);
	platform::drawCircle(0, 0, 500, 0, false);
	platform::drawCircle(
			sin(rotationPhase) * 375, cos(rotationPhase) * 375, 100, 1, false);
	platform::flushDrawCacheWithBlur();
	int color = 3;
	if(platform::isMouseDown())
	{
		color = 2;
	}
	platform::drawCircle(
			platform::getMouseX(), platform::getMouseY(), 25, color, true);
	rotationPhase += .025f;
	
	//platform::drawImage(-500,-500,500,500, "cap.png", rotationPhase*4);
	//platform::drawImage(0,0,500,500, "flag.png", rotationPhase*4);
}

void cleanup()
{

}
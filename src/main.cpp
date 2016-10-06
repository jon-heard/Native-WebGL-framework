
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
	platform::setBackgroundColor(0, 0, 0);
}

float rotationPhase = 0;
void frameLogic()
{
	platform::drawCircle(false, 4, 0, 0, 1000);
	platform::drawCircle(false, 0, 0, 0, 500);
	platform::drawCircle(
			false, 1, sin(rotationPhase) * 375, cos(rotationPhase) * 375, 100);
	int color = 3;
	if(platform::isMouseDown())
	{
		color = 2;
	}
	platform::drawCircle(
			true, color, platform::getMouseX(), platform::getMouseY(), 25);
	rotationPhase += .025f;
}

void cleanup()
{

}

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
	platform_run(init, frameLogic, cleanup);
}

void init()
{
	platform_setTitle("Base app");
	platform_setBackgroundColor(0, 0, 0);
}

float rotationPhase = 0;
void frameLogic()
{
	platform_drawCircle(false, 4, 0, 0, 1000);
	platform_drawCircle(false, 0, 0, 0, 500);
	platform_drawCircle(
			false, 1, sin(rotationPhase) * 375, cos(rotationPhase) * 375, 100);
	int color = 3;
	if(platform_isMouseDown())
	{
		color = 2;
	}
	platform_drawCircle(
			true, color, platform_getMouseX(), platform_getMouseY(), 25);
	rotationPhase += .025f;
}

void cleanup()
{

}
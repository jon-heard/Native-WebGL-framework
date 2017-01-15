
#include <iostream>
#include <math.h>
#include "../platform/platform.h"

using namespace std;

float rad2pi = 1;//180.0 / 3.141592653589793;

void drawLine(int x1, int y1, int x2, int y2, int width)
{
	float rotation = atan2(x1-x2, y1-y2);
	float length = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
	float avgX = (x1+x2)*.5;
	float avgY = (y1+y2)*.5;
	platform::setNextDraw_rotation(rotation);
	platform::drawImage(avgX, avgY, width*10, length, "media/line.png");
}

void drawArrow(int x1, int y1, int x2, int y2, int width, int headWidth)
{
	if(headWidth <= 0) { headWidth = width; }
	platform::Color color = platform::getNextDraw_color();
	float rotation = atan2(x1-x2, y1-y2);
	x2 += sin(rotation) * headWidth;
	y2 += cos(rotation) * headWidth;
	float length = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
	float avgX = (x1+x2)*.5;
	float avgY = (y1+y2)*.5;
	platform::setNextDraw_rotation(rotation);
	platform::drawImage(avgX, avgY, width*10, length, "media/line.png");
	platform::setNextDraw_rotation(rotation);
	platform::setNextDraw_color(color);
	platform::drawImage(x2, y2, headWidth*10, headWidth*2, "media/arrow.png");
}

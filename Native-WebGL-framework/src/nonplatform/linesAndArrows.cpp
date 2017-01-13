
#include <iostream>
#include <math.h>
#include "../platform/platform.h"

using namespace std;

float rad2pi = 1;//180.0 / 3.141592653589793;

void drawLine(int x1, int y1, int x2, int y2, int width)
{
	float avgX = (x1+x2)*.5;
	float avgY = (y1+y2)*.5;
	float length = sqrt((x1-x2)^2 + (y1-y2)^2);
	//platform::setNextDraw_rotation(atan2(x1-x1, y1-y2));
	platform::setNextDraw_rotation(atan2(x1-x2, y1-y2)*rad2pi);
	platform::drawImage(avgX, avgY, width, length, "media/line.png");
	cout << length << endl;
}

void drawArrow(int x1, int y1, int x2, int y2, int width)
{

}

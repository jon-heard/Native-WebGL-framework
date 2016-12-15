
// Just included in platform.cpp for init and shutdown.  Most functions are
// referenced in platform.h
#ifndef DRAW_H_
#define DRAW_H_

namespace platform
{
	void draw_init();
	void draw_shutdown();

	typedef struct Color
	{
		float red; float green; float blue;

		Color(float red, float green, float blue)
		{
			this->red = red;
			this->green = green;
			this->blue = blue;
		}
	} Color;

	const Color COLORS[] = {
		{1, 0, 0},			// 00 - Red
		{0, 1, 0},			// 01 - Green
		{0, 0, 1},			// 02 - Blue
		{.5f, 0, .5f},		// 03 - purple
		{.5f, .5f, .5f},	// 04 - gray
		{0, 0, 0},			// 05 - black
		{1, 1, 1},			// 06 - white
	};

	extern Color nextColor;
	extern float nextOpacity;
	extern float nextRotation;
	extern bool nextShader;

	extern unsigned int rectangleBuffer;

	void setNextDraw_color(Color color);
}

#endif /* DRAW_H_ */

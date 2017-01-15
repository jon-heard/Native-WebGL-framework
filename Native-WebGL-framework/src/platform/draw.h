
// Just included in platform.cpp for init and shutdown.  Most functions are
// referenced in platform.h
#ifndef DRAW_H_
#define DRAW_H_

#include "platform.h"

namespace platform
{
	void draw_init();
	void draw_shutdown();

	const Color COLORS[] = {
		Color(1, 0, 0),			// 00 - Red
		Color(0, 1, 0),			// 01 - Green
		Color(0, 0, 1),			// 02 - Blue
		Color(.5f, 0, .5f),		// 03 - purple
		Color(.5f, .5f, .5f),	// 04 - gray
		Color(0, 0, 0),			// 05 - black
		Color(1, 1, 1),			// 06 - white
	};

	extern Color nextColor;
	extern float nextOpacity;
	extern float nextRotation;
	extern bool nextShader;
//
//	extern unsigned int rectangleBuffer;
//
//	void setNextDraw_color(Color color);
}

#endif /* DRAW_H_ */

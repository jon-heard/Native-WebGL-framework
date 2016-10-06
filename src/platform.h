#ifndef PLATFORM_H
#define PLATFORM_H

namespace platform
{
	void  run(void (*init)(), void (*frameLogic)(), void (*cleanup)());

	void  setTitle(const char* title);
	void  setBackgroundColor(float red, float green, float blue);

	void  drawCircle(
			bool diskOrCircle, int colorIndex, float x, float y, float radius);

	bool  isMouseDown();
	float getMouseX();
	float getMouseY();
}

#endif // PLATFORM_H

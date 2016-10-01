#ifndef PLATFORM_H
#define PLATFORM_H

void  platform_run(void (*init)(), void (*frameLogic)(), void (*cleanup)());
		
void  platform_setTitle(const char* title);
void  platform_setBackgroundColor(float red, float green, float blue);

void  platform_drawCircle(
		bool diskOrCircle, int colorIndex, float x, float y, float radius);

bool  platform_isMouseDown();
float platform_getMouseX();
float platform_getMouseY();

#endif // PLATFORM_H

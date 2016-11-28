#ifndef PLATFORM_H
#define PLATFORM_H

namespace platform
{
	// Application settings
	const int CIRCLE_RESOLUTION = 50;
   #ifdef EMSCRIPTEN
      const unsigned int WIN_WIDTH = 600, WIN_HEIGHT = 600;
   #else
      const unsigned int WIN_WIDTH = 900, WIN_HEIGHT = 900;
   #endif
   const int LEFT = -300, RIGHT = 300, TOP = -300, BOTTOM = 300;

	// Environment
	void run(void (*init)(), void (*frameLogic)(), void (*cleanup)());
	void setTitle(const char* title);
	void setBackgroundColor(float red, float green, float blue);
	unsigned long getTime();

	// Draw
	void drawCircle(
			float x, float y, float radius, int colorIndex, bool filled, float opacity = 1);
	void drawRectangle(
			float x, float y, float xSize, float ySize,
			int colorIndex, bool filled, float opacity = 1, float rotation = 0);
	int  loadImage(const char* filename);
	int  drawImage(
			float x, float y, float sizeX, float sizeY,
			const char* filename, float opacity = 1, float rotation = 0);
	int  drawImage(
			float x, float y, float sizeX, float sizeY,
			int textureId, float opacity = 1, float rotation = 0);

	// Text
	void setTextSize(unsigned int size);
	void drawText(
			float x, float y, int colorIndex,
			const char* toDraw, float opacity = 1, float rotation = 0);

	// Input
	bool  isMouseDown();
	float getMouseX();
	float getMouseY();
	bool isMouseClicked();

//	void cacheDraws();
//	void flushDrawCache();
//	void flushDrawCacheWithBlur();

}

#endif // PLATFORM_H

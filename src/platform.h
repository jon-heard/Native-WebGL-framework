#ifndef PLATFORM_H
#define PLATFORM_H

namespace platform
{
	void run(void (*init)(), void (*frameLogic)(), void (*cleanup)());

	void setTitle(const char* title);
	void setBackgroundColor(float red, float green, float blue);

	void drawCircle(
			float x, float y, float radius, int colorIndex, bool diskOrCircle);

	int  loadImage(const char* filename);
	int  drawImage(
			float x, float y, float sizeX, float sizeY, const char* filename, float rotation = 0);
	int  drawImage(
			float x, float y, float sizeX, float sizeY, int textureId, float rotation = 0);

	void cacheDraws();
	void flushDrawCache();
	void flushDrawCacheWithBlur();

	bool  isMouseDown();
	float getMouseX();
	float getMouseY();
}

#endif // PLATFORM_H

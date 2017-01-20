#ifndef PLATFORM_H
#define PLATFORM_H

namespace platform
{
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
	void drawCircle(float x, float y, float radius, bool filled);
	void drawRectangle(float x, float y, float xSize, float ySize, bool filled);
	int  loadImage(const char* filename);
	int  drawImage(
			float x, float y, float sizeX, float sizeY, const char* filename);
	int  drawImage(float x, float y, float sizeX, float sizeY, int textureId);
	void setNextDraw_color(Color color);
	void setNextDraw_color(float red, float green, float blue);
	void setNextDraw_color(int color);
	void setNextDraw_opacity(float opacity);
	void setNextDraw_rotation(float rotation);
	void setNextDraw_useCustomShader(bool useCustomShader);
	Color getNextDraw_color();


	// Text
	unsigned int getTextWidth(const char* text);
	unsigned int getDefaultFontSize();
	void setNextDraw_fontSize(unsigned int size);
	void setNextDraw_textWidth(unsigned int value);
	void drawText(float x, float y, const char* toDraw);

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

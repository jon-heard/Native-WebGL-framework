
#include "Platform.h"
#include "errorHandling.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

using namespace std;

// http://stackoverflow.com/questions/23177229/how-to-cast-int-to-const-glvoid
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace platform
{
	FT_Library ft;
	FT_Face face;

	void text_init()
	{
		if (FT_Init_FreeType(&ft))
			handleErrors(0, "FREETYPE: Could not init FreeType Library");
		if (FT_New_Face(ft, "media/times.ttf", 0, &face))
			handleErrors(0, "FREETYPE: Failed to load font");
		FT_Set_Pixel_Sizes(face, 0, 12);
	}

	void text_shutdown()
	{
	}

	void setTextSize(unsigned int size)
	{
		FT_Set_Pixel_Sizes(face, 0, size);
	}

	void drawText(
			float x, float y, int colorIndex,
			const char* toDraw, float opacity, float rotation)
	{
	}
}

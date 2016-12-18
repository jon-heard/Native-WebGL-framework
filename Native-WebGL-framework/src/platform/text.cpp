
#include "Platform.h"
#include "errorHandling.h"
#include <map>
#include <math.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <gl/gl.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "draw.h"
#include "shader.h"

#include <iostream>

using namespace std;

// http://stackoverflow.com/questions/23177229/how-to-cast-int-to-const-glvoid
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace platform
{
	struct ivec2
	{
		int x, y;
		ivec2() { x = y = 0; }
		ivec2(int x, int y)
		{
			this->x = x;
			this->y = y;
		}
	};
	struct Character
	{
		unsigned int textureId;
		ivec2        size;
		ivec2        bearing;
		FT_Pos       advance;
	};

	FT_Library ft;
	FT_Face face;
	map<unsigned int, map<char, Character>*> glyphSets;
	unsigned int nextSize, defaultSize;

	void setupGlyphSet(unsigned int size);
	void clearGlyphSets();
	void clearGlyphSet(map<char, Character>* set);

	void text_init(unsigned int pDefaultSize)
	{
		nextSize = defaultSize = pDefaultSize;
		if (FT_Init_FreeType(&ft))
			handleErrors(0, "FREETYPE: Could not init FreeType Library");
		if (FT_New_Face(ft, "media/times.ttf", 0, &face))
			handleErrors(0, "FREETYPE: Failed to load font");
		FT_Set_Pixel_Sizes(face, 0, defaultSize);
		setupGlyphSet(defaultSize);
	}

	void text_shutdown()
	{
		clearGlyphSets();
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	void setNextDraw_fontSize(unsigned int size)
	{
		if(glyphSets.find(size) == glyphSets.end())
		{
			setupGlyphSet(size);
		}
		nextSize = size;
	}

	void drawText(float x, float y, const char* toDraw)
	{
		if(!nextShader) Shader::useShader("media/frag_coloredtexture.txt");
		int drawOpacity = nextOpacity;
		Color drawColor = (nextColor.red==-1) ? COLORS[5] : nextColor;
		map<char, Character>* glyphSet = glyphSets[nextSize];

		int xOffset = 0;
		// Iterate through all characters
		for (int i = 0; toDraw[i] != '\0'; i++)
		{
			Character ch = (*glyphSet)[toDraw[i]];
			platform::setNextDraw_color(drawColor);
			platform::setNextDraw_opacity(drawOpacity);
			platform::setNextDraw_useCustomShader(true);
			platform::drawImage(
					x + ch.bearing.x + xOffset + 16,//ch.size.x/2,
					y - ch.size.y + ch.bearing.y - 16,//ch.size.y/2,
					32,//ch.size.x,
					32,//ch.size.y,
					ch.textureId);
			xOffset += (ch.advance >> 6);
		}

		nextSize = defaultSize;
	}

	void setupGlyphSet(unsigned int size)
	{
		if(glyphSets.find(size) != glyphSets.end())
		{
			return;
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
		FT_Set_Pixel_Sizes(face, 0, size);

		map<char, Character>* newSet = new map<char, Character>();
		glyphSets[size] = newSet;

		for (GLubyte c = 0; c < 128; c++)
		{
		    // Load character glyph
		    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		    {
		        cout << "ERROR::FREETYTPE: Failed to load Glyph" << endl;
		        continue;
		    }

		    // Flip bitmap buffer
		    int width = face->glyph->bitmap.width;
		    int height = face->glyph->bitmap.rows;
		    char* data = new char[32*32];
		    memset(data, 0, 32*32);
		    for(int row = 0; row < height; row++)
		    {
		    	memcpy(
		    		data + 32/*width*/ * row,
					face->glyph->bitmap.buffer + width * (height - row - 1),
					sizeof(char) * width );
		    }

		    // Generate texture
		    GLuint texture;
		    glGenTextures(1, &texture);
		    glBindTexture(GL_TEXTURE_2D, texture);
		    // Set texture options
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		    glTexImage2D(
		        GL_TEXTURE_2D,
		        0,
				GL_ALPHA,
		        32,//face->glyph->bitmap.width,
		        32,//face->glyph->bitmap.rows,
		        0,
				GL_ALPHA,
		        GL_UNSIGNED_BYTE,
				data
		    );

		    // Now store character for later use
		    Character character = {
		        texture,
		        ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
		        ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
		        face->glyph->advance.x
		    };
		    // Hack to adjust characters beneath the line
		    if(c == 'g' || c == 'j' || c == 'p' || c == 'q' || c == 'y' || c == 'Q')
		    {
		    	character.bearing.y += size / 2.5;
		    }
		    newSet->insert(std::pair<char, Character>(c, character));
		}
	}

	void clearGlyphSets()
	{
		for(map<unsigned int, map<char, Character>*>::iterator i = glyphSets.begin();
			i != glyphSets.end(); i++)
		{
			clearGlyphSet(i->second);
			delete i->second;
		}
	}
	void clearGlyphSet(map<char, Character>* set)
	{
		for(map<char, Character>::iterator i = set->begin();
			i != set->end(); i++)
		{
			glDeleteTextures(1, &(*i).second.textureId);
		}
	}
}

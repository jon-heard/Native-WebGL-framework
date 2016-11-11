#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>
#include "errorHandling.h"

void shaders_init();
void shaders_setSceneLayout(float left, float top, float width, float height);
GLuint shaders_getAttributeIndex_vertexPosition();

void shaders_useColor(float xPos, float yPos, float xScale, float yScale, float red, float green, float blue);
void shaders_useTexture(float xPos, float yPos, float xScale, float yScale, int textureId);
void shaders_useBlur(float xPos, float yPos, float xScale, float yScale, int textureId, float blurAmount);

#endif // SHADERS_H
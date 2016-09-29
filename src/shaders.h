#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>
#include "errorHandling.h"

void shaders_init();
void shaders_useBase();
void shaders_setObjectPosition(float x, float y);
void shaders_setObjectScale(float scale);
void shaders_setObjectColor(float red, float green, float blue);
void shaders_setSceneLayout(float left, float top, float width, float height);
GLuint shaders_getAttributeIndex_vertexPosition();

#endif // SHADERS_H

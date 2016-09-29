#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>
#include "errorHandling.h"

void shaders_init();
void shaders_useBase();
void shaders_setSceneLayout(float left, float top, float width, float height);
GLuint shaders_getAttributeIndex_position();
GLuint shaders_getAttributeIndex_color();

#endif // SHADERS_H

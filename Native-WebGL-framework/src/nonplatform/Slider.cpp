
#include "Slider.h"
#include "../platform/platform.h"

#include <string>
#include "../platform/draw.h"
#include <iostream>
#include <math.h>

using namespace std;
using namespace platform;

const Color SLIDER_BAR_COLOR_1 = Color(.5, 0, 0);
const Color SLIDER_BAR_COLOR_2 = Color(1, 1, 1);
const Color SLIDER_KNOB_COLOR_1 = Color(.85, .85, 0);
const Color SLIDER_KNOB_COLOR_2 = Color(1, 1, 1);
const Color SLIDER_LABEL_COLOR = Color(1, 1, 1);
const char* SLIDER_DEFAULT_TEXT = "Nameless";

Slider::Slider()
{
	this->value = .5f;
	text = SLIDER_DEFAULT_TEXT;
	isDragging = false;
	mouseOffset = 0;
	x = y = 0;
	width = 100;
	mouseIsDown = false;
}

Slider::~Slider() {}

void Slider::frameLogic()
{
	if(isMouseDown() && !mouseIsDown)
	{
		mouseIsDown = true;
		int mouseX = getMouseX();
		int mouseY = getMouseY();
		int knobX = x + (value-.5f) * width;
		int knobY = y;
		int distance = sqrt(
				(mouseX-knobX)*(mouseX-knobX) +
				(mouseY-knobY)*(mouseY-knobY));
		if(distance <= 10)
		{
			isDragging = true;
			mouseOffset = mouseX - knobX;
		}
	}
	if(!isMouseDown() && mouseIsDown)
	{
		mouseIsDown = false;
		isDragging = false;
	}

	if(isDragging)
	{
		int curOffset = getMouseX() - (x + (value-.5f) * width);
		int adjust = curOffset - mouseOffset;
		value += adjust / width;
	}
	if(value < 0) value = 0;
	if(value > 1) value = 1;

	// Draw
	platform::setNextDraw_color(SLIDER_BAR_COLOR_1);
	platform::drawRectangle(x, y, width, 7, true);
	platform::setNextDraw_color(SLIDER_BAR_COLOR_2);
	platform::drawRectangle(x, y, width, 7, false);

	int knobX = x + (value-.5f) * width;
	platform::setNextDraw_color(SLIDER_KNOB_COLOR_1);
	platform::drawCircle(knobX, y, 10, true);
	platform::setNextDraw_color(SLIDER_KNOB_COLOR_2);
	platform::drawCircle(knobX, y, 10, false);
}

string Slider::getText() const
{
	return text;
}
void Slider::setText(string value)
{
	text = value;
}
float Slider::getValue() const
{
	return value;
}
void Slider::setValue(float value)
{
	this->value = value;
}
float Slider::getX() const
{
	return x;
}
float Slider::getY() const
{
	return y;
}
void Slider::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}
float Slider::getWidth() const
{
	return width;
}
void Slider::setWidth(float width)
{
	this->width = width;
}

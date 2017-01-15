/*
 * ButtonSet.cpp
 *
 *  Created on: Jan 2, 2017
 *      Author: Jon
 */

#include <string.h>
#include "ButtonSet.h"
#include "../platform/platform.h"

#include <iostream>

using namespace std;

Button::Button(float x, float y, float sizeX, float sizeY) :
	x(x), y(y), sizeX(sizeX), sizeY(sizeY),
	onClick(0),
	image_current(""), image_normal(""), image_hover(""), image_down("") {}

Button::~Button() {}

bool Button::isContaining(float x, float y) const
{
	if(
			x > this->x-this->sizeX/2 &&
			y > this->y-this->sizeY/2 &&
			x < this->x+this->sizeX/2 &&
			y < this->y+this->sizeY/2)
	{
		return true;
	}
	return false;
}

void Button::render() const
{
	if(image_current[0] != '\0')
	{
		platform::drawImage(x, y, sizeX, sizeY, image_current);
	}
}

fncButtonEvent Button::getOnClick() const
{
	return onClick;
}

void Button::setOnClick(fncButtonEvent value)
{
	this->onClick = value;
}

const char* Button::getImage() const
{
	return image_current;
}

void Button::setImage(const char* name)
{
	if(name[0] != '\0')
	{
		image_current = name;
	}
}

const char* Button::getImage_normal() const
{
	return image_normal;
}

void Button::setImage_normal(const char* name)
{
	image_current = image_normal = name;
}

const char* Button::getImage_hover() const
{
	return image_hover;
}

void Button::setImage_hover(const char* name)
{
	image_hover = name;
}

const char* Button::getImage_down() const
{
	return image_down;
}

void Button::setImage_down(const char* name)
{
	image_down = name;
}

ButtonSet::ButtonSet() : clickingButton(0) {}

ButtonSet::~ButtonSet()
{
	for(vector<Button*>::iterator i= buttons.begin();
		i != buttons.end(); ++i)
	{
		delete (*i);
	}
}

Button* ButtonSet::addButton(float x, float y, float sizeX, float sizeY)
{
	Button* result = new Button(x, y, sizeX, sizeY);
	buttons.push_back(result);
	return result;
}

void ButtonSet::doEachTime()
{
	float mouseDown = platform::isMouseDown();
	float mouseX = platform::getMouseX();
	float mouseY = platform::getMouseY();
	for(vector<Button*>::iterator i= buttons.begin();
		i != buttons.end(); ++i)
	{
		bool contains = (*i)->isContaining(mouseX, mouseY);
		const char* image_current = (*i)->getImage_normal();

		if(*i == clickingButton)
		{
			if(contains)
			{
				if(mouseDown)
				{
					image_current = (*i)->getImage_down();
				}
				else
				{
					image_current = (*i)->getImage_hover();
					if(!strcmp(image_current,""))
					{
						image_current = (*i)->getImage_normal();
					}
					fncButtonEvent fnc = (*i)->getOnClick();
					if(fnc)
					{
						fnc();
					}
					clickingButton = 0;
				}
			}
			else
			{
				image_current = (*i)->getImage_normal();
				if(!mouseDown)
				{
					clickingButton = 0;
				}
			}
		}
		else
		{
			if(!clickingButton && contains)
			{
				if(mouseDown)
				{
					clickingButton = *i;
					image_current = (*i)->getImage_down();
				}
				else
				{
					image_current = (*i)->getImage_hover();
				}
			}
		}
		(*i)->setImage(image_current);
		(*i)->render();
	}
}

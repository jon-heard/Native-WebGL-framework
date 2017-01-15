/*
 * FadeSystem.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: Jon
 */

#include <iostream>
#include "FadeSystem.h"

using namespace std;

FadeSystem* FadeSystem::instance = 0;

FadeSystem* FadeSystem::getInstance()
{
	if(!FadeSystem::instance)
	{
		FadeSystem::instance = new FadeSystem();
	}
	return FadeSystem::instance;
}

FadeSystem::FadeSystem() {}
FadeSystem::~FadeSystem() {}

FadeSystemItem* FadeSystem::addFade(
		float x, float y, float sizeX, float sizeY, const char* image,
		bool isFadingIn)
{
	FadeSystemItem* result = new FadeSystemItem();
	items.push_back(result);
	result->setImage(image);
	result->setPosition(x, y);
	result->setSize(sizeX, sizeY);
	result->setIsFadingIn(isFadingIn);
	return result;
}

void FadeSystem::doEachTime()
{
	vector<FadeSystemItem*> toRemove;
	unsigned long time = platform::getTime();
	for(vector<FadeSystemItem*>::iterator i = items.begin();
	    i != items.end(); ++i)
	{
		unsigned long fadeTime = (*i)->getFadeTime();
		long timeLeft = (*i)->getStartTime() + fadeTime - time;
		if(timeLeft <= 0)
		{
			FadeDoneCallback callback = (*i)->getDoneCallback();
			if(callback != 0)
			{
				callback(*i);
			}
			items.erase(i);
			i--;
		}
		else
		{
			(*i)->draw((float)timeLeft / fadeTime);
		}
	}
}


FadeSystemItem::FadeSystemItem() :
	image(""), x(0), y(0), sizeX(1), sizeY(1),
	startTime(platform::getTime()), fadeTime(500), doneCallback(0),
	color(platform::Color(-1,-1,-1)), rotation(0), opacity(1), fadingIn(false)
{}

void FadeSystemItem::draw(float fade)
{
	platform::setNextDraw_color(color);
	platform::setNextDraw_rotation(rotation);
	if(fadingIn)
	{
		platform::setNextDraw_opacity(opacity * (1-fade));
	}
	else
	{
		platform::setNextDraw_opacity(opacity * fade);
	}
	platform::drawImage(x, y, sizeX, sizeY, image);
}

const char* FadeSystemItem::getImage() const
{
	return image;
}
void FadeSystemItem::setImage(const char* value)
{
	this->image = value;
}
float FadeSystemItem::getX() const
{
	return x;
}
float FadeSystemItem::getY() const
{
	return y;
}
void FadeSystemItem::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}
float FadeSystemItem::getSizeX() const
{
	return sizeX;
}
float FadeSystemItem::getSizeY() const
{
	return sizeY;
}
void FadeSystemItem::setSize(float x, float y)
{
	this->sizeX = x;
	this->sizeY = y;
}
unsigned long FadeSystemItem::getStartTime() const
{
	return startTime;
}
unsigned long FadeSystemItem::getFadeTime() const
{
	return fadeTime;
}
void FadeSystemItem::setFadeTime(unsigned long value)
{
	this->fadeTime = value;
}
FadeDoneCallback FadeSystemItem::getDoneCallback() const
{
	return doneCallback;
}
void FadeSystemItem::setDoneCallback(FadeDoneCallback value)
{
	this->doneCallback = value;
}
platform::Color FadeSystemItem::getColor() const
{
	return color;
}
void FadeSystemItem::setColor(platform::Color value)
{
	this->color = value;
}
float FadeSystemItem::getRotation() const
{
	return rotation;
}
void FadeSystemItem::setRotation(float value)
{
	this->rotation = value;
}
float FadeSystemItem::getOpacity() const
{
	return opacity;
}
void FadeSystemItem::setOpacity(float value)
{
	this->opacity = value;
}
bool FadeSystemItem::getIsFadingIn()
{
	return fadingIn;
}
void FadeSystemItem::setIsFadingIn(bool value)
{
	this->fadingIn = value;
}

/*
 * FadeSystem.h
 *
 *  Created on: Jan 15, 2017
 *      Author: Jon
 */

#ifndef NONPLATFORM_FADESYSTEM_H_
#define NONPLATFORM_FADESYSTEM_H_

#include <vector>
#include "../platform/platform.h"

class FadeSystemItem;

typedef void (*FadeDoneCallback)(FadeSystemItem*);

class FadeSystem
{
public:
	static FadeSystem* getInstance();
	FadeSystemItem* addFade(
			float x, float y, float sizeX, float sizeY, const char* image,
			bool isFadingIn = false);
	void doEachTime();
private:
	FadeSystem();
	virtual ~FadeSystem();
	static FadeSystem* instance;
	std::vector<FadeSystemItem*> items;
};

class FadeSystemItem
{
public:
	FadeSystemItem();
	void draw(float fade);

	const char* getImage() const;
	void setImage(const char* value);
	float getX() const;
	float getY() const;
	void setPosition(float x, float y);
	float getSizeX() const;
	float getSizeY() const;
	void setSize(float x, float y);
	unsigned long getStartTime() const;
	unsigned long getFadeTime() const;
	void setFadeTime(unsigned long value);
	FadeDoneCallback getDoneCallback() const;
	void setDoneCallback(FadeDoneCallback value);
	platform::Color getColor() const;
	void setColor(platform::Color value);
	float getRotation() const;
	void setRotation(float value);
	float getOpacity() const;
	void setOpacity(float value);
	bool getIsFadingIn();
	void setIsFadingIn(bool value);
private:
	const char* image;
	float x, y, sizeX, sizeY;
	unsigned long startTime, fadeTime;
	FadeDoneCallback doneCallback;
	platform::Color color;
	float rotation, opacity;
	bool fadingIn;
};

#endif /* NONPLATFORM_FADESYSTEM_H_ */

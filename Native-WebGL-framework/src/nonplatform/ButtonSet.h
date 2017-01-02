/*
 * ButtonSet.h
 *
 *  Created on: Jan 2, 2017
 *      Author: Jon
 */

#ifndef NONPLATFORM_BUTTONSET_H_
#define NONPLATFORM_BUTTONSET_H_

#include <vector>

typedef void (*fncButtonEvent)();

class Button
{
public:
	Button(float x, float y, float sizeX, float sizeY);
	virtual ~Button();

	bool isContaining(float x, float y) const;

	void render() const;

	fncButtonEvent getOnClick() const;
	void setOnClick(fncButtonEvent value);

	const char* getImage() const;
	void setImage(const char* name);
	const char* getImage_normal() const;
	void setImage_normal(const char* name);
	const char* getImage_hover() const;
	void setImage_hover(const char* name);
	const char* getImage_down() const;
	void setImage_down(const char* name);
private:
	float x,y;
	float sizeX,sizeY;
	fncButtonEvent onClick = 0;
	const char* image_current = "";
	const char* image_normal = "";
	const char* image_hover = "";
	const char* image_down = "";
};

class ButtonSet
{
public:
	virtual ~ButtonSet();
	Button* addButton(float x, float y, float sizeX, float sizeY);
	void doEachTime();
private:
	std::vector<Button*> buttons;
	bool isDown;
	Button* clickingButton = 0;
};

#endif /* NONPLATFORM_BUTTONSET_H_ */

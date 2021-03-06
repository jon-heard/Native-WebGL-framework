
#ifndef SFMLSLIDER_H
#define SFMLSLIDER_H

#include <iostream>

class Slider
{
public:
	Slider();
	virtual ~Slider();

	// Called once for each slider to draw
	void frameLogic();

	// Text - The text in the label
	std::string getText() const;
	void setText(std::string value);
	// Value - The numeric value of the slider (0.0-1.0)
	float getValue() const;
	void setValue(float value);
	// Position - The x,y coordinates of the center of the slider
	float getX() const;
	float getY() const;
	void setPosition(float x, float y);
	// Width - The visual width of the slider
	float getWidth() const;
	void setWidth(float width);
private:
	float value;

	std::string text;
	bool isDragging;
	bool mouseIsDown;
	int mouseOffset;

	float x, y, width;
};

#endif

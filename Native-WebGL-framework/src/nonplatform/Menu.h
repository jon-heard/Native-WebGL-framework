
#ifndef NONPLATFORM_MENU_H_
#define NONPLATFORM_MENU_H_

#include <vector>
#include "../platform/platform.h"

class MenuItem;

typedef void (*fncMenuEvent)(MenuItem* menu);

class MenuItem
{
public:
	MenuItem(const char* text);
	MenuItem(const char* text, fncMenuEvent event);
	virtual ~MenuItem();

	static MenuItem* getBack();

	const char* getText();
	void setText(const char* text);
	unsigned int getFontSize() const;
	void setFontSize(unsigned int value);
	MenuItem* getParent() const;
	MenuItem* getChild(unsigned int index) const;
	unsigned int getChildCount() const;
	MenuItem* addChild(MenuItem* child);
	fncMenuEvent getEvent() const;
	void setEvent(fncMenuEvent value);
private:
	static MenuItem* back;
	const char* text;
	unsigned int fontSize;
	MenuItem* parent;
	std::vector<MenuItem*> children;
	fncMenuEvent clickEvent;
};

class Menu
{
public:
	Menu();
	virtual ~Menu();
	void doEachTime();

	static void backEvent(MenuItem* menu);

	MenuItem* getRoot() const;
	float getX() const;
	float getY() const;
	void setPosition(float x, float y);
	unsigned int getWidth() const;
	void setWidth(unsigned int value);
	unsigned int getMenuItemSpacing() const;
	void setMenuItemSpacing(unsigned int value);
	unsigned int getFontSize() const;
	void setFontSize(unsigned int value);
	platform::Color getColor_normal();
	platform::Color getColor_highlight();
	void setColors(platform::Color normal, platform::Color highlight);
private:
	static Menu* currentMenu;
	MenuItem *root, *current, *selected;
	float x, y;
	unsigned int width;
	unsigned int menuItemSpacing, fontSize;
	platform::Color color_normal, color_highlight;
};

#endif /* NONPLATFORM_MENU_H_ */

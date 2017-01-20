
#include "Menu.h"
#include <iostream>

using namespace std;

MenuItem* MenuItem::back = new MenuItem("..back", Menu::backEvent);
Menu* Menu::currentMenu = 0;

MenuItem::MenuItem(const char* text) :
		text(text), fontSize(0), parent(0), clickEvent(0) {}

MenuItem::MenuItem(const char* text, fncMenuEvent event) :
		text(text), fontSize(0), parent(0), clickEvent(event) {}

MenuItem::~MenuItem()
{
	for(vector<MenuItem*>::iterator i = children.begin();
	    i != children.end(); i++)
	{
		delete (*i);
	}
}

MenuItem* MenuItem::getBack()
{
	return MenuItem::back;
}

const char* MenuItem::getText()
{
	return text;
}
void MenuItem::setText(const char* text)
{
	this->text = text;
}
unsigned int MenuItem::getFontSize() const
{
	return fontSize;
}
void MenuItem::setFontSize(unsigned int value)
{
	fontSize = value;
}
MenuItem* MenuItem::getParent() const
{
	return parent;
}
MenuItem* MenuItem::getChild(unsigned int index) const
{
	if(parent)
	{
		if(index == 0)
		{
			return MenuItem::back;
		}
		index--;
	}
	if(index >= children.size())
	{
		return 0;
	}
	return children[index];
}
unsigned int MenuItem::getChildCount() const
{
	unsigned int result = children.size();
	if(parent)
	{
		result = result + 1;
	}
	return result;
}
MenuItem* MenuItem::addChild(MenuItem* child)
{
	child->parent = this;
	children.push_back(child);
	return child;
}
fncMenuEvent MenuItem::getEvent() const
{
	return clickEvent;
}
void MenuItem::setEvent(fncMenuEvent value)
{
	clickEvent = value;
}


Menu::Menu() :
		root(new MenuItem("")), selected(0),
		x(0), y(0), width(0),
		menuItemSpacing(0), fontSize(0),
		color_normal(platform::Color(-1,-1,-1)),
		color_highlight(platform::Color(-1,-1,-1))
{
	current = root;
}
Menu::~Menu()
{
	delete root;
}
void Menu::doEachTime()
{
	int childCount = current->getChildCount();
	unsigned int currentFontSize = current->getFontSize();
	if(currentFontSize == 0) currentFontSize = fontSize;
	if(currentFontSize == 0) currentFontSize=platform::getDefaultFontSize();

	// Get hovered menu item
	bool clicked = false;
	MenuItem* hovered = 0;
	float xMouse = platform::getMouseX() - x;
	float yMouse = platform::getMouseY() - y;
	if(xMouse > 0 && yMouse > 0)
	{
		unsigned int menuHeight = currentFontSize + menuItemSpacing;
		yMouse /= menuHeight;
		hovered = current->getChild(yMouse);
		if(hovered)
		{
			if(hovered->getChildCount() < 2 && !hovered->getEvent())
			{
				hovered = 0;
			}
			else
			{
				platform::setNextDraw_fontSize(currentFontSize);
				unsigned int textWidth =
						platform::getTextWidth(hovered->getText());
				if(xMouse > textWidth)
				{
					hovered = 0;
				}
			}
		}
	}
	if(platform::isMouseDown())
	{
		selected = hovered;
	}
	else if(selected)
	{
		clicked = true;
	}


	// Draw menu items
	int drawOffset = 0;
	for(int i = 0; i < childCount; i++)
	{
		MenuItem* child = current->getChild(i);
		unsigned int myFontSize = currentFontSize;
		if(child == MenuItem::getBack())
		{
			myFontSize = platform::getDefaultFontSize();
		}
		platform::Color color = color_normal;
		if(child == hovered)
		{
			color = color_highlight;
		}
		platform::setNextDraw_color(color);
		platform::setNextDraw_fontSize(myFontSize);
		platform::setNextDraw_textWidth(width);
		platform::drawText(x, y+myFontSize+drawOffset, child->getText());
		drawOffset += myFontSize + menuItemSpacing;
		if(child == MenuItem::getBack())
		{
			drawOffset += myFontSize/2;
		}
	}

	// Handle clicking
	if(clicked)
	{
		if(selected->getChildCount() > 1)
		{
			current = selected;
		}
		fncMenuEvent event = selected->getEvent();
		if(event)
		{
			Menu::currentMenu = this;
			event(selected);
			Menu::currentMenu = 0;
		}
		selected = 0;
	}
}

void Menu::backEvent(MenuItem* menu)
{
	Menu::currentMenu->current = Menu::currentMenu->current->getParent();
}

MenuItem* Menu::getRoot() const
{
	return root;
}
float Menu::getX() const
{
	return x;
}
float Menu::getY() const
{
	return y;
}
void Menu::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}
unsigned int Menu::getWidth() const
{
	return width;
}
void Menu::setWidth(unsigned int value)
{
	width = value;
}
unsigned int Menu::getMenuItemSpacing() const
{
	return menuItemSpacing;
}
void Menu::setMenuItemSpacing(unsigned int value)
{
	menuItemSpacing = value;
}
unsigned int Menu::getFontSize() const
{
	return fontSize;
}
void Menu::setFontSize(unsigned int value)
{
	fontSize = value;
}
platform::Color Menu::getColor_normal()
{
	return color_normal;
}
platform::Color Menu::getColor_highlight()
{
	return color_highlight;
}
void Menu::setColors(platform::Color normal, platform::Color highlight)
{
	color_normal = normal;
	color_highlight = highlight;
}

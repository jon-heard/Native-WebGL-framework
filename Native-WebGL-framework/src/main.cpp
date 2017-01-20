
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "platform/platform.h"
#include "nonplatform/DrawCache_Glow.h"
#include "platform/Shader.h"
#include "nonplatform/Slider.h"
#include "nonplatform/ButtonSet.h"
#include "nonplatform/linesAndArrows.h"
#include "nonplatform/FadeSystem.h"
#include "nonplatform/Menu.h"
#include <sstream>

using namespace std;

void init();
void frameLogic();
void cleanup();

platform::DrawCache_Glow* glow = NULL;
Slider* slider1 = NULL;
ButtonSet buttons;
const char* currentSmiley = "media/smiley1.png";
const char* otherSmiley = "media/smiley2.png";
const char* smileyBuf = "media/smiley1.png";

const char* LONG_TEXT = "\"I must leave!\"  Said Duncan.\n\nNo!  \"You mustn't.  I shant allow it\", replied Betty.\n\n\"It is not for you to stopy me, Betty.  If we are to survive, I must face the beast head on.\"\n\n\"Fine.  If you are so determined then at least take this with you.\"";


int main()
{
	platform::run(init, frameLogic, cleanup);
}



void smileyCallback(FadeSystemItem* item)
{
	currentSmiley = smileyBuf;
}
void swapSmileys()
{
	smileyBuf = otherSmiley;
	otherSmiley = currentSmiley;
	currentSmiley = "";
	FadeSystemItem* i = FadeSystem::getInstance()->addFade(-200, 0, 128, 128, otherSmiley);
	FadeSystem::getInstance()->addFade(-200, 0, 128, 128, smileyBuf, true);
	i->setDoneCallback(smileyCallback);
}
void menuEvent(MenuItem* menu)
{
	swapSmileys();
}
void button1(Button* btn) { cout << "Button 1 clicked" << endl; swapSmileys(); }
void button2(Button* btn) { cout << "Button 2 clicked" << endl; swapSmileys(); }
void button3(Button* btn) { cout << "Button 3 clicked" << endl; swapSmileys(); }
void button4(Button* btn) { cout << "Button 4 clicked" << endl; swapSmileys(); }
void button5(Button* btn) { cout << "Button 5 clicked" << endl; swapSmileys(); }
void button6(Button* btn) { cout << "Button 6 clicked" << endl; swapSmileys(); }
void button7(Button* btn) { cout << "Button 7 clicked" << endl; swapSmileys(); }
void button8(Button* btn) { cout << "Button 8 clicked" << endl; swapSmileys(); }
void button9(Button* btn) { cout << "Button 9 clicked" << endl; swapSmileys(); }

Menu myMenu;

void init()
{
	platform::setTitle("Base app");
	platform::setBackgroundColor(.25f, .25f, .25f);
	glow = new platform::DrawCache_Glow();
	slider1 = new Slider();
	slider1->setPosition(-150, -280);
	slider1->setWidth(250);
	for(int y = 0; y < 3; y++)
	{
		for(int x = 0; x < 3; x++)
		{
			Button* button = buttons.addButton(x*50+100, y*50-50, 40, 40);
			if(y != 1 || (x != 1 && x != 2))
			{
				button->setImage_normal("media/button_normal.png");
				if(y != 0 || x == 0)
				{
					button->setImage_hover("media/button_hover.png");
				}
				button->setImage_down("media/button_down.png");
			}
			fncButtonEvent event = NULL;
			if(x==0 && y==0) { event = button1; }
			if(x==1 && y==0) { event = button2; }
			if(x==2 && y==0) { event = button3; }
			if(x==0 && y==1) { event = button4; }
			if(x==1 && y==1) { event = button5; }
			if(x==2 && y==1) { event = button6; }
			if(x==0 && y==2) { event = button7; }
			//if(x==1 && y==2) { event = button8; }
			if(x==2 && y==2) { event = button9; }
			button->setOnClick(event);
		}
	}

	myMenu.setPosition(-25, -225);
	myMenu.setColors(platform::Color(1,1,1), platform::Color(.5,.5,1));
	myMenu.setMenuItemSpacing(0);
	myMenu.setFontSize(20);
	myMenu.setWidth(250);
	myMenu.getRoot()->addChild(new MenuItem("First"));
	MenuItem* node1 = myMenu.getRoot()->addChild(new MenuItem("Second"));
		node1->addChild(new MenuItem("subItem1"));
		MenuItem* node2 = node1->addChild(new MenuItem("subItem2"));
			node2->addChild(new MenuItem(LONG_TEXT));
			node2->setFontSize(15);
		node2 = node1->addChild(new MenuItem("Change smiley"));
		node2->setEvent(menuEvent);
	myMenu.getRoot()->addChild(new MenuItem("Third"));
}

float rotationPhase = 0;
void frameLogic()
{
	platform::setNextDraw_color(3);
	platform::drawCircle(0, 0, 300, false);
	platform::setNextDraw_color(0);
	platform::drawCircle(0, 0, 150, false);
	platform::setNextDraw_color(0);
	platform::drawRectangle(0, 0, 150, 150, true);
	platform::setNextDraw_color(1);
	platform::drawRectangle(100, 0, 30, 50, false);
	platform::setNextDraw_color(1);
	platform::drawRectangle(-100, 0, 30, 50, true);
	platform::setNextDraw_color(1);
	platform::drawRectangle(0, 100, 50, 30, false);
	platform::setNextDraw_color(1);
	platform::setNextDraw_opacity(.5f);
	platform::drawRectangle(0, -100, 50, 30, true);

	platform::setNextDraw_color(1);
	platform::drawCircle(
			cos(rotationPhase) * 100, sin(rotationPhase) * 100, 30, false);
	rotationPhase += .025f;

	int color = 3;
	if(platform::isMouseDown())
	{
		color = 2;
	}
	//cout << platform::getMouseX() << ", " << platform::getMouseY() << endl;
	platform::setNextDraw_color(color);
	platform::drawCircle(
			platform::getMouseX(), platform::getMouseY(), 50, true);

	platform::drawImage(-150, -150, 150, 150, "media/colorTest.png");
	platform::drawImage(150, -150, 150, 150, "media/flag.png");

	platform::Shader::useShader("media/frag_blur.txt");
	platform::Shader::setParameter_float("blurAmount", .005f);
	platform::setNextDraw_useCustomShader(true);
	platform::drawImage(150, 150, 150, 150, "media/flag.png");

	platform::setNextDraw_color(1);
	platform::setNextDraw_rotation(rotationPhase);

	glow->start();
	platform::drawImage(0, 0, 64, 64, "media/ligands_3.png");
	glow->stop();
	glow->flush();

	platform::setNextDraw_color(1);
	//platform::setNextDraw_fontSize(24);
	platform::drawText(-200,100,"abcdefghijklmnopqrstuvwxyz");
	platform::setNextDraw_fontSize(24);
	platform::drawText(-200,150,"abcdefghijklmnopqrstuvwxyz");
	platform::drawText(-200,200,"abcABCDEFGHIJKLMNOPQRSTUVWXYZabc");
	platform::drawText(-200,250,"a.b.g.j.p.q.y.A.N.Q.a");

	slider1->frameLogic();
	stringstream s;
	s << slider1->getValue();
	platform::setNextDraw_color(1,1,1);
	platform::drawText(slider1->getX() + 140, slider1->getY() + 5, s.str().c_str());

	platform::setNextDraw_color(1,1,1);
	drawArrow(0, 0, platform::getMouseX(), platform::getMouseY(), 5, 5);

	buttons.doEachTime();
	FadeSystem::getInstance()->doEachTime();

	platform::drawImage(-200, 0, 128, 128, currentSmiley);

	myMenu.doEachTime();
}

void cleanup()
{
	delete glow;
	delete slider1;
}

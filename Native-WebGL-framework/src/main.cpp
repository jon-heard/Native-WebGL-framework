
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "platform/platform.h"
#include "platform/DrawCache_Glow.h"
#include "platform/Shader.h"
#include "platform/Slider.h"
#include "sstream"

using namespace std;

void init();
void frameLogic();
void cleanup();

platform::DrawCache_Glow* glow = NULL;
Slider* slider1 = NULL;

int main()
{
	platform::run(init, frameLogic, cleanup);
}

void init()
{
	platform::setTitle("Base app");
	platform::setBackgroundColor(.25f, .25f, .25f);
	glow = new platform::DrawCache_Glow();
	slider1 = new Slider();
	slider1->setPosition(-150, -280);
	slider1->setWidth(250);
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
}

void cleanup()
{
	delete glow;
	delete slider1;
}

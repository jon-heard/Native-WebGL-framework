
#include <GLFW/glfw3.h>
#include "platform.h"

namespace platform
{
	int mouseX = 0, mouseY = 0, mouseDown = 0;

	void handleMousePosition(GLFWwindow* window, double xpos, double ypos);
	void handleMouseClick(GLFWwindow* window, int button, int action, int mods);

	void input_init(GLFWwindow* window)
	{
	    glfwSetCursorPosCallback(window, handleMousePosition);
		glfwSetMouseButtonCallback(window, handleMouseClick);
	}

	void input_shutdown() {}

	bool isMouseDown()
	{
		return mouseDown;
	}

	float getMouseX()
	{
		return mouseX;
	}

	float getMouseY()
	{
		return mouseY;
	}

   bool hasClicked = false;
   bool isMouseClicked()
   {
      if(!mouseDown)
      {
         hasClicked = false;
         return false;
      }
      else
      {
         if(hasClicked)
         {
            return false;
         }
         else
         {
            hasClicked = true;
            return true;
         }
      }
   }

	void handleMousePosition(GLFWwindow* window, double xpos, double ypos)
	{
		mouseX = xpos*(RIGHT-LEFT)/WIN_WIDTH+LEFT;
		mouseY = ypos*(BOTTOM-TOP)/WIN_HEIGHT+TOP;
	}

	void handleMouseClick(GLFWwindow* window, int button, int action, int mods)
	{
		mouseDown = (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS);
	}
}

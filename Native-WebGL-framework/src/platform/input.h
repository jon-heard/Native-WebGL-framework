// Just included in platform.cpp for init and shutdown.  Most functions are
// referenced in platform.h

#ifndef INPUT_H_
#define INPUT_H_

namespace platform
{
	void input_init(GLFWwindow* window);
	void input_shutdown();
}

#endif /* INPUT_H_ */

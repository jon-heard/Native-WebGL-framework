
#include "DrawCache.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "Platform.h"

namespace platform
{
	extern float background_red;
	extern float background_green;
	extern float background_blue;

	DrawCache::DrawCache()
	{
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGBA, WIN_WIDTH, WIN_HEIGHT,
				0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenFramebuffers(1, &bufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, bufferId);
		glFramebufferTexture2D(
				GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, textureId, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return;
	}

	DrawCache::~DrawCache() {}

	void DrawCache::start()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, bufferId);
		glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
		glClearColor(background_red, background_green, background_blue, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void DrawCache::stop()
	{
		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}

	void DrawCache::flush()
	{
		drawImage(0, 0, RIGHT-LEFT, BOTTOM-TOP, textureId);
	}
}

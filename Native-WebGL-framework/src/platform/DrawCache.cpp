
#include "DrawCache.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "Platform.h"

namespace platform
{
	DrawCache::DrawCache()
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGBA8, WIN_WIDTH, WIN_HEIGHT,
				0, GL_RGBA, GL_INT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenFramebuffers(1, &buffer);
		glBindFramebuffer(GL_FRAMEBUFFER, buffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return;
	}

	DrawCache::~DrawCache() {}

	void DrawCache::start()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, buffer);
		glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void DrawCache::stop()
	{
		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}

	void DrawCache::flush()
	{
		drawImage(0, 0, RIGHT-LEFT, BOTTOM-TOP, texture);

		//		glBindFramebuffer(GL_FRAMEBUFFER,0);
		//
		//		shaders_useBlur(LEFT, TOP, RIGHT-LEFT, BOTTOM-TOP, texture, .003f);
		//
		//		glBindBuffer(GL_ARRAY_BUFFER, rectangleBuffer);
		//
		//		glVertexAttribPointer(
		//				attribute_position, 2, GL_FLOAT, GL_FALSE, 8, BUFFER_OFFSET(0));
		//		glEnableVertexAttribArray(attribute_position);
		//
		//		glDrawArrays(GL_TRIANGLES, 0, 12);
		//
		//		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

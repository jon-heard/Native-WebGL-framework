
#ifndef PLATFORM_DRAWCACHE_H_
#define PLATFORM_DRAWCACHE_H_

#include "Shader.h"

namespace platform
{
	class DrawCache
	{
	public:
		DrawCache();
		virtual ~DrawCache();

		void start();
		void stop();
		virtual void flush();

		Shader* getShader();
		void setShader(Shader* value);
	public:
		unsigned int bufferId;
		unsigned int textureId;
	};
}

#endif /* PLATFORM_DRAWCACHE_H_ */

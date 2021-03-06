/*
 * DrawCache_Glow.h
 *
 *  Created on: Dec 1, 2016
 *      Author: Jon
 */

#ifndef PLATFORM_DRAWCACHE_GLOW_H_
#define PLATFORM_DRAWCACHE_GLOW_H_

#include "../platform/DrawCache.h"

namespace platform
{
	class DrawCache_Glow : public DrawCache
	{
	public:
		DrawCache_Glow();
		virtual ~DrawCache_Glow();
		virtual void flush();
	private:
		static unsigned int instanceCount;
		static platform::Shader* blurShader;
		static platform::Shader* combineShader;
		static platform::DrawCache* glow;
		static platform::DrawCache* combined;
	};
}

#endif /* PLATFORM_DRAWCACHE_GLOW_H_ */

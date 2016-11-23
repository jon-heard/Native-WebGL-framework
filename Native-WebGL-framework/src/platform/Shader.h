/*
 * Shader.h
 *
 *  Created on: Nov 22, 2016
 *      Author: Jon
 */

#ifndef PLATFORM_SHADER_H_
#define PLATFORM_SHADER_H_

#include <map>
#include <string>

namespace platform
{
	class Shader
	{
	public:
		static bool Shader_Init();

		Shader(const char* fragSourceFile);
		virtual ~Shader();

		static bool useShader(const char* name);

		static int getParameterId(const char* name);
		static int getParameterType(const char* name);

		static bool setParameter_float(const char* name, float value1);
		static bool setParameter_vec2(const char* name,
				float value1, float value2);
		static bool setParameter_vec3(const char* name,
				float value1, float value2, float value3);
		static bool setParameter_vec4(const char* name,
				float value1, float value2, float value3, float value4);
		static bool setParameter_mat4(const char* name, float* value1);
		static bool setParameter_Texture1(const char* name, int textureId1);

		static void setSceneLayout(
				float left, float top, float right, float bottom);
	private:
		unsigned int programId = 0;
		bool ready = false;
		std::map<std::string, std::pair<unsigned int, unsigned int>> parameters;

		static bool inited;
		static std::map<std::string, Shader*> shaderCollection;
		static Shader* currentShader;
		static float left, top, right, bottom;
	};
}

#endif /* PLATFORM_SHADER_H_ */

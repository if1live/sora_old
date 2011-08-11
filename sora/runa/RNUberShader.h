// Ŭnicode please
#pragma once

#include "matsu/MTSingleton.h"
#include "runa/RNTypedef.h"
#if _WIN_
#include <unordered_map>
#elif _IPHONE_
#include <tr1/unordered_map>
#endif

//uber shader에서 상태변수를 몰아서 관리하기 편하게 상수이용함
#define UBER_SHADER_USE_NORMAL	0x01
#define UBER_SHADER_USE_TEXCOORD	0x02
#define UBER_SHADER_USE_COLOR	0x04

//test shader
#define UBER_SHADER_TEST_CEL 0x08

namespace runa
{
	class UberShader;
	class ShaderProgram;

	class UberShader : public matsu::Singleton<UberShader> {
	public:
		typedef std::tr1::unordered_map<int,ShaderProgramPtr> ShaderDictType;

		UberShader();
		~UberShader();

		ShaderProgramPtr getShader(int flag);
		void cleanupUnusedShader();
	private:
		std::string origVertSrc_;
		std::string origFragSrc_;

		bool hasFlag(int src, int test) const;
		ShaderProgramPtr create(int flag) const;
		std::string createMacroCode(int flag) const;

		ShaderDictType shaderDict_;
	};
}
// Ŭnicode please
#pragma once

#include "runa/RNTypedef.h"
#if _WIN_
#include <unordered_map>
#elif _IPHONE_
#include <tr1/unordered_map>
#endif

#define UBER_SHADER_USE_NORMAL	0x01
#define UBER_SHADER_USE_TEXCOORD	0x02
#define UBER_SHADER_USE_COLOR	0x04

namespace runa
{
	class UberShader;
	class ShaderDefineIdentifier;

	class UberShader {
	public:
		typedef unsigned int FlagType;
		typedef std::tr1::unordered_map<FlagType,ShaderProgramPtr> ShaderDictType;
		
	public:
		UberShader(const std::string &vertSrc, const std::string &fragSrc);
		~UberShader();

		ShaderProgramPtr getShader(FlagType flag);
		void cleanupUnusedShader();
	private:
		std::string origVertSrc_;
		std::string origFragSrc_;

		bool hasFlag(FlagType src, FlagType test) const;
		ShaderProgramPtr create(FlagType flag) const;
		std::string createMacroCode(FlagType flag) const;

		ShaderDictType shaderDict_;
	};

	
}
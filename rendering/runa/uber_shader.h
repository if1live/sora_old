// Ŭnicode please
#pragma once

#include "runa/runa_enum.h"

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
		UberShader(const std::string &vert_src, const std::string &frag_src);
		~UberShader();

		ShaderProgramPtr getShader(FlagType flag);
		void CleanupUnusedShader();
	private:
		std::string orig_vert_src_;
		std::string orig_frag_src_;

		bool HasFlag(FlagType src, FlagType test) const;
		ShaderProgramPtr create(FlagType flag) const;
		std::string CreateMacroCode(FlagType flag) const;

		ShaderDictType shader_dict_;
	};

	
}
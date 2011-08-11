﻿// Ŭnicode please
#include "../SoraStdAfx.h"

#include "runa/RNUberShader.h"
#include "runa/RNShaderProgram.h"

using namespace std;
using namespace matsu;

namespace runa
{
	UberShader::UberShader(const std::string &vertSrc, const std::string &fragSrc)
		: origVertSrc_(vertSrc),
		origFragSrc_(fragSrc)
	{
	}
	UberShader::~UberShader()
	{
	}

	ShaderProgramPtr UberShader::getShader(FlagType flag)
	{
		ShaderDictType::iterator it = shaderDict_.find(flag);
		if(it == shaderDict_.end())
		{
			//코드 생성
			shaderDict_[flag] = create(flag);
		}
		return shaderDict_[flag];
	}
	ShaderProgramPtr UberShader::create(FlagType flag) const
	{
		string prefix = createMacroCode(flag);
		string vertSrc = prefix + origVertSrc_;
		string fragSrc = prefix + origFragSrc_;
			
		//MT_LINE_LOG("== vertex source ==");
		//MT_LINE_LOG(vertSrc);
		//MT_LINE_LOG("== fragment source ==");
		//MT_LINE_LOG(fragSrc);
			
		ShaderProgramPtr prog(new ShaderProgram(vertSrc, fragSrc));
		return prog;
	}
	std::string UberShader::createMacroCode(FlagType flag) const
	{
		string macro;
		if(hasFlag(flag, UBER_SHADER_USE_NORMAL))
		{
			macro += "#define USE_NORMAL 1 \n";
		}
		if(hasFlag(flag, UBER_SHADER_USE_COLOR))
		{
			macro += "#define USE_COLOR 1 \n";
		}
		if(hasFlag(flag, UBER_SHADER_USE_TEXCOORD))
		{
			macro += "#define USE_TEXCOORD 1 \n";
		}
		/*
		if(hasFlag(flag, UBER_SHADER_TEST_CEL))
		{
			macro += "#define TEST_CEL 1 \n";
		}
		*/
		return macro;
	}
	bool UberShader::hasFlag(FlagType src, FlagType test) const
	{
		return ((src & test) == test);
	}
	void UberShader::cleanupUnusedShader()
	{
		MT_LOG("Uber Shader::shader count : %d", shaderDict_.size());
			
		vector<int> unusedList;
			
		ShaderDictType::iterator it = shaderDict_.begin();
		ShaderDictType::iterator endit = shaderDict_.end();
		for( ; it != endit ; it++) {
			if(it->second.use_count() <= 1) {
				//위의 tex와 it, 그래서 2면 그외의 아무곳에서도 참조하지 않는것이다
				unusedList.push_back(it->first);
			}
		}
			
		//unused texture의 실제 파기는 for뒤에서 한다(그래샤 loop가 문제없이 통과된다
		BOOST_FOREACH(int shaderId, unusedList) {
			ShaderDictType::iterator it = shaderDict_.find(shaderId);
			shaderDict_.erase(it);
		}
			
		MT_LOG("Uber Shader::after cleanup unused Shader count : %d", shaderDict_.size());
	}
}
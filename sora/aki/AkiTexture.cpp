#include "../SoraStdAfx.h"
#include "aki/AkiTexture.h"

namespace aki
{
	TextureSize::TextureSize()
		: origWidth_(0),
		origHeight_(0),
		texWidth_(0),
		texHeight_(0)
	{
	}
	TextureSize::TextureSize(int origWidth, int origHeight, int texWidth, int texHeight)
		: origWidth_(origWidth),
		origHeight_(origHeight),
		texWidth_(texWidth),
		texHeight_(texHeight)
	{
	}
	TextureSize::~TextureSize()
	{
	}
	int TextureSize::getOrigWidth() const
	{
		return origWidth_;
	}
	int TextureSize::getOrigHeight() const
	{
		return origHeight_;
	}
	int TextureSize::getTexWidth() const
	{
		return texWidth_;
	}
	int TextureSize::getTexHeight() const
	{
		return texHeight_;
	}
}
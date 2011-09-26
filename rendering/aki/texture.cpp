#include "precompile.h"
#include "aki/texture.h"

namespace aki
{
	TextureSize::TextureSize()
		: orig_width_(0),
		orig_height_(0),
		tex_width_(0),
		tex_height_(0)
	{
	}
	TextureSize::TextureSize(int orig_width, int orig_height, int tex_width, int tex_height)
		: orig_width_(orig_width),
		orig_height_(orig_height),
		tex_width_(tex_width),
		tex_height_(tex_height)
	{
	}
	TextureSize::~TextureSize()
	{
	}
}
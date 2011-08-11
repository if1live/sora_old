#pragma once

#include <string>
#include "aki/AkiTypedef.h"

namespace aki
{
	class ImageLoader_SOIL {
	public:
		static ImagePtr load(const std::string &str);
	};
}
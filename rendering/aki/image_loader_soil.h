#pragma once

#include <string>
#include "aki/aki_enum.h"

namespace aki
{
	class ImageLoader_SOIL {
	public:
		static ImagePtr load(const std::string &str);
	};
}
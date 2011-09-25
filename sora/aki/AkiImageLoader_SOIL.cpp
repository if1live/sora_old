#include "../SoraStdAfx.h"

#if _WIN_

#include "aki/AkiImageLoader_SOIL.h"
#include "aki/AkiImage.h"

//이미지 불러오는 기능을 윈도우에서는 SOIL로 하고 아이폰의 내장 라이브러리로 한다 
#include <SOIL.h>

using namespace aki;
using namespace std;

namespace aki
{
	ImagePtr ImageLoader_SOIL::load(const std::string &path)
	{
		SR_ASSERT(path.length() > 0);
		int width, height, channels;
		unsigned char *data = SOIL_load_image(
			path.c_str(), 
			&width,
			&height, 
			&channels,
			SOIL_LOAD_AUTO
			);
		
		InternalFormat format;
		switch(channels) {
		case SOIL_LOAD_L:
			format = InternalFormatLuminance;
			break;
		case SOIL_LOAD_LA:
			format = InternalFormatLuminanceAlpha;
			break;
		case SOIL_LOAD_RGB:
			format = InternalFormatRGB;
			break;
		case SOIL_LOAD_RGBA:
			format = InternalFormatRGBA;
			break;
		}
		PixelType pixelType = PixelType8888;

		ImageDescription desc(width, height, format, pixelType);
		return ImagePtr(new Image(desc, data));
	}
}

#endif
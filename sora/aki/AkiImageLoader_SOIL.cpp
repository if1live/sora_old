#include "../SoraStdAfx.h"

#if _WIN_

#include "aki/AkiImageLoader_SOIL.h"
#include "aki/AkiImage.h"

//�̹��� �ҷ����� ����� �����쿡���� SOIL�� �ϰ� �������� ���� ���̺귯���� �Ѵ� 
#include <SOIL.h>

using namespace aki;
using namespace std;

namespace aki
{
	ImagePtr ImageLoader_SOIL::load(const std::string &path)
	{
		MT_ASSERT(path.length() > 0);
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
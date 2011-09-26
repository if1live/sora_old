#include"precompile.h"
#if SR_WIN

#include "aki/image_loader_soil.h"
#include "aki/image.h"

//�̹��� �ҷ����� ����� �����쿡���� SOIL�� �ϰ� �������� ���� ���̺귯���� �Ѵ� 
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
			format = kInternalFormatLuminance;
			break;
		case SOIL_LOAD_LA:
			format = kInternalFormatLuminanceAlpha;
			break;
		case SOIL_LOAD_RGB:
			format = kInternalFormatRGB;
			break;
		case SOIL_LOAD_RGBA:
			format = kInternalFormatRGBA;
			break;
		}
		PixelType pixel_type = kPixelType8888;

		ImageDescription desc(width, height, format, pixel_type);
		return ImagePtr(new Image(desc, data));
	}
}

#endif
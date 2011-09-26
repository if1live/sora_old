#pragma once

namespace aki
{
	class Image;
	typedef std::tr1::shared_ptr<Image> ImagePtr;

	//opengl을 위한 이미지라이브러리니까 opengl es에서 받아들일수있는
	//포맷으로만 내부에서 처리해도 충분하다
	//opengl과 동일한 enum을 씀으로써 직통변환을 가능하게함
	typedef enum {
		kInternalFormatRGBA = GL_RGBA,
		kInternalFormatRGB = GL_RGB,
		kInternalFormatLuminanceAlpha = GL_LUMINANCE_ALPHA,
		kInternalFormatLuminance = GL_LUMINANCE,
		kInternalFormatAlpha = GL_ALPHA,
	} InternalFormat;

	//opengl과 같은 포맷을 써서 직동변환을 허용
	typedef enum {
		kPixelType8888 = GL_UNSIGNED_BYTE,
		kPixelType4444 = GL_UNSIGNED_SHORT_4_4_4_4,
		kPixelType5551 = GL_UNSIGNED_SHORT_5_5_5_1,
		kPixelType565 = GL_UNSIGNED_SHORT_5_6_5
	} PixelType;

	//texture
	class TextureSize;
	template<unsigned int N>	class TextureGroup;
	typedef TextureGroup<1> Texture;
	typedef std::tr1::shared_ptr<Texture> TexturePtr;
}
#pragma once

namespace aki
{
	class Image;
	typedef std::tr1::shared_ptr<Image> ImagePtr;

	//opengl�� ���� �̹������̺귯���ϱ� opengl es���� �޾Ƶ��ϼ��ִ�
	//�������θ� ���ο��� ó���ص� ����ϴ�
	//opengl�� ������ enum�� �����ν� ���뺯ȯ�� �����ϰ���
	typedef enum {
		InternalFormatRGBA = GL_RGBA,
		InternalFormatRGB = GL_RGB,
		InternalFormatLuminanceAlpha = GL_LUMINANCE_ALPHA,
		InternalFormatLuminance = GL_LUMINANCE,
		InternalFormatAlpha = GL_ALPHA,
	} InternalFormat;

	//opengl�� ���� ������ �Ἥ ������ȯ�� ���
	typedef enum {
		PixelType8888 = GL_UNSIGNED_BYTE,
		PixelType4444 = GL_UNSIGNED_SHORT_4_4_4_4,
		PixelType5551 = GL_UNSIGNED_SHORT_5_5_5_1,
		PixelType565 = GL_UNSIGNED_SHORT_5_6_5
	} PixelType;

	//texture
	class TextureSize;
	template<unsigned int N>	class TextureGroup;
	typedef TextureGroup<1> Texture;
	typedef std::tr1::shared_ptr<Texture> TexturePtr;
}
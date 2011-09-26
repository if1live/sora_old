#pragma once

namespace aki
{
	class Image;
	typedef std::tr1::shared_ptr<Image> ImagePtr;

	//opengl�� ���� �̹������̺귯���ϱ� opengl es���� �޾Ƶ��ϼ��ִ�
	//�������θ� ���ο��� ó���ص� ����ϴ�
	//opengl�� ������ enum�� �����ν� ���뺯ȯ�� �����ϰ���
	typedef enum {
		kInternalFormatRGBA = GL_RGBA,
		kInternalFormatRGB = GL_RGB,
		kInternalFormatLuminanceAlpha = GL_LUMINANCE_ALPHA,
		kInternalFormatLuminance = GL_LUMINANCE,
		kInternalFormatAlpha = GL_ALPHA,
	} InternalFormat;

	//opengl�� ���� ������ �Ἥ ������ȯ�� ���
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
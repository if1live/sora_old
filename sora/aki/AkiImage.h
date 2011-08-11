#pragma once

#include "aki/AkiTypedef.h"

namespace aki
{
	class ImageDescription {
	public:
		ImageDescription();
		ImageDescription(int width, int height, InternalFormat internalFormat, PixelType pixelType);
		~ImageDescription();
		int getWidth() const;
		int getHeight() const;
		InternalFormat getInternalFormat() const;
		PixelType getPixelType() const;

		bool operator==(const ImageDescription &o) const;
		bool operator!=(const ImageDescription &o) const;
		
		int getRowLineSize() const;
		int getPixelSize() const;
	private:
		int width_;
		int height_;
		InternalFormat internalFormat_;
		PixelType pixelType_;
	};
	class Image {
	public:
		static ImagePtr create(const std::string &path);
	public:
		//RGBA�� ����� ������ �̹��� �����
		//�̹����� �ؽ��� �����ϱ����� ũ�Ⱑ �ƴҶ� ����Ѵ�
		Image(int width, int height);
		Image(const ImageDescription &desc, void *data);
		~Image();
		const ImageDescription &getDesc() const;
		const void *getData() const;

		//�ٸ� �̹����� ������ �Լ�. �̰��� �־��
		//�ؽ���ũ�⸦ 2�� �¼��� �����ϴ°��� �����ϰ� �Ҽ��ִ�
		void overwrite(const Image &o);

		int getPixelIndex(int x, int y) const;
		
		bool operator==(const Image &o) const;
		bool operator!=(const Image &o) const;
	private:
		ImageDescription desc_;
		void *data_;
	};
}
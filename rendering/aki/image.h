#pragma once

#include "aki/aki_enum.h"

namespace aki
{
	class ImageDescription {
	public:
		ImageDescription();
		ImageDescription(int width, int height, InternalFormat internal_format, PixelType pixel_type);
		~ImageDescription();
		int get_width() const;
		int get_height() const;
		InternalFormat get_internal_format() const;
		PixelType get_pixel_type() const;

		bool operator==(const ImageDescription &o) const;
		bool operator!=(const ImageDescription &o) const;
		
		int GetRowLineSize() const;
		int GetPixelSize() const;
	private:
		int width_;
		int height_;
		InternalFormat internal_format_;
		PixelType pixel_type_;
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
		const ImageDescription &get_desc() const;
		const void *get_data() const;

		//�ٸ� �̹����� ������ �Լ�. �̰��� �־��
		//�ؽ���ũ�⸦ 2�� �¼��� �����ϴ°��� �����ϰ� �Ҽ��ִ�
		void Overwrite(const Image &o);

		int GetPixelIndex(int x, int y) const;
		
		bool operator==(const Image &o) const;
		bool operator!=(const Image &o) const;
	private:
		ImageDescription desc_;
		void *data_;
	};
}
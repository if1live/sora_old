#pragma once

#include "aki/aki_enum.h"

namespace aki
{
	class ImageDescription {
	public:
		ImageDescription();
		ImageDescription(int width, int height, InternalFormat internal_format, PixelType pixel_type);
		~ImageDescription();
		int width() const;
		int height() const;
		InternalFormat internal_format() const;
		PixelType pixel_type() const;

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
		//RGBA를 사용한 검정색 이미지 만들기
		//이미지가 텍스쳐 생성하기위한 크기가 아닐때 사용한다
		Image(int width, int height);
		Image(const ImageDescription &desc, void *data);
		~Image();
		const ImageDescription &desc() const;
		const void *data() const;

		//다른 이미지를 덮어씌우는 함수. 이것이 있어야
		//텍스쳐크기를 2의 승수로 보정하는것을 깨끗하게 할수있다
		void Overwrite(const Image &o);

		int GetPixelIndex(int x, int y) const;
		
		bool operator==(const Image &o) const;
		bool operator!=(const Image &o) const;
	private:
		ImageDescription desc_;
		void *data_;
	};
}
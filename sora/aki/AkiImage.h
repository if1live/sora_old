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
		//RGBA를 사용한 검정색 이미지 만들기
		//이미지가 텍스쳐 생성하기위한 크기가 아닐때 사용한다
		Image(int width, int height);
		Image(const ImageDescription &desc, void *data);
		~Image();
		const ImageDescription &getDesc() const;
		const void *getData() const;

		//다른 이미지를 덮어씌우는 함수. 이것이 있어야
		//텍스쳐크기를 2의 승수로 보정하는것을 깨끗하게 할수있다
		void overwrite(const Image &o);

		int getPixelIndex(int x, int y) const;
		
		bool operator==(const Image &o) const;
		bool operator!=(const Image &o) const;
	private:
		ImageDescription desc_;
		void *data_;
	};
}
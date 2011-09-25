#include "../SoraStdAfx.h"
#include "aki/AkiImage.h"

#if _WIN_
#include "aki/AkiImageLoader_SOIL.h"
#elif
#error "not support"
#endif


namespace aki
{
	ImageDescription::ImageDescription()
		: width_(0), 
		height_(0),
		internalFormat_(InternalFormatRGBA),
		pixelType_(PixelType8888)
	{
	}
	ImageDescription::ImageDescription(int width, int height, InternalFormat internalFormat, PixelType pixelType)
		: width_(width),
		height_(height),
		internalFormat_(internalFormat),
		pixelType_(pixelType)
	{
	}
	ImageDescription::~ImageDescription()
	{
	}
	int ImageDescription::getWidth() const
	{
		return width_;
	}
	int ImageDescription::getHeight() const
	{
		return height_;
	}

	InternalFormat ImageDescription::getInternalFormat() const
	{
		return internalFormat_;
	}
	PixelType ImageDescription::getPixelType() const
	{
		return pixelType_;
	}

	int ImageDescription::getPixelSize() const
	{
		int pixelSize = 0;
		switch(getPixelType()) {
		case PixelType8888:
			pixelSize = 4;	//4byte
			break;
		case PixelType5551:
			pixelSize = 2;
			break;
		case PixelType4444:
			pixelSize = 2;
			break;
		case PixelType565:
			pixelSize = 2;
			break;
		default:
			SR_ASSERT(!"not valid");
		}
		return pixelSize;
	}
	int ImageDescription::getRowLineSize() const
	{
		int pixelSize = getPixelSize();
		int linesize = pixelSize * width_;
		return linesize;
	}
	bool ImageDescription::operator==(const ImageDescription &o) const
	{
		return (width_ == o.width_
			&& height_ == o.height_
			&& internalFormat_ == o.internalFormat_
			&& pixelType_ == o.pixelType_);
	}
	bool ImageDescription::operator!=(const ImageDescription &o) const
	{
		return !(*this == o);
	}
	/////////////////////////////////
	ImagePtr Image::create(const std::string &path)
	{
#if _WIN_
		//use soil
		return ImageLoader_SOIL::load(path);
#elif _IPHONE_
#error "not support"
		//use core graphic
#endif
	}
	Image::Image(const ImageDescription &desc, void *data)
		: desc_(desc), data_(data)
	{
	}
	Image::Image(int width, int height)
		: data_(NULL)
	{
		desc_ = ImageDescription(width, height, InternalFormatRGBA, PixelType8888);
		data_ = malloc(sizeof(unsigned char) * 4 * width * height);
		memset(data_, 0, sizeof(unsigned char) * 4 * width * height);
	}
	Image::~Image()
	{
		free(data_);
		data_ = NULL;
	}
	const ImageDescription &Image::getDesc() const
	{
		return desc_;
	}
	const void *Image::getData() const
	{
		return data_;
	}
	void Image::overwrite(const Image &o)
	{
		//포맷이 동일할때만 덮어쓰기가 가능하게하자
		//만약 포맷이 다를ㄹ떄 덮어쓰기를 지원할 경우는 아직까지 코드상에서 등장할일이
		//없을거같으니까 나중에 추가하자
		SR_ASSERT(desc_.getInternalFormat() == o.getDesc().getInternalFormat()
			&& desc_.getPixelType() == o.getDesc().getPixelType());
		//텍스쳐 크기 보정용인데 더 작은 이미지에 덮어쓰는 일은 없을것이다
		SR_ASSERT(desc_.getWidth() >= o.getDesc().getWidth()
			&& desc_.getHeight() >= o.getDesc().getHeight());

		//줄단위로 복사하기
		for(int y = 0 ; y < o.getDesc().getHeight() ; y++)
		{
			int prevIndex = o.getPixelIndex(0, y);
			int currIndex = getPixelIndex(0, y);
			unsigned char *prevData = (unsigned char*)o.getData();
			unsigned char *currData = (unsigned char*)getData();

			unsigned char *src = prevData + prevIndex;
			unsigned char *dst = currData + currIndex;

			int rowline = o.getDesc().getRowLineSize();
			memcpy(dst, src, rowline);
		}
	}
	int Image::getPixelIndex(int x, int y) const
	{
		int width = desc_.getWidth();
		int height = desc_.getHeight();
		SR_ASSERT(x >= 0 && x < width && y >= 0 && y < height);
		int pixelSize = desc_.getPixelSize();
		int linesize = desc_.getRowLineSize();
		return (linesize * y) + (pixelSize * x);
	}

	bool Image::operator==(const Image &o) const
	{
		if(desc_ != o.desc_)
		{
			return false;
		}
		int size = desc_.getHeight() * desc_.getRowLineSize();
		if(memcmp(data_, o.data_, size) == 0)
		{
			return true;
		}
		return false;
	}
	bool Image::operator!=(const Image &o) const
	{
		return !(*this == o);
	}
}
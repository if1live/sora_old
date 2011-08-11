#include "aki/AkiImage.h"
#include "matsu/MTMath.h"

namespace aki
{
	template<unsigned int N>
	int TextureGroup<N>::nextId_ = 0;

	template<unsigned int N>
	TextureGroup<N>::TextureGroup()
		: id_(nextId_++)
	{
		BOOST_STATIC_ASSERT(N >= 1);
		GLuint *ptr = reinterpret_cast<GLuint*>(&handle_);
		glGenTextures(N, ptr);	
	}
	template<unsigned int N>
	TextureGroup<N>::~TextureGroup()
	{
		GLuint *ptr = reinterpret_cast<GLuint*>(&handle_);
		glDeleteTextures(N, ptr);
	}

	template<unsigned int N>
	int TextureGroup<N>::getId() const
	{
		return id_;
	}
	template<unsigned int N>
	GLuint TextureGroup<N>::getHandle(int index) const
	{
		MT_ASSERT(index >= 0 && index < N);
		return handle_[index];
	}

	template<unsigned int N>
	GLuint TextureGroup<N>::getHandle() const
	{
		return handle_[0];
	}
	template<unsigned int N>
	template<int index>
	GLuint TextureGroup<N>::getHandle() const
	{
		BOOST_STATIC_ASSERT(index >= 0 && index < N);
		return getHandle(index);
	}
	template<unsigned int N>
	bool TextureGroup<N>::loadImage(Image *img)
	{
		return loadImage(0, img);
	}

	template<unsigned int N>
	bool TextureGroup<N>::loadImage(int index, Image *img)
	{
		MT_ASSERT(index >= 0 && index < N);
		const ImageDescription &desc = img->getDesc();
		int width = desc.getWidth();
		int height = desc.getHeight();
		int texWidth = matsu::ceilPower(2, width);
		int texHeight = matsu::ceilPower(2, height);

		TextureSize size(width, height, texWidth, texHeight);
		texSize_[index] = size;
		
		//bind+set tex param
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, handle_[index]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//load image
		//텍스쳐 크기 보정때문에 여기부터는 약간 달라진다
		if(width != texWidth || height != texHeight)
		{
			//2의 승수로 보정한 이미지 생성
			Image newImg(texWidth, texHeight);
			newImg.overwrite(*img);

			const void *data = newImg.getData();
			MT_ASSERT(data != NULL);

			int internalFormat = newImg.getDesc().getInternalFormat();
			int pixelType = newImg.getDesc().getPixelType();
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texWidth, texHeight,
					 0, internalFormat, pixelType, data);
		}
		else
		{
			const void *data = img->getData();
			MT_ASSERT(data != NULL);

			int internalFormat = img->getDesc().getInternalFormat();
			int pixelType = img->getDesc().getPixelType();
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texWidth, texHeight,
					 0, internalFormat, pixelType, data);
		}

		return true;
	}

	template<unsigned int N>
	template<int index>
	bool TextureGroup<N>::loadImage(Image *img)
	{
		BOOST_STATIC_ASSERT(index >= 0 && index < N);
		return loadImage(index, img);
	}

	template<unsigned int N>
	const TextureSize &TextureGroup<N>::getSize() const
	{
		return texSize_[0];
	}

	template<unsigned int N>
	const TextureSize &TextureGroup<N>::getSize(int index) const
	{
		MT_ASSERT(index >= 0 && index < N);
		return texSize_[index];
	}

	template<unsigned int N>
	template<int index>
	const TextureSize &TextureGroup<N>::getSize() const
	{
		BOOST_STATIC_ASSERT(index >= 0 && index < N);
		return getSize(index);
	}

	template<unsigned int N>
	bool TextureGroup<N>::isLoaded() const
	{
		return isLoaded<0>();
	}

	template<unsigned int N>
	bool TextureGroup<N>::isLoaded(int index) const
	{
		MT_ASSERT(index >= 0 && index < N);
		const TextureSize &size = getSize();
		if(size.getTexWidth() == 0 && size.getTexHeight() == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	template<unsigned int N>
	template<int index>
	bool TextureGroup<N>::isLoaded() const
	{
		BOOST_STATIC_ASSERT(index >= 0 && index < N);
		return isLoaded(index);
	}
}
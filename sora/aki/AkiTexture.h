#pragma once

#include "aki/AkiTypedef.h"

namespace aki
{
	class TextureSize {
	public:
		TextureSize();
		TextureSize(int origWidth, int origHeight, int texWidth, int texHeight);
		~TextureSize();
		
		int getOrigWidth() const;
		int getOrigHeight() const;
		int getTexWidth() const;
		int getTexHeight() const;

	private:
		int origWidth_;
		int origHeight_;
		int texWidth_;
		int texHeight_;
	};
	//opengl texture
	template<unsigned int N>
	class TextureGroup {
	public:
		TextureGroup();
		~TextureGroup();

		int getId() const;

		GLuint getHandle() const;
		GLuint getHandle(int index) const;
		template<int index>
		GLuint getHandle() const;

		bool loadImage(Image *img);
		bool loadImage(int index, Image *img);
		template<int index>
		bool loadImage(Image *img);

		const TextureSize &getSize() const;
		const TextureSize &getSize(int index) const;
		template<int index>
		const TextureSize &getSize() const;

		bool isLoaded() const;
		bool isLoaded(int index) const;
		template<int index>
		bool isLoaded() const;
	private:
		int id_;
		GLuint handle_[N];
		TextureSize texSize_[N];

		static int nextId_;
	};
}

//template impl
#include "aki/AkiTexture.hpp"
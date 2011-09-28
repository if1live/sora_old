#pragma once

#include "aki/aki_enum.h"

namespace aki
{
	class TextureSize {
	public:
		TextureSize();
		TextureSize(int orig_width, int orig_height, int tex_width, int tex_height);
		~TextureSize();

    SR_GETTER(int, orig_width);
    SR_GETTER(int, orig_height);
    SR_GETTER(int, tex_width);
    SR_GETTER(int, tex_height);
	};

	//opengl texture
	template<unsigned int N>
	class TextureGroup {
	public:
		TextureGroup();
		~TextureGroup();

		int id() const;

		GLuint handle() const;
		GLuint handle(int index) const;
		template<int index>
		GLuint handle() const;

		bool LoadImage(Image *img);
		bool LoadImage(int index, Image *img);
		template<int index>
		bool LoadImage(Image *img);

		const TextureSize &size() const;
		const TextureSize &size(int index) const;
		template<int index>
		const TextureSize &size() const;

		bool IsLoaded() const;
		bool IsLoaded(int index) const;
		template<int index>
		bool IsLoaded() const;

	private:
		int id_;
		GLuint handle_[N];
		TextureSize tex_size_[N];

		static int next_id_;
	};
}

//template impl
#include "aki/texture.hpp"
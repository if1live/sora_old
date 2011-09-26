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

		int get_id() const;

		GLuint get_handle() const;
		GLuint get_handle(int index) const;
		template<int index>
		GLuint get_handle() const;

		bool LoadImage(Image *img);
		bool LoadImage(int index, Image *img);
		template<int index>
		bool LoadImage(Image *img);

		const TextureSize &get_size() const;
		const TextureSize &get_size(int index) const;
		template<int index>
		const TextureSize &get_size() const;

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
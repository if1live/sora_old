/*  Copyright (C) 2011 by if1live */
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// Ŭnicode please
#include "precompile.h"
#include "aki/texture_loader.h"
#include "aki/texture2d.h"
#include "sora/platform.h"

#if SR_WIN
#include <SOIL.h>
#endif

namespace aki {;
#if SR_WIN
bool TextureLoader::LoadTexture2D(const std::string &filename,
  GLuint *tex_id, TextureSize *tex_size) {
  SR_ASSERT(tex_id != NULL);
  SR_ASSERT(tex_size != NULL);
  SR_ASSERT(filename.empty() == false);

  SetTexture2DLoadEnv();

  // soil로 바로 텍스쳐 로딩하는 기능을 만들어놓자. 해당 기능이 있으면 좋은 이유는
  // 텍스쳐로딩에 대한 책임을 라이브러리로 다 넘길수있기 때문이다.
  // 이런 api없이 image를 부른다음에 텍스쳐를 로딩하면 플랫폼에 따라 최적화된 로딩을 할수없다
  // load an image file directly as a new OpenGL texture */
  GLuint tex_2d = SOIL_load_OGL_texture (
    filename.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
	);
  //SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	
  // check for an error during the load process
  if( 0 == tex_2d ) {
	  printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
  }
  *tex_id = tex_2d;

  // get texture size
  int width, height, channels;
  unsigned char *data = SOIL_load_image(
    filename.c_str(),
    &width,
    &height,
    &channels,
    SOIL_LOAD_AUTO);

  tex_size->orig_width = width;
  tex_size->orig_height = height;
  tex_size->tex_width = width;
  tex_size->tex_height = height;

  return true;
}
#else
#error "not support"
#endif
bool TextureLoader::SetTexture2DLoadEnv() {
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  return true;
}
bool TextureLoader::LoadTexture2D(const Image &img,
  GLuint *tex_id, TextureSize *tex_size) {
  SR_ASSERT(tex_id != NULL);
  SR_ASSERT(tex_size != NULL);

  const ImageDescription &desc = img.desc();
  int width = desc.width;
  int height = desc.height;
  int tex_width = matsu::MathHelper::CeilPower(2, width);
  int tex_height = matsu::MathHelper::CeilPower(2, height);

  TextureSize size(width, height, tex_width, tex_height);
  *tex_size = size;

  glGenTextures(1, tex_id);

  // bind+set tex param
  // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, *tex_id);
  SetTexture2DLoadEnv();
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // load image
  // 텍스쳐 크기 보정때문에 여기부터는 약간 달라진다
  if (width != tex_width || height != tex_height) {
    // 2의 승수로 보정한 이미지 생성
    Image newImg(tex_width, tex_height);
    newImg.Overwrite(img);

    const void *data = newImg.data();
    SR_ASSERT(data != NULL);

    int internal_format = newImg.desc().internal_format;
    int pixel_type = newImg.desc().pixel_type;
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, tex_width, tex_height,
      0, internal_format, pixel_type, data);
  } else {
    const void *data = img.data();
    SR_ASSERT(data != NULL);

    int internal_format = img.desc().internal_format;
    int pixel_type = img.desc().pixel_type;
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, tex_width, tex_height,
      0, internal_format, pixel_type, data);
  }

  return true;
}
}

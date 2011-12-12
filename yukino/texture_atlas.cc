#include "yukino_stdafx.h"
#include "sora/texture.h"
#include "texture_atlas.h"

namespace yukino {;
float TextureAtlasSegment::GetAtlasX() const {
  float tex_width = parent->tex->tex_header.tex_width;
  return x / tex_width;
}
float TextureAtlasSegment::GetAtlasY() const {
  float tex_height = parent->tex->tex_header.tex_height;
  return y / tex_height;
}
float TextureAtlasSegment::GetAtlasW() const {
  float tex_width = parent->tex->tex_header.tex_width;
  return w / tex_width;
}
float TextureAtlasSegment::GetAtlasH() const {
  float tex_height = parent->tex->tex_header.tex_height;
  return h / tex_height;
}

TextureAtlas::TextureAtlas(sora::Texture *tex)
  : tex(tex) {
}
TextureAtlas::TextureAtlas()
 : tex(NULL) {
}
TextureAtlas::~TextureAtlas() {
}

const TextureAtlasSegment *TextureAtlas::GetSegment(const std::string &key) const {
  SegmentDictType::const_iterator it = seg_dict_.begin();
  SegmentDictType::const_iterator endit = seg_dict_.end();
  for ( ; it != endit ; it++) {
    if (it->first == key) {
      const TextureAtlasSegment &seg = it->second;
      return &seg;
    }
  }
  return NULL;
}
TextureAtlasSegment *TextureAtlas::AddSegment(const std::string &key, float x, float y, float w, float h) {
  TextureAtlasSegment seg(this, x, y, w, h);
  seg_dict_[key] = seg;
  return &(seg_dict_[key]);
}
}
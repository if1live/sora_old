#pragma once

namespace sora {;
class Texture;
}

namespace yukino {;

class TextureAtlasSegment;
class TextureAtlas;

class TextureAtlasSegment {
public:
  TextureAtlasSegment()
    : parent(NULL), x(0), y(0), w(0), h(0) { }
  TextureAtlasSegment(TextureAtlas *parent, float x, float y, float w, float h)
    : parent(parent), x(x), y(y), w(w), h(h) {
  }
  ~TextureAtlasSegment() {}
  TextureAtlas *parent;
  float x;
  float y;
  float w;
  float h;

  float GetAtlasX() const;
  float GetAtlasY() const;
  float GetAtlasW() const;
  float GetAtlasH() const;
};

class TextureAtlas {
public:
  friend class TextureAtlasSegment;
  typedef std::tr1::unordered_map<std::string, TextureAtlasSegment> SegmentDictType;
public:
  TextureAtlas();
  TextureAtlas(sora::Texture *tex);
  ~TextureAtlas();

  const TextureAtlasSegment *GetSegment(const std::string &key) const;
  TextureAtlasSegment *AddSegment(const std::string &key, float x, float y, float w, float h);
  
  sora::Texture *tex;

private:
  SegmentDictType seg_dict_;
};

}
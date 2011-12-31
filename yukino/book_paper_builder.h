// Ŭnicode please
#pragma once

#include "book_scene.h"

namespace sora {;
struct TextureSubImage;
}

namespace yukino {;
class BookPaperBuilder {
public:
  BookPaperBuilder(float cubeWidth, float cubeHeight, float cubeDepth);
  ~BookPaperBuilder();

  BookPaperPtr build();

  //set attribute
  BookPaperBuilder& set_sprite(sora::TextureSubImage *sprite);
  BookPaperBuilder& set_type(BookPaperType type);
  BookPaperBuilder& set_pos(const sora::vec3 &pos);
  BookPaperBuilder& set_fold(AxisDirection axis, float degree);
  BookPaperBuilder& set_rotate(float pitch, float roll, float yaw);
  //BookPaperBuilder& setModel(sora::model::SoraModelPtr model);

  void CreateFloorModel();
  void CreateLeftModel();
  void CreateRightModel();
  void CreateCeilModel();
  void CreateForwardModel();
  void CreateNormalModel(float width, float height);

  float cube_width() const { return cube_width_; }
  float cube_height() const { return cube_height_; }
  float cube_depth() const { return cube_depth_; }

public:
  BookVertex v1;
  BookVertex v2;
  BookVertex v3;
  BookVertex v4;

private:
  void clear();

  const float cube_width_;
  const float cube_height_;
  const float cube_depth_;

  sora::TextureSubImage *sprite_;
  BookPaperType type_;

  float pitch_;
  float roll_;
  float yaw_;

  float x_;
  float y_;
  float z_;

  float width_;
  float height_;

  AxisDirection axis_;
  float angle_;
};
}
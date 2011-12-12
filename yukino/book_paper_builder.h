// Ŭnicode please
#pragma once

#include "book_scene.h"

namespace yukino {;
class TextureAtlasSegment;

class BookPaperBuilder {
public:
  BookPaperBuilder(float cubeWidth, float cubeHeight, float cubeDepth);
  ~BookPaperBuilder();

  BookPaperPtr build();

  //set attribute
  BookPaperBuilder& setSprite(TextureAtlasSegment *sprite);
  BookPaperBuilder& setType(BookPaperType type);
  BookPaperBuilder& setPos(const sora::vec3 &pos);
  BookPaperBuilder& setFold(AxisDirection axis, float degree);
  BookPaperBuilder& setRotate(float pitch, float roll, float yaw);
  //BookPaperBuilder& setModel(sora::model::SoraModelPtr model);

  void createFloorModel();
  void createLeftModel();
  void createRightModel();
  void createCeilModel();
  void createForwardModel();
  void createNormalModel(float width, float height);

  float getCubeWidth() const;
  float getCubeHeight() const;
  float getCubeDepth() const;

public:
  BookVertex v1;
  BookVertex v2;
  BookVertex v3;
  BookVertex v4;

private:
  void clear();

  const float cubeWidth_;
  const float cubeHeight_;
  const float cubeDepth_;

  TextureAtlasSegment *sprite_;
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
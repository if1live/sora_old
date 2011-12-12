﻿// Ŭnicode please
#include "yukino_stdafx.h"
#include "book.h"
#include "book_paper.h"
#include "texture_atlas.h"
#include "renderer.h"
#include "sora/texture.h"
#include "book_paper_builder.h"

using namespace std;
using namespace sora;

namespace yukino {;
BookPaper::BookPaper(TextureAtlasSegment *sprite, BookPaperType type)
  : sprite_(sprite), type_(type), posX_(0), posY_(0), posZ_(0),
  pitch_(0), roll_(0), yaw_(0)
{
}
BookPaper::~BookPaper()
{
}
void BookPaper::setPos(float x, float y, float z)
{
  posX_ = x;
  posY_ = y;
  posZ_ = z;
}
void BookPaper::setRotate(float pitch, float roll, float yaw)
{
  pitch_ = pitch;
  roll_ = roll;
  yaw_ = yaw;
}
float BookPaper::getZ() const
{
  return posZ_;
}
BookPaperType BookPaper::getType() const
{
  return type_;
}
void BookPaper::draw()
{
  //텍스쳐 활성화
  sora::Texture *tex = sprite_->parent->tex;
  srglBindTexture(GL_TEXTURE_2D, tex->handle);


  if(type_ == BookPaperTypeNormal) {
    //get model height
    float min_y = FLT_MAX;
    float max_y = FLT_MIN;
    vector<BookVertex> point_list;
    point_list.push_back(v1);
    point_list.push_back(v2);
    point_list.push_back(v3);
    point_list.push_back(v4);
    for (int i = 0 ; i < point_list.size() ; i++) {
      const BookVertex &v = point_list[i];
      if (min_y > v.y) {
        min_y = v.y;
      }
      if (max_y < v.y) {
        max_y = v.y;
      }
    }
    float height = max_y - min_y;
    
    float bookHeight = Book::GetInstance().getHeight();

    srglPushMatrix();
    srglTranslatef(posX_, posY_ - bookHeight/2 + height/2, posZ_);
    srglRotatef(roll_, 0, 1, 0);
    srglRotatef(pitch_, 1, 0, 0);
    srglRotatef(yaw_, 0, 0, 1);

    srglBegin(GL_QUADS);
    for (int i = 0 ; i < point_list.size() ; i++) {
      const BookVertex &v = point_list[i];
      srglTexCoord2f(v.s, v.t);
      srglVertex3f(v.x, v.y, v.z);
    }
    srglEnd();

    srglPopMatrix();

  } else {
    //배경같은 경우
    vector<BookVertex> point_list;
    point_list.push_back(v1);
    point_list.push_back(v2);
    point_list.push_back(v3);
    point_list.push_back(v4);

    srglBegin(GL_QUADS);
    for (int i = 0 ; i < point_list.size() ; i++) {
      const BookVertex &v = point_list[i];
      srglTexCoord2f(v.s, v.t);
      srglVertex3f(v.x, v.y, v.z);
    }
    srglEnd();
  }
}
/////////////////
////////////
BookPaperFactory::BookPaperFactory(float cubeWidth, float cubeHeight, float cubeDepth)
  : builder_(new BookPaperBuilder(cubeWidth, cubeHeight, cubeDepth))
{
}
BookPaperFactory::~BookPaperFactory()
{
}
BookPaperPtr BookPaperFactory::createFloor(TextureAtlasSegment *sprite)
{
  builder_->setSprite(sprite);
  builder_->setType(BookPaperTypeFloor);
  builder_->createFloorModel();
  return builder_->build();
}

BookPaperPtr BookPaperFactory::createLeft(TextureAtlasSegment *sprite)
{
  builder_->setSprite(sprite);
  builder_->setType(BookPaperTypeLeft);
  builder_->createLeftModel();
  return builder_->build();
}
BookPaperPtr BookPaperFactory::createRight(TextureAtlasSegment *sprite)
{
  builder_->setSprite(sprite);
  builder_->setType(BookPaperTypeRight);
  builder_->createRightModel();
  return builder_->build();
}
BookPaperPtr BookPaperFactory::createCeil(TextureAtlasSegment *sprite)
{
  builder_->setSprite(sprite);
  builder_->setType(BookPaperTypeCeil);
  builder_->createCeilModel();
  return builder_->build();
}
BookPaperPtr BookPaperFactory::createForward(TextureAtlasSegment *sprite)
{
  builder_->setSprite(sprite);
  builder_->setType(BookPaperTypeForward);
  builder_->createForwardModel();
  return builder_->build();
}
BookPaperPtr BookPaperFactory::createNormal(TextureAtlasSegment *sprite, float x, float y, float z, float w, float h)
{
  builder_->setSprite(sprite);
  builder_->setType(BookPaperTypeNormal);
  builder_->setPos(vec3(x, y, z));
  builder_->createNormalModel(w, h);
  return builder_->build();
}

BookPaperPtr BookPaperFactory::createNormalWithRoll(TextureAtlasSegment *sprite, float x, float y, float z, float w, float h, float roll)
{
  builder_->setSprite(sprite);
  builder_->setType(BookPaperTypeNormal);
  builder_->setPos(vec3(x, y, z));
  builder_->createNormalModel(w, h);
  builder_->setRotate(0, roll, 0);
  return builder_->build();
}

BookPaperPtr BookPaperFactory::createNormalWithPitch(TextureAtlasSegment *sprite, float x, float y, float z, float w, float h, float pitch)
{
  builder_->setSprite(sprite);
  builder_->setType(BookPaperTypeNormal);
  builder_->setPos(vec3(x, y, z));
  builder_->createNormalModel(w, h);
  builder_->setRotate(pitch, 0, 0);
  return builder_->build();
}
BookPaperPtr BookPaperFactory::createNormalWithYaw(TextureAtlasSegment *sprite, float x, float y, float z, float w, float h, float yaw)
{
  builder_->setSprite(sprite);
  builder_->setType(BookPaperTypeNormal);
  builder_->setPos(vec3(x, y, z));
  builder_->createNormalModel(w, h);
  builder_->setRotate(0, 0, yaw);
  return builder_->build();
}
}
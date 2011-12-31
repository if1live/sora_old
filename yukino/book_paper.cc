// Ŭnicode please
#include "yukino_stdafx.h"
#include "book.h"
#include "book_paper.h"
#include "sora/texture_atlas.h"
#include "sora/immediate_mode_emulator.h"
#include "sora/texture.h"
#include "book_paper_builder.h"

using namespace std;
using namespace sora;

namespace yukino {;
BookPaper::BookPaper(TextureSubImage *sprite, BookPaperType type)
  : sprite_(sprite), type_(type), pos_x(0), pos_y(0), pos_z(0),
  pitch(0), roll(0), yaw(0) {
}
BookPaper::~BookPaper() {
}
void BookPaper::draw()
{
  //텍스쳐 활성화
  sora::Texture *tex = sprite_->tex;
  srglBindTexture(GL_TEXTURE_2D, tex->handle);


  if(type_ == kBookPaperNormal) {
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
    SR_ASSERT(height > 0);
    
    float bookHeight = Book::GetInstance().height;

    srglPushMatrix();
    srglTranslatef(pos_x, pos_y - bookHeight/2 + height/2, pos_z);
    srglRotatef(roll, 0, 1, 0);
    srglRotatef(pitch, 1, 0, 0);
    srglRotatef(yaw, 0, 0, 1);

    srglBegin(GL_QUADS);
    for (int i = 0 ; i < point_list.size() ; i++) {
      const BookVertex &v = point_list[i];
      srglTexCoord2f(v.s, v.t);
      srglVertex3f(v.x, v.y, v.z);
    }
    srglEnd();
    /*
    srglBegin(GL_LINE_LOOP);
    for (int i = 0 ; i < point_list.size() ; i++) {
      const BookVertex &v = point_list[i];
      srglTexCoord2f(0, 0);
      srglVertex3f(v.x, v.y, v.z);
    }
    srglEnd();
    */
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
  : builder_(new BookPaperBuilder(cubeWidth, cubeHeight, cubeDepth)) {
}
BookPaperFactory::~BookPaperFactory() {
}
BookPaperPtr BookPaperFactory::CreateFloor(TextureSubImage *sprite) {
  builder_->set_sprite(sprite);
  builder_->set_type(kBookPaperFloor);
  builder_->CreateFloorModel();
  return builder_->build();
}

BookPaperPtr BookPaperFactory::CreateLeft(TextureSubImage *sprite) {
  builder_->set_sprite(sprite);
  builder_->set_type(kBookPaperLeft);
  builder_->CreateLeftModel();
  return builder_->build();
}
BookPaperPtr BookPaperFactory::CreateRight(TextureSubImage *sprite) {
  builder_->set_sprite(sprite);
  builder_->set_type(kBookPaperRight);
  builder_->CreateRightModel();
  return builder_->build();
}
BookPaperPtr BookPaperFactory::CreateCeil(TextureSubImage *sprite) {
  builder_->set_sprite(sprite);
  builder_->set_type(kBookPaperCeil);
  builder_->CreateCeilModel();
  return builder_->build();
}
BookPaperPtr BookPaperFactory::CreateForward(TextureSubImage *sprite) {
  builder_->set_sprite(sprite);
  builder_->set_type(kBookPaperForward);
  builder_->CreateForwardModel();
  return builder_->build();
}
BookPaperPtr BookPaperFactory::CreateNormal(TextureSubImage *sprite, float x, float y, float z, float w, float h) {
  builder_->set_sprite(sprite);
  builder_->set_type(kBookPaperNormal);
  builder_->set_pos(vec3(x, y, z));
  builder_->CreateNormalModel(w, h);
  return builder_->build();
}

BookPaperPtr BookPaperFactory::CreateNormalWithRoll(TextureSubImage *sprite, float x, float y, float z, float w, float h, float roll) {
  builder_->set_sprite(sprite);
  builder_->set_type(kBookPaperNormal);
  builder_->set_pos(vec3(x, y, z));
  builder_->CreateNormalModel(w, h);
  builder_->set_rotate(0, roll, 0);
  return builder_->build();
}

BookPaperPtr BookPaperFactory::CreateNormalWithPitch(TextureSubImage *sprite, float x, float y, float z, float w, float h, float pitch) {
  builder_->set_sprite(sprite);
  builder_->set_type(kBookPaperNormal);
  builder_->set_pos(vec3(x, y, z));
  builder_->CreateNormalModel(w, h);
  builder_->set_rotate(pitch, 0, 0);
  return builder_->build();
}
BookPaperPtr BookPaperFactory::CreateNormalWithYaw(TextureSubImage *sprite, float x, float y, float z, float w, float h, float yaw) {
  builder_->set_sprite(sprite);
  builder_->set_type(kBookPaperNormal);
  builder_->set_pos(vec3(x, y, z));
  builder_->CreateNormalModel(w, h);
  builder_->set_rotate(0, 0, yaw);
  return builder_->build();
}
}
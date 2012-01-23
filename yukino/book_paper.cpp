// Ŭnicode please
#include "yukino_stdafx.h"
#include "book.h"
#include "book_paper.h"
#include "sora/render/texture_atlas.h"
#include "sora/render/immediate_mode_emulator.h"
#include "sora/render/texture.h"
#include "book_paper_builder.h"

#if SR_USE_PCH == 0
#include "sora/render/gl_inc.h"
#endif

using namespace std;
using namespace sora;

namespace yukino {;
BookPaper::BookPaper(TextureSubImage *sprite, BookPaperType type)
  : type_(type), pos_x(0), pos_y(0), pos_z(0),
  pitch(0), roll(0), yaw(0) {
}
BookPaper::BookPaper() 
  : type_(kBookPaperNormal), pos_x(0), pos_y(0), pos_z(0),
  pitch(0), roll(0), yaw(0) {
}
BookPaper::~BookPaper() {
}
void BookPaper::SetBaseAttribute(sora::TextureSubImage *sprite, BookPaperType type) {
  sprite_ = *sprite;
  type_ = type;
}
void BookPaper::draw() {
  //텍스쳐 활성화
  srglBindTexture(GL_TEXTURE_2D, sprite_.GetHandle());

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

}
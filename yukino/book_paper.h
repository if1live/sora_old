// Ŭnicode please
#pragma once

#include "sora/vector.h"

namespace yukino {;
class TextureAtlasSegment;

class BookPaperFactory;
class BookPaperBuilder;
class BookPaper;

//회전 변환의 축과 연산 순서
//pitch : x축을 기준으로 회전(바닥을 야매로 구성할떄 쓸수있다)
//roll : y축을 기준으로 회전(일반적으로 사용될것이라고 예상되는것)
//yaw : z축을 기준으로 회전

typedef enum {
  AxisDirectionX,
  AxisDirectionY,
  AxisDirectionZ,
} AxisDirection;

///@brief 한개의 이미지로 표현가능한 무언가의 타입이다. 이것을 다양하게 함으로써 축회전 및 특수기능을
///xml자체에 명시하지 않고 속성을 명시함으로써 간단하게 책의 장면을 구성할수 있도록 했다
typedef enum {
  BookPaperTypeNormal,	///@brief 일반적으로 사용되는 팝업
  BookPaperTypeFloor,	///@brief 바닥을 표현하기 위해서 사용되는 스프라이트. 벽과 직접적으로 관련된것은 크기가 자동조정된다
  BookPaperTypeLeft,	///@brief 왼쪽 벽을 표현하기 위해서 사용되는 스프라이트
  BookPaperTypeRight,	///@brief 오른쪽 벽을 표현하기 위해서 사용되는 스프라이트..
  BookPaperTypeCeil,	///@brief 천장을 표현하기 위해서 사용됨
  BookPaperTypeForward,	///@brief 정면을 표현하기 위해서 사용됨
  BookPaperTypeFoldAxisY,		///y축을 기준으로 반으로 접힌 팝업
} BookPaperType;

typedef std::tr1::shared_ptr<BookPaper> BookPaperPtr;

struct BookVertex {
  float x, y, z;
  float s, t;
  void texCoord2(float s, float t) { this->s = s; this->t = t; }
  void vertex3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
};

class BookPaper {
public:
  BookPaper(TextureAtlasSegment *sprite, BookPaperType type);
  ~BookPaper();
  BookPaperType getType() const;
  void draw();

  //set translate
  void setPos(float x, float y, float z);
  void setRotate(float pitch, float roll, float yaw);

  float getZ() const;

  //model info
  //4 3
  //1 2
  BookVertex v1;
  BookVertex v2;
  BookVertex v3;
  BookVertex v4;

private:
  TextureAtlasSegment *sprite_;
  const BookPaperType type_;


  //변환에 대한 정보를 넣기
  float posX_;
  float posY_;
  float posZ_;

  float pitch_;
  float roll_;
  float yaw_;
};

///@brief book paper builder를 기반으로 쉽게 뭔가를 만들어낼떄 사용하는 클래스
class BookPaperFactory {
public:
  BookPaperFactory(float cubeWidth, float cubeHeight, float cubeDepth);
  ~BookPaperFactory();
  BookPaperPtr createFloor(TextureAtlasSegment *sprite);
  BookPaperPtr createLeft(TextureAtlasSegment *sprite);
  BookPaperPtr createRight(TextureAtlasSegment *sprite);
  BookPaperPtr createCeil(TextureAtlasSegment *sprite);
  BookPaperPtr createForward(TextureAtlasSegment *sprite);
  BookPaperPtr createNormal(TextureAtlasSegment *sprite, float x, float y, float z, float w, float h);

  BookPaperPtr createNormalWithRoll(TextureAtlasSegment *sprite, float x, float y, float z, float w, float h, float roll);
  BookPaperPtr createNormalWithPitch(TextureAtlasSegment *sprite, float x, float y, float z, float w, float h, float pitch);
  BookPaperPtr createNormalWithYaw(TextureAtlasSegment *sprite, float x, float y, float z, float w, float h, float yaw);
private:
  std::auto_ptr<BookPaperBuilder> builder_;
};
}

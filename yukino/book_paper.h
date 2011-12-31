// Ŭnicode please
#pragma once

#include "sora/vector.h"

namespace sora {;
struct TextureSubImage;
}

namespace yukino {;
class BookPaperFactory;
class BookPaperBuilder;
class BookPaper;

//회전 변환의 축과 연산 순서
//pitch : x축을 기준으로 회전(바닥을 야매로 구성할떄 쓸수있다)
//roll : y축을 기준으로 회전(일반적으로 사용될것이라고 예상되는것)
//yaw : z축을 기준으로 회전

typedef enum {
  kAxisDirectionX,
  kAxisDirectionY,
  kAxisDirectionZ,
} AxisDirection;

///@brief 한개의 이미지로 표현가능한 무언가의 타입이다. 이것을 다양하게 함으로써 축회전 및 특수기능을
///xml자체에 명시하지 않고 속성을 명시함으로써 간단하게 책의 장면을 구성할수 있도록 했다
typedef enum {
  kBookPaperNormal,	///@brief 일반적으로 사용되는 팝업
  kBookPaperFloor,	///@brief 바닥을 표현하기 위해서 사용되는 스프라이트. 벽과 직접적으로 관련된것은 크기가 자동조정된다
  kBookPaperLeft,	///@brief 왼쪽 벽을 표현하기 위해서 사용되는 스프라이트
  kBookPaperRight,	///@brief 오른쪽 벽을 표현하기 위해서 사용되는 스프라이트..
  kBookPaperCeil,	///@brief 천장을 표현하기 위해서 사용됨
  kBookPaperForward,	///@brief 정면을 표현하기 위해서 사용됨
  kBookPaperFoldAxisY,		///y축을 기준으로 반으로 접힌 팝업
} BookPaperType;

typedef std::tr1::shared_ptr<BookPaper> BookPaperPtr;

struct BookVertex {
  float x, y, z;
  float s, t;
  void TexCoord2(float s, float t) { this->s = s; this->t = t; }
  void Vertex3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
};

class BookPaper {
public:
  BookPaper(sora::TextureSubImage *sprite, BookPaperType type);
  ~BookPaper();
  BookPaperType type() const { return type_; }
  void draw();

  //model info
  //4 3
  //1 2
  BookVertex v1;
  BookVertex v2;
  BookVertex v3;
  BookVertex v4;

public:
  //변환에 대한 정보
  float pos_x;
  float pos_y;
  float pos_z;

  float pitch;
  float roll;
  float yaw;

private:
  sora::TextureSubImage *sprite_;
  const BookPaperType type_;
};

///@brief book paper builder를 기반으로 쉽게 뭔가를 만들어낼떄 사용하는 클래스
class BookPaperFactory {
public:
  BookPaperFactory(float cubeWidth, float cubeHeight, float cubeDepth);
  ~BookPaperFactory();
  BookPaperPtr CreateFloor(sora::TextureSubImage *sprite);
  BookPaperPtr CreateLeft(sora::TextureSubImage *sprite);
  BookPaperPtr CreateRight(sora::TextureSubImage *sprite);
  BookPaperPtr CreateCeil(sora::TextureSubImage *sprite);
  BookPaperPtr CreateForward(sora::TextureSubImage *sprite);
  BookPaperPtr CreateNormal(sora::TextureSubImage *sprite, float x, float y, float z, float w, float h);

  BookPaperPtr CreateNormalWithRoll(sora::TextureSubImage *sprite, float x, float y, float z, float w, float h, float roll);
  BookPaperPtr CreateNormalWithPitch(sora::TextureSubImage *sprite, float x, float y, float z, float w, float h, float pitch);
  BookPaperPtr CreateNormalWithYaw(sora::TextureSubImage *sprite, float x, float y, float z, float w, float h, float yaw);
private:
  std::auto_ptr<BookPaperBuilder> builder_;
};
}

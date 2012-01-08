// Ŭnicode please
#pragma once

#include "sora/vector.h"
#include "sora/texture_atlas.h"

namespace yukino {;
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

struct BookVertex {
  float x, y, z;
  float s, t;
  void TexCoord2(float s, float t) { this->s = s; this->t = t; }
  void Vertex3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
};

//복사해도 문제없는 속성만 남기자
//그래야 메모리 관리 생각없이 다루기 편하다
class BookPaper {
public:
  BookPaper();
  BookPaper(sora::TextureSubImage *sprite, BookPaperType type);
  ~BookPaper();
  void SetBaseAttribute(sora::TextureSubImage *sprite, BookPaperType type);
  BookPaperType type() const { return type_; }
  void draw();

public:
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
  sora::TextureSubImage sprite_;
  BookPaperType type_;
};
}

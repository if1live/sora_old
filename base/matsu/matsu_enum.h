//ë
#pragma once

#define MT_PI (3.141592f)
#define MT_PI_2 (MT_PI / 2)
#define MT_PI_4 (MT_PI / 4)

namespace matsu
{
///@brief 축과 관련된 내용은 왠지 언젠가 다시 쓸거같아서..
typedef enum {
  AxisDirectionX,
  AxisDirectionY,
  AxisDirectionZ
} AxisDirection;

///@brief 2차원에서 방향을 표현하는데 쓰이는것
typedef enum {
  Direction2Left,
  Direction2Right,
  Direction2Up,
  Direction2Down
} Direction2;

///@brief 3차원에서 방향을 표현하는데 쓰이는것
typedef enum {
  Direction3Left,
  Direction3Right,
  Direction3Top,
  Direction3Bottom,
  Direction3Front,
  Direction3Back
} Direction3;
}
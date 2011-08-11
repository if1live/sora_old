// Ŭnicode please
#pragma once

///@brief sora프로젝트에서 사용할 데이터타입을 typedef로 재정의한것이다.
///일반적인 경우는 int,float같은것으로도 충분하겠지만
///크로스플랫폼을 고려해서 typedef를 걸어서 쓰자(특히 wchar는 윈동(2byte) 아이폰(4byte)로 낚으니까)

namespace matsu
{
	typedef int i32;
	typedef unsigned int u32;
	typedef short i16;
	typedef unsigned short u16;
	typedef char i8;
	typedef unsigned char u8;

	typedef char byte;
	
	typedef float f32;
	typedef double f64;
}

///@brief matsu::i32같은건 너무 길어서 짧은 이름으로 쓰고싶어 편법을 도입
///그렇다고 네임스페이스 없이 i32를 썼다가는 후에 이름충돌이 날거같고
///using ...을 썼다가는 이름 충돌의 가능성이 남을거같아서
///단축 네임스페이스 명으로 m::i32를 쓸수있도록 했다.
namespace m = matsu;


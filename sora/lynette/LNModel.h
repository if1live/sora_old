// Ŭnicode please
#pragma once

#include <vector>
#include "matsu/MTType.h"
#include "matsu/MTVector.h"
#include "lynette/LNTypedef.h"

namespace lynette
{		
	///@brief sora내부에서 사용되는 모델 클래스이다. 외부 모델은 이 형식에 맞춰서 변환된 후에
	///그려질수 있다. glut에 있던 구를 그리거나 큐브를 그리는 함수의 경우도 이 클래스의 인스턴스를
	///생성한 후에 그린다. 기능상은 엔진에서 쓰이니까 이름을 Model로 하는것이 좋을듯하지만
	///왠지모르게 이름 충돌의 가능성이 느껴져서 sora를 붙였다. 
	///vertex를 저장할때의 각각의 정보의 크기는...
	///pos : float * 3
	///color : float * 4
	///texcoord : float * 2
	///normal : float * 3
	///으로 일단 정해져있다. 		
	class Model {
	public:
		Model(matsu::u32 drawMode, matsu::u32 useFlag = 0);
		~Model();

		///@brief vertex정보를 채우기
		void addVertexData(matsu::f32 a, matsu::f32 b);
		void addVertexData(matsu::f32 a, matsu::f32 b, matsu::f32 c);
		void addVertexData(matsu::f32 a, matsu::f32 b, matsu::f32 c, matsu::f32 d);
		void addVertexData(const matsu::vec2 &v);
		void addVertexData(const matsu::vec3 &v);
		void addVertexData(const matsu::vec4 &v);
		void setVertexData(const std::vector<matsu::f32> &data);

		///@brief index 정보 채우기
		void addIndexData(matsu::i16 a);
		void setIndexData(const std::vector<matsu::u16> &data);

		const matsu::f32* getVertexData() const;
		const matsu::u16* getIndexData() const;
		matsu::i32 getStride() const;
		matsu::i32 getIndexCount() const;
		matsu::i32 getVertexCount() const;

		const matsu::f32* getPosPtr() const;
		const matsu::f32* getColorPtr() const;
		const matsu::f32* getTexCoordPtr() const;
		const matsu::f32* getNormalPtr() const;

		bool isUseColor() const;
		bool isUseTexCoord() const;
		bool isUseNormal() const;
		matsu::u32 getDrawMode() const;
		bool isIndexDataExist() const;

	private:
		bool useColor_;
		bool useTexCoord_;
		bool useNormal_;

		///@brief 어떤 형태로 그릴것인지를 결정
		matsu::u32 drawMode_;

		///@brief vertex를 구성하는 정보를 저장하는 배열이다. 모든 데이터는 통째로 여기에 들어간다
		///Vertex3같은 클래스를 사용하지 않는 이유는 필요한 정보만 저장해서 메모리를 아끼기 위해서이닫
		///배열에 저장될때의 우선순의는 pos > color > texCoord > normal이다
		std::vector<matsu::f32> vertexdata_;

		///@brief vertex의 index정보를 가지고 있는 배열이다. 
		///비어있으면 인덱스를 이용해서 그리지 않는것으로 갅주한다
		std::vector<matsu::u16> indexdata_;
	};
}
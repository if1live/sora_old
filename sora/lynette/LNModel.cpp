// Ŭnicode please
#include "../SoraStdAfx.h"

#include "lynette/LNModel.h"
#include "lynette/LNTypedef.h"

using namespace matsu;

namespace lynette
{
	Model::Model(matsu::u32 drawMode, matsu::u32 useFlag)
		: useColor_(false), useTexCoord_(false), useNormal_(false), drawMode_(drawMode)
	{
		if((useFlag & LYNETTE_USE_COLOR) == LYNETTE_USE_COLOR)
		{
			useColor_ = true;
		}				
		if((useFlag & LYNETTE_USE_TEXCOORD) == LYNETTE_USE_TEXCOORD)
		{
			useTexCoord_ = true;
		}
		if((useFlag & LYNETTE_USE_NORMAL) == LYNETTE_USE_NORMAL)
		{
			useNormal_ = true;
		}
	}
	Model::~Model()
	{
	}
	const matsu::f32* Model::getVertexData() const
	{
		MT_ASSERT(vertexdata_.size() > 0 && "vertex data is empty!!");
		return &vertexdata_[0];
	}
	const matsu::u16* Model::getIndexData() const
	{
		return &indexdata_[0];
	}
	const matsu::f32* Model::getPosPtr() const
	{
		return getVertexData();
	}
	const matsu::f32* Model::getColorPtr() const
	{
		MT_ASSERT(isUseColor());
		return getPosPtr() + 3;
	}
	const matsu::f32* Model::getTexCoordPtr() const
	{
		MT_ASSERT(isUseTexCoord());
		matsu::f32* ptr = const_cast<matsu::f32*>(getPosPtr());
		ptr += 3;	//pos end
		if(isUseColor())
		{
			ptr += 4;
		}
		return ptr;
	}
	const matsu::f32* Model::getNormalPtr() const
	{
		MT_ASSERT(isUseNormal());
		matsu::f32* ptr = const_cast<matsu::f32*>(getPosPtr());
		ptr += 3;	//pos end
		if(isUseColor())
		{
			ptr += 4;
		}
		if(isUseTexCoord())
		{
			ptr += 2;
		}
		return ptr;
	}
	matsu::i32 Model::getStride() const
	{
		///pos용 3개는 항상 기본
		matsu::i32 stride = 3;
		if(isUseColor() == true)
		{
			stride += 4;
		}
		if(isUseTexCoord() == true)
		{
			stride += 2;
		}
		if(isUseNormal() == true)
		{
			stride += 3;
		}
		return stride * sizeof(matsu::f32);
	}
	matsu::i32 Model::getIndexCount() const
	{
		return indexdata_.size();
	}
	matsu::i32 Model::getVertexCount() const
	{
		matsu::u32 size = vertexdata_.size() * sizeof(matsu::f32);
		matsu::u32 stride = getStride();
		return size / stride;
	}

	bool Model::isUseTexCoord() const
	{
		return useTexCoord_;
	}
	bool Model::isUseColor() const
	{
		return useColor_;
	}
	bool Model::isUseNormal() const
	{
		return useNormal_;
	}
	matsu::u32 Model::getDrawMode() const
	{
		return drawMode_;
	}
	bool Model::isIndexDataExist() const
	{
		if(indexdata_.size() == 0)
		{
			return false;
		}
		else 
		{
			return true;
		}
	}
	void Model::addVertexData(matsu::f32 a, matsu::f32 b)
	{
		vertexdata_.push_back(a);
		vertexdata_.push_back(b);
	}
	void Model::addVertexData(matsu::f32 a, matsu::f32 b, matsu::f32 c)
	{
		vertexdata_.push_back(a);
		vertexdata_.push_back(b);
		vertexdata_.push_back(c);
	}
	void Model::addVertexData(matsu::f32 a, matsu::f32 b, matsu::f32 c, matsu::f32 d)
	{
		vertexdata_.push_back(a);
		vertexdata_.push_back(b);
		vertexdata_.push_back(c);
		vertexdata_.push_back(d);
	}
	void Model::addVertexData(const matsu::vec2 &v)
	{
		vertexdata_.push_back(v.getX());
		vertexdata_.push_back(v.getY());
	}
	void Model::addVertexData(const matsu::vec3 &v)
	{
		vertexdata_.push_back(v.getX());
		vertexdata_.push_back(v.getY());
		vertexdata_.push_back(v.getZ());
	}
	void Model::addVertexData(const matsu::vec4 &v)
	{
		vertexdata_.push_back(v.x());
		vertexdata_.push_back(v.y());
		vertexdata_.push_back(v.z());
		vertexdata_.push_back(v.w());
	}

	void Model::setVertexData(const std::vector<matsu::f32> &data)
	{
		vertexdata_ = data;
	}
		
	void Model::addIndexData(matsu::i16 a)
	{
		indexdata_.push_back(a);
	}
	void Model::setIndexData(const std::vector<matsu::u16> &data)
	{
		indexdata_ = data;
	}
}
// Ŭnicode please
namespace lynette
{
	template<typename T>
	BoundingVolumeTpl<T>::BoundingVolumeTpl(const sora::model::Model &model)
		:	left_(INT_MAX),
			right_(-INT_MAX),
			bottom_(INT_MAX),
			top_(-INT_MAX),
			front_(-INT_MAX),
			back_(INT_MAX)
	{
		const float *posPtr = model.getPosPtr();
		int stride = model.getStride();
		int vertexCnt = model.getVertexCount();

		for(int i = 0 ; i < vertexCnt ; i++)
		{
			///3d니가 float3개로 좌표를 표현할 것이다 
			int index = (stride / sizeof(matsu::f32)) * i;
			const matsu::f32 *vertexPtr = posPtr+index;
			float x = vertexPtr[0];
			float y = vertexPtr[1];
			float z = vertexPtr[2];

			if(x < left_)
			{
				left_ = x;
			}
			else if(x > right_)
			{
				right_ = x;
			}
			if(y < bottom_)
			{
				bottom_ = y;
			}
			else if(y > top_)
			{
				top_ = y;
			}
			if(z > front_)
			{
				front_ = z;
			}
			else if(z < back_)
			{
				back_ = z;
			}
		}
	}
	template<typename T>
	BoundingVolumeTpl<T>::~BoundingVolumeTpl()
	{
	}

	template<typename T>
	T BoundingVolumeTpl<T>::getLeft() const
	{
		return left_;
	}
	template<typename T>
	T BoundingVolumeTpl<T>::getRight() const
	{
		return right_;
	}
	template<typename T>
	T BoundingVolumeTpl<T>::getTop() const
	{
		return top_;
	}
	template<typename T>
	T BoundingVolumeTpl<T>::getBottom() const
	{
		return bottom_;
	}
	template<typename T>
	T BoundingVolumeTpl<T>::getFront() const
	{
		return front_;
	}
	template<typename T>
	T BoundingVolumeTpl<T>::getBack() const
	{
		return back_;
	}
	template<typename T>
	T BoundingVolumeTpl<T>::getWidth() const
	{
		return getRight() - getLeft();
	}
	template<typename T>
	T BoundingVolumeTpl<T>::getHeight() const
	{
		return getTop() - getBottom();
	}
	template<typename T>
	T BoundingVolumeTpl<T>::getDepth() const
	{
		return getFront() - getBack();
	}
}
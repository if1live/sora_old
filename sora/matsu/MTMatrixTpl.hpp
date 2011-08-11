#pragma once

//template impl
namespace matsu
{
	template<typename T, int row, int col>
	template<typename T2>
	MatrixTpl<T,row,col>::MatrixTpl(const MatrixTpl<T2,row,col> &o)
	{
		for(int i = 0 ; i < row*col ; i++)
		{
			data_[i] = static_cast<T> (o.data_[i]);
		}
	}
	
	template<typename T, int row, int col>
	MatrixTpl<T,row,col>::MatrixTpl()
	{
		memset(data_, 0, sizeof(T) * row * col);
		int minValue = row > col ? col : row;
		for(int i = 0 ; i < minValue ; i++)
		{
			set(i, i, 1);
		}
	}
	template<typename T, int row, int col>
	template<unsigned int N>
	MatrixTpl<T,row,col>::MatrixTpl(const T(&data)[N])
	{
		setData(data);
	}
	template<typename T, int row, int col>
	template<int row2, int col2>
	MatrixTpl<T,row,col>::MatrixTpl(const MatrixTpl<T,row2,col2> &o)
	{
		memset(data_, 0, sizeof(T) * row * col);
		for(int i = 0 ; i < row2 && i < row ; i++)
		{
			for(int j = 0 ; j < col && j < col2 ; j++)
			{
				set(i, j, o.get(i, j));
			}
		}
	}
	
	template<typename T, int row, int col>
	MatrixTpl<T,row,col>::~MatrixTpl()
	{
	}
	
	template<typename T, int row, int col>
	template<unsigned int N>
	void MatrixTpl<T,row,col>::setData(const T(&data)[N])
	{
		MT_ASSERT(N == row*col);
		memcpy(data_, data, sizeof(T) * row * col);
	}
	template<typename T,int row, int col>
	void MatrixTpl<T,row,col>::setData(const T *data)
	{
		//data의 크기가 보장되어있지 않은게 좀 걸리는데...
		memcpy(data_, data, sizeof(T) * row * col);
	}

	template<typename T, int row, int col>
	T MatrixTpl<T,row,col>::get(int r, int c) const
	{
		return data_[getIndex(r, c)];
	}
	
	template<typename T, int row, int col>
	void MatrixTpl<T,row,col>::set(int r, int c, T value)
	{
		data_[getIndex(r, c)] = value;
	}
	
	template<typename T, int row, int col>
	T* MatrixTpl<T,row,col>::pointer()
	{
		return data_;
	}
	
	template<typename T, int row, int col>
	const T* MatrixTpl<T,row,col>::pointer() const
	{
		return data_;
	}
	
	template<typename T, int row, int col>
	int MatrixTpl<T,row,col>::getIndex(int r, int c) const
	{
		int index = c*row + r;
		MT_ASSERT(index >= 0 && index < row*col);
		return index;
	}
	template<typename T, int row, int col>
	MatrixTpl<T,row,col> MatrixTpl<T,row,col>::operator+(const MatrixTpl<T,row,col> &o) const
	{
		MatrixTpl<T,row,col> tmp = *this;
		tmp += o;
		return tmp;
	}
	template<typename T, int row, int col>
	MatrixTpl<T,row,col> &MatrixTpl<T,row,col>::operator+=(const MatrixTpl<T,row,col> &o)
	{
		for(int i = 0 ; i < row*col ; i++)
		{
			data_[i] += o.data_[i];
		}
	}
	template<typename T, int row, int col>
	MatrixTpl<T,row,col> MatrixTpl<T,row,col>::operator-(const MatrixTpl<T,row,col> &o) const
	{
		MatrixTpl<T,row,col> tmp = *this;
		tmp -= o;
		return tmp;
	}
	template<typename T, int row, int col>
	MatrixTpl<T,row,col> &MatrixTpl<T,row,col>::operator-=(const MatrixTpl<T,row,col> &o)
	{
		for(int i = 0 ; i < row*col ; i++)
		{
			data_[i] -= o.data_[i];
		}
	}
	
	template<typename T, int row, int col>
	MatrixTpl<T,row,col> MatrixTpl<T,row,col>::operator-() const
	{
		MatrixTpl<T,row,col> tmp;
		for(int i = 0 ; i < row*col ; i++)
		{
			tmp.data_[i] -= data_[i];
		}
		return tmp;
	}
	
	template<typename T, int row, int col>
	bool MatrixTpl<T,row,col>::operator==(const MatrixTpl<T,row,col> &o) const
	{
		for(int i = 0 ; i < row*col ; i++)
		{
			if(data_[i] != o.data_[i])
			{
				return false;
			}
		}
		return true;
	}
	
	template<typename T, int row, int col>
	bool MatrixTpl<T,row,col>::operator!=(const MatrixTpl<T,row,col> &o) const
	{
		return !(*this == o);
	}
	
	template<typename T, int row, int col>
	MatrixTpl<T,col,row> MatrixTpl<T,row,col>::transpose() const
	{
		MatrixTpl<T,col,row> result;
		for(int i = 0 ; i < row ; i++)
		{
			for(int j = 0 ; j < col ; j++)
			{
				result.set(j, i, get(i, j));
			}
		}
		return result;
	}

	template<typename T, int row, int col>
	VectorTpl<T,col> MatrixTpl<T,row,col>::getRow(int r) const
	{
		VectorTpl<T,col> result;
		for(int i = 0 ; i < col ; i++)
		{
			result.set(i, get(r, i));
		}
		return result;
	}
	template<typename T, int row, int col>
	VectorTpl<T,row> MatrixTpl<T,row,col>::getCol(int c) const
	{
		VectorTpl<T,row> result;
		for(int i = 0 ; i < row ; i++)
		{
			result.set(i, get(i, c));
		}
		return result;
	}
	
	template<typename T, int row, int col>
	void MatrixTpl<T,row,col>::setRow(int r, const VectorTpl<T,col> &o)
	{
		for(int i = 0 ; i < col ; i++)
		{
			set(r, i, o.get(i));
		}
	}
	
	template<typename T, int row, int col>
	void MatrixTpl<T,row,col>::setCol(int c, const VectorTpl<T,row> &o)
	{
		for(int i = 0 ; i < row ; i++)
		{
			set(i, c, o.get(i));
		}
	}
	
	template<typename T, int row, int col>
	template<int size>
	MatrixTpl<T,size,size> MatrixTpl<T,row,col>::operator*=(const MatrixTpl<T,size,size> &o)
	{
		MatrixTpl<T,size,size> result;
		MT_ASSERT(row == col && "support only X*X matrix");
		for(int i = 0 ; i < size ; i++)
		{
			for(int j = 0 ; j < size ; j++)
			{
				VectorTpl<T,size> rowVec = getRow(i);
				VectorTpl<T,size> colVec = o.getCol(j);
				T value = rowVec.dot(colVec);
				result.set(i, j, value);
			}
		}
		memcpy(data_, result.data_, sizeof(T) * row * col);
	}
	template<typename T, int row, int col>
	template<int col2>
	MatrixTpl<T,row,col2> MatrixTpl<T,row,col>::operator*(const MatrixTpl<T,col,col2> &o) const
	{
		MatrixTpl<T,row,col2> result;
		for(int i = 0 ; i < row ; i++)
		{
			for(int j = 0 ; j < col2 ; j++)
			{
				VectorTpl<T,col> rowVec = getRow(i);
				VectorTpl<T,col> colVec = o.getCol(j);
				T value = rowVec.dot(colVec);
				result.set(i, j, value);
			}
		}
		return result;
	}
	
	template<typename T, int row, int col>
	MatrixTpl<T,row,col> MatrixTpl<T,row,col>::operator*(T s) const
	{
		MatrixTpl<T,row,col> tmp = *this;
		tmp *= s;
		return tmp;
	}
	
	template<typename T, int row, int col>
	MatrixTpl<T,row,col> &MatrixTpl<T,row,col>::operator*=(T s)
	{
		for(int i = 0 ; i < row*col ; i++)
		{
			data_[i] *= s;
		}
		return *this;
	}

	template<typename T, int row, int col>
	VectorTpl<T,row> MatrixTpl<T,row,col>::operator*(const VectorTpl<T,col> &o) const
	{
		T data[row];
		for(int i = 0 ; i < row ; i++)
		{
			VectorTpl<T,col> rowVec = getRow(i);
			data[i] = rowVec.dot(o);
		}
		return VectorTpl<T,row>(data);
	}
}
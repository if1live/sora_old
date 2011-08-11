#pragma once

#include "matsu/MTVectorTpl.h"

namespace matsu
{
	///@brief 여러종류의 행렬을 적젏하게 처리하기 위해서 만든 클래스이다.
	///인자로 row, col을 명시할때는 행-열로 받지만
	///내부에서 저장은 col-row순으로 되면(opengl친화적)
	///배열을 인자로 받을때도 그렇게 처리된다
	template<typename T, int row, int col>
	class MatrixTpl {
	public:
		enum {
			Row = row,
			Col = col,
		};
	public:
		MatrixTpl();
		template<unsigned int N>
		MatrixTpl(const T(&data)[N]);
		template<int row2, int col2>
		MatrixTpl(const MatrixTpl<T,row2,col2> &o);
		template<typename T2>
		MatrixTpl(const MatrixTpl<T2,row,col> &o);
		~MatrixTpl();
		
		T get(int r, int c) const;
		void set(int r, int c, T value);
		template<unsigned int N>
		void setData(const T(&data)[N]);
		void setData(const T *data);
		
		T* pointer();
		const T* pointer() const;
		
		//operator
		MatrixTpl<T,row,col> operator+(const MatrixTpl<T,row,col> &o) const;
		MatrixTpl<T,row,col> &operator+=(const MatrixTpl<T,row,col> &o);
		MatrixTpl<T,row,col> operator-(const MatrixTpl<T,row,col> &o) const;
		MatrixTpl<T,row,col> &operator-=(const MatrixTpl<T,row,col> &o);
		MatrixTpl<T,row,col> operator-() const;
		///@brief *=는 자신의 값 자체가 바뀌므로 행렬이 정행렬일때만 가능하다
		template<int size>
		MatrixTpl<T,size,size> operator*=(const MatrixTpl<T,size,size> &o);
		template<int col2>
		MatrixTpl<T,row,col2> operator*(const MatrixTpl<T,col,col2> &o) const;
		MatrixTpl<T,row,col> operator*(T s) const;
		MatrixTpl<T,row,col> &operator*=(T s);

		VectorTpl<T,row> operator*(const VectorTpl<T,col> &o) const;
		
		bool operator==(const MatrixTpl<T,row,col> &o) const;
		bool operator!=(const MatrixTpl<T,row,col> &o) const;
		
		VectorTpl<T,col> getRow(int r) const;
		VectorTpl<T,row> getCol(int c) const;
		void setRow(int r, const VectorTpl<T,col> &o);
		void setCol(int c, const VectorTpl<T,row> &o);
		
		MatrixTpl<T,col,row> transpose() const;
		
	private:
		int getIndex(int r, int c) const;
		T data_[row*col];
	};
	
	typedef MatrixTpl<int,2,2> imat2;
	typedef MatrixTpl<float,2,2> mat2;
	typedef MatrixTpl<int,3,3> imat3;
	typedef MatrixTpl<float,3,3> mat3;
	typedef MatrixTpl<int,4,4> imat4;
	typedef MatrixTpl<float,4,4> mat4;
}

#include "matsu/MTMatrixTpl.hpp"
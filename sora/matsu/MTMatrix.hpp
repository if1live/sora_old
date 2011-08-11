// Ŭnicode please
#pragma once
namespace matsu
{
	namespace mat
	{
		template <typename T>
		MatrixTpl<T,4,4> identity()
		{
			return MatrixTpl<T,4,4>();
		}
		template <typename T>
		MatrixTpl<T,4,4> translate(T x, T y, T z)
		{
			MatrixTpl<T,4,4> m;
			T data[] = {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				x, y, z, 1
			};
			m.setData(data);
			return m;
		}
		template <typename T>
		MatrixTpl<T,4,4> scale(T s)
		{
			return scale(s, s, s);
		}
		template<typename T>
		MatrixTpl<T,4,4> scale(T x, T y, T z)
		{
			MatrixTpl<T,4,4> m;
			T data[] = {
				x, 0, 0, 0,
				0, y, 0, 0,
				0, 0, z, 0,
				0, 0, 0, 1
			};
			m.setData(data);
			return m;
		}
		template <typename T>
		MatrixTpl<T,4,4> xRotate(T degrees)
		{
			T radians = degrees * 3.14159f / 180.0f;
			T s = std::sin(radians);
			T c = std::cos(radians);

			MatrixTpl<T,4,4> m;
			T data[] = {
				1, 0, 0, 0,
				0, c, s, 0,
				0, -s, c, 0,
				0, 0, 0, 1
			};
			m.setData(data);
			return m;
		}
		template <typename T>
		MatrixTpl<T,4,4> yRotate(T degrees)
		{
			T radians = degrees * 3.14159f / 180.0f;
			T s = std::sin(radians);
			T c = std::cos(radians);

			MatrixTpl<T,4,4> m;
			T data[] = {
				c, 0, -s, 0,
				0, 1, 0, 0,
				s, 0, c, 0,
				0, 0, 0, 1,
			};
			m.setData(data);
			return m;
		}
		template <typename T>
		MatrixTpl<T,4,4> zRotate(T degrees)
		{
			T radians = degrees * 3.14159f / 180.0f;
			T s = std::sin(radians);
			T c = std::cos(radians);

			MatrixTpl<T,4,4> m;
			T data[] = {
				c, s, 0, 0,
				-s, c, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
			m.setData(data);
			return m;
		}
		
		template<typename T>
		MatrixTpl<T,4,4> xAxisShear(T y, T z)
		{
			MatrixTpl<T,4,4> m;
			T data[] = {
				1, 0, 0, 0,
				y, 1, 0, 0,
				z, 0, 1, 0,
				0, 0, 0, 1
			};
			m.setData(data);
			return m;
		}
		template<typename T>
		MatrixTpl<T,4,4> yAxisShear(T x, T z)
		{
			MatrixTpl<T,4,4> m;
			T data[] = {
				1, x, 0, 0,
				0, 1, 0, 0,
				0, z, 1, 0,
				0, 0, 0, 1,
			};
			m.setData(data);
			return m;
		}
		template<typename T>
		MatrixTpl<T,4,4> zAxisShear(T x, T y)
		{
			MatrixTpl<T,4,4> m;
			T data[] = {
				1, 0, x, 0,
				0, 1, y, 0,
				0, 0, 1, 0,
				0, 0, 0, 1,
			};
			m.setData(data);
			return m;
		}

//#pragma mark camera function
		template<typename T>
		MatrixTpl<T,4,4> ortho(T left, T right, T bottom, T top, T nearVal, T farVal)
		{
			//http://www.opengl.org/sdk/docs/man/xhtml/glOrtho.xml
			T a = static_cast<T>(2) / (right - left);
			T b = static_cast<T>(2) / (top - bottom);
			T c = static_cast<T>(-2) / (farVal - nearVal);
			T tx = -(right+left) / (right-left);
			T ty = -(top+bottom) / (top-bottom);
			T tz = -(farVal+nearVal) / (farVal-nearVal);

			MatrixTpl<T,4,4> m;
			T data[] = {
				a, 0, 0, 0,
				0, b, 0, 0, 
				0, 0, c, 0,
				tx, ty, tz, 1
			};
			m.setData(data);

			return m;
		}

		template<typename T>
		MatrixTpl<T,4,4> frustum(T left, T right, T bottom, T top, T nearVal, T farVal)
		{
			T a = static_cast<T>(2) * nearVal / (right - left);
			T b = static_cast<T>(2) * nearVal / (top - bottom);
			T c = (right + left) / (right - left);
			T d = (top + bottom) / (top - bottom);
			T e = - (farVal + nearVal) / (farVal - nearVal);
			T f = -static_cast<T>(2) * farVal * nearVal / (farVal - nearVal);
			MatrixTpl<T,4,4> m;
			T data[] = {
				a, 0, 0, 0,
				0, b, 0, 0,
				c, d, e, -1,
				0, 0, f, 1
			};
			m.setData(data);
			return m;
		}

		template<typename T>
		MatrixTpl<T,4,4> lookAt(const matsu::VectorTpl<T,3> &eye, const matsu::VectorTpl<T,3> &target, const matsu::VectorTpl<T,3> &up)
		{
			matsu::VectorTpl<T,3> z = (eye-target).normalize();
			matsu::VectorTpl<T,3> x = up.cross(z).normalize();
			matsu::VectorTpl<T,3> y = z.cross(x).normalize();

			MatrixTpl<T,4,4> m;
			m.setCol(0, vec4(x, 0));
			m.setCol(1, vec4(y, 0));
			m.setCol(2, vec4(z, 0));
			m.setCol(3, vec4(0, 0, 0, 1));

			VectorTpl<T,4> eyePrime = m * matsu::VectorTpl<T,4>(-eye, static_cast<T> (1));
			m = m.transpose();
			m.setCol(3, eyePrime);

			return m;
		}
	}
}

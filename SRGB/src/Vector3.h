#pragma once

#include<iostream>
#include <type_traits>
#include <cassert>

template <typename T>
class Vec3 {
	public:
		//Members
		union {
			T x; T s; T r;
		};
		union {
			T y; T u; T g;
		};
		union {
			T z; T v; T b;
		};



		//=============Constructors====================
		Vec3() : x(0), y(0), z(0) {}

		Vec3(T nX, T nY, T nZ) : x(nX), y(nY), z(nZ) {}

		//copy constructor
		Vec3(const Vec3<T> &input) : x(input.x), y(input.y), z(input.z) {}




		//========Access operators====================

		Vec3<T> operator=(const Vec3<T> &other) 
		{
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}


		T& operator[](int n)
		{
			assert(n >= 0 && n <= 2);

			if (n == 0)
			{
				return x;
			}
			else if (n == 1)
			{
				return y;
			}
			else
			{
				return z;
			}
		}


		//============scalar operators=================
		Vec3<T> operator+(T other) const
		{
			return Vec3<T>(x + other, y + other, z + other);
		}

		Vec3<T> operator-(T other) const
		{
			return Vec3<T>(x - other, y - other, z - other);
		}

		Vec3<T> operator*(T other) const
		{
			return Vec3<T>(x * other, y * other, z * other);
		}


		Vec3<T>& operator+=(T other) 
		{
			x += other;
			y += other;
			z += other;
			return *this;
		}

		Vec3<T>& operator-=(T other)
		{
			x -= other;
			y -= other;
			z -= other;
			return *this;
		}
		//*= and /= could be added


		//============vector operators=================

		Vec3<T> operator+(const Vec3<T> &other) const
		{
			return Vec3<T>(x+other.x, y+other.y, z+other.z);
		}

		Vec3<T> operator-(const Vec3<T> &other) const
		{
			return Vec3<T>(x - other.x, y - other.y, z - other.z);
		}

		Vec3<T> operator*(const Vec3<T> &other) const
		{
			return Vec3<T>(x * other.x, y * other.y, z * other.z);
		}	
	
		T dot(const Vec3<T> &other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}

		Vec3<T> cross(const Vec3<T> &other) const
		{
			return Vec3<T>( (y * other.z - other.y * z), (z * other.x - other.z * x), (x * other.y - other.x * y) );
		}


		Vec3<T>& operator+=(const Vec3<T> &other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		Vec3<T>& operator-=(const Vec3<T> &other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		//=========== Compare operator ==================

		bool operator==(const Vec3<T> &other)
		{
			return std::fabs(x-other.x)<FLT_EPSILON && std::fabs(y - other.y) < FLT_EPSILON && std::fabs(z - other.z) < FLT_EPSILON;
		}

		bool operator!=(const Vec3<T> &other)
		{
			return !(*this==other);
		}


		//=========== Other functions ===================

		T length() const
		{
			return (T) sqrt(x*x + y*y + z*z);
		}

		void normalize()
		{
			T len = length();
			x = x/len;
			y = y/len;
			z = z/len;
		}

};

//Common type definitions
typedef class Vec3<int> Vec3i;
typedef class Vec3<float> Vec3f;
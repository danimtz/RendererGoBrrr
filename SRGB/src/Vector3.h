#pragma once

#include <iostream>
#include <type_traits>
#include <cassert>

//Cointains template classes for Vec3, Vec2



template <typename T>
class Vec3 {
public:
	//Members

	
	union {
		T x; T s; 
	};
	union {
		T y; T u; 
	};
	union {
		T z; T v; 
	};
	//For homogenous coordinates perspective divide
	T w;// This should be removed and made into Vector4 in a real math library. as it produces memory waste when w is not needed. 

	//=============Constructors====================
	Vec3() : x(0), y(0), z(0), w(1) {}

	Vec3(T n) : x(n), y(n), z(n), w(1) {}

	Vec3(T nX, T nY, T nZ) : x(nX), y(nY), z(nZ), w(1) {}

	Vec3(T nX, T nY, T nZ, T nw) : x(nX), y(nY), z(nZ), w(nw) {} //Used in Mat4 multiplication operator overload

	//copy constructor
	Vec3(const Vec3<T> &input) : x(input.x), y(input.y), z(input.z), w(input.w) {}




	//========Access operators====================

	Vec3<T> operator=(const Vec3<T> &other) 
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}

	template<class otherT>
	Vec3<T> operator=(const Vec3<otherT> &other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}

	T& operator[](int n)
	{
		
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


	const T& operator[](int n) const
	{
		
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

	Vec3<T> operator-() const
	{
		return Vec3<T>(-x, -y, -z);
	}


	Vec3<T> operator*(T other) const
	{
		return Vec3<T>(x * other, y * other, z * other);
	}


	Vec3<T> operator/(T other) const
	{
		return Vec3<T>(x / other, y / other, z / other);
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
	
	Vec3<T> operator/(const Vec3<T>& other) const
	{
		return Vec3<T>(x / other.x, y / other.y, z / other.z);
	}

	T dot(const Vec3<T> &other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	Vec3<T> cross(const Vec3<T> &other) const
	{
		return Vec3<T>( (y * other.z - other.y * z), (z * other.x - other.z * x), (x * other.y - other.x * y) );
	}

	Vec3<T> mult(const Vec3<T>& other) const
	{
		return Vec3<T>(x * other.x, y * other.y, z * other.z);
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

	Vec3<T> negate()
	{
		Vec3<T> negated;
		negated.x = -x;
		negated.y = -y;
		negated.z = -z;

		return negated;
	}

	void perspecDiv()
	{
		float invW = 1.0f/w;
		x *= invW;
		y *= invW;
		z *= invW;
		//w = 1; LEAVE W as is. This is only used in renderer->rasterizer part of pipeline
	}

	static Vec3<T> reflect(const Vec3<T> &incident, const Vec3<T> &normal)
	{
		Vec3<T> reflection;
		
		reflection = incident - (normal*(incident.dot(normal) * 2.0f));

		return reflection;
	}

	static void printVec3ToConsole(const Vec3<T>& vec) 
	{
		printf("[%f] [%f] [%f]\n", vec.x, vec.y, vec.z);

	}

};

//Common type definitions
typedef class Vec3<int> Vec3i;
typedef class Vec3<float> Vec3f;



template <typename T>
class Vec2 {
public:
	//Members
	union {
		T x; T u; T r;
	};
	union {
		T y; T v; T g;
	};




	//=============Constructors====================
	Vec2() : x(0), y(0) {}

	Vec2(T nX, T nY) : x(nX), y(nY) {}

	//copy constructor
	Vec2(const Vec2<T> &input) : x(input.x), y(input.y) {}




	//========Access operators====================

	Vec2<T> operator=(const Vec2<T> &other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	//Convert vec3 to vec2 (copy x and y only)
	template<class otherT>
	Vec2<T> operator=(const Vec3<otherT> &other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}


	T& operator[](int n)
	{
		assert(n >= 0 && n <= 1);

		if (n == 0)
		{
			return x;
		}
		else
		{
			return y;
		}
	}

	const T& operator[](int n) const
	{
		assert(n >= 0 && n <= 1);

		if (n == 0)
		{
			return x;
		}
		else
		{
			return y;
		}
	}


	//============scalar operators=================
	Vec2<T> operator+(T other) const
	{
		return Vec2<T>(x + other, y + other);
	}

	Vec2<T> operator-(T other) const
	{
		return Vec2<T>(x - other, y - other);
	}

	Vec2<T> operator*(T other) const
	{
		return Vec2<T>(x * other, y * other);
	}


	Vec2<T>& operator+=(T other)
	{
		x += other;
		y += other;
		return *this;
	}

	Vec2<T>& operator-=(T other)
	{
		x -= other;
		y -= other;
		return *this;
	}
	//*= and /= could be added


	//============vector operators=================

	Vec2<T> operator+(const Vec2<T> &other) const
	{
		return Vec2<T>(x + other.x, y + other.y);
	}

	Vec2<T> operator-(const Vec2<T> &other) const
	{
		return Vec2<T>(x - other.x, y - other.y);
	}

	Vec2<T> operator*(const Vec2<T> &other) const
	{
		return Vec2<T>(x * other.x, y * other.y);
	}

	T dot(const Vec2<T> &other) const
	{
		return x * other.x + y * other.y;
	}



	Vec2<T>& operator+=(const Vec2<T> &other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2<T>& operator-=(const Vec2<T> &other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	//=========== Compare operator ==================

	bool operator==(const Vec2<T> &other)
	{
		return std::fabs(x - other.x) < FLT_EPSILON && std::fabs(y - other.y) < FLT_EPSILON;
	}

	bool operator!=(const Vec2<T> &other)
	{
		return !(*this == other);
	}


	//=========== Other functions ===================

	T length() const
	{
		return (T)sqrt(x * x + y * y);
	}

	void normalize()
	{
		T len = length();
		x = x / len;
		y = y / len;
	}

};

//Common type definitions
typedef class Vec2<int> Vec2i;
typedef class Vec2<float> Vec2f;




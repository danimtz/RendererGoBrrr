#pragma once

#include<iostream>
#include <type_traits>
#include <cassert>

//Cointains template classes for Vec2, Vec3, Mat4


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
		return std::fabs(x - other.x) < FLT_EPSILON && std::fabs(y - other.y) < FLT_EPSILON ;
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
	//For homogenous coordinates perspective divide
	T w;

	//=============Constructors====================
	Vec3() : x(0), y(0), z(0), w(1) {}

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
		return *this;
	}

	template<class otherT>
	Vec3<T> operator=(const Vec3<otherT> &other)
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


	const T& operator[](int n) const
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

	void perspecDiv()
	{
		x /= w;
		y /= w;
		z /= w;
		w = 1;
	}

};

//Common type definitions
typedef class Vec3<int> Vec3i;
typedef class Vec3<float> Vec3f;



#if 0

template<typename T>
class Mat4 
{
public:

	T data[16];

	//=========== Constructors ===================

	Mat4()//Default identity matrix constuctor
	{
		for (int i = 0; i < 16; i++)
		{
			this->data[i] = (i % 5) ? 0 : 1;
		}
	}

	Mat4(const T *n)//All one value
	{
		std::memcpy(this->data, n, 16*sizeof(T));
	}

	Mat4(const Mat4<T> & input)//Copy constructor
	{
		std::memcpy(this->data, input.data, 16*sizeof(T));
	}

	template<class otherT>
	Mat4(const Mat4<otherT> & input)//Copy constructor from Mat4 with diff type
	{
		for (int i = 0; i < 16; i++)
		{
		
			this->data[i] = static_cast<T>(input.data[i]);
		}
	}


	// ============= Assignment operators ===========================

	Mat4<T>& operator=(const Mat4<T> &other)//assignment operator
	{
		std::memcpy(this->data, other.data, 16 * sizeof(T));
		return *this;
	}

	template<class otherT>
	Mat4<T>& operator=(const Mat4<otherT> &other)//assignment operator for diffferent type Mat4
	{
		std::memcpy(this->data, other.data, 16 * sizeof(T));
		return *this;
	}

	// ============= Access  operators ===================

	//	[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]
	//			  ||
	//			 \  /
	//		      \/
	//		[ 0  4  8  12 ] 
	//		[ 1  5  9  13 ] 
	//		[ 2  6  10 14 ]
	//		[ 3  7  11 15 ]

	T& at(int row, int col)
	{
		return this->data[row + col*4];
	}

	const T& at(int row, int col) const
	{
		return this->data[row + col * 4];
	}

	T& operator()(int row, int col)
	{
		return this->data[row + col * 4];
	}

	const T& operator()(int row, int col) const
	{
		return this->data[row + col * 4];
	}

	//-----------Scale set/get-------------
	void setScale(T sx, T sy, T sz)
	{
		at(0, 0) = sx;
		at(1, 1) = sy;
		at(2, 2) = sz;
	}

	void setScale(T s)
	{
		at(0, 0) = s;
		at(1, 1) = s;
		at(2, 2) = s;
	}

	void setScale(const Vec3<T> &vec)
	{
		at(0, 0) = vec.x;
		at(1, 1) = vec.y;
		at(2, 2) = vec.z;
	}

	Vec3<T> getScale() const
	{
		return Vec3<T>(at(0.0), at(1,1), at(2,2));
	}

	//-----------Translation set/get-------------
	void setTranslation(T tx, T ty, T tz)
	{
		at(3, 0) = tx;
		at(3, 1) = ty;
		at(3, 2) = tz;
		at(3, 3) = 1;
	}

	void setTranslation(const Vec3<T> &vec)
	{
		at(3, 0) = vec.x;
		at(3, 1) = vec.y;
		at(3, 2) = vec.z;
		at(3, 3) = 1;
	}

	Vec3<T> getTranslation() const
	{
		return Vec3<T>(at(0.0), at(1, 1), at(2, 2));
	}



	// ============= Comparison  operators ===================
	bool operator==(const Mat4<T> &other) const
	{
		for (int i = 0; i < 16; i++)
		{
			if(std::fabs(this->data[i]-other.data[i]) >= FLT_EPSILON){
				return false;
			}

		}
		return true;
	}

	bool operator!=(const Mat4<T> &other) const
	{
		return !(*this == other);
	}

	

	// ============== Static functions (create specific matrices) =====================
	
	//Lookat
	
	
	//Viewport transform
	static Mat4<T> createViewportTransform(T vwidth, T vheight)
	{
		/*
		viewport width = vr - vl 
		viewport height = vt - vb
		
		z scaling doesnt use nearVal/farVal. Assumes its from -1 to 1 in NDC and scales it to 0 to 1 in viewport

		width and height arguments are given in pixels. -1 is subtracted to account for arrays starting at 0
		e.g. 1280px width -> vr(1280) - vl(1)  = 1279
		  
			  vr - vl                                     vr + vl
			------------       0               0         ----------
				 2                                           2

							 vt - vb                      vt + vb
				 0         ------------        0         -----------     
								2                            2

				 0              0              1/2          1/2


				 0              0              0             1

													
		  
		 */
		//info: http://glasnost.itcarlow.ie/~powerk/GeneralGraphicsNotes/projection/viewport_transformation.html

		Mat4<T> vp_trans;

		//scale
		vp_trans.at(0, 0) = (vwidth - 1) / 2;
		vp_trans.at(1, 1) = (vheight - 1) / 2;
		vp_trans.at(2, 2) = 1/2;

		//translate
		vp_trans.at(0, 3) = (vwidth ) / 2;
		vp_trans.at(1, 3) = (vheight) / 2;
		vp_trans.at(2, 3) = 1 / 2;
		


		return vp_trans;

	}


	//Orthographic
	//Fustrum


	//================ Scalar/Matrix addition/subtraction  =============================

	Mat4<T> operator+(T &other) const
	{
		Mat4<T> result;
		for (int i = 0; i < 16; i++)
		{
			result.data[i] = this->data[i] + other;
		}
		return result;
	}

	Mat4<T> operator-(T &other) const
	{
		Mat4<T> result;
		for (int i = 0; i < 16; i++)
		{
			result.data[i] = this->data[i] - other;
		}
		return result;
	}

	Mat4<T> operator*(T &other) const
	{
		Mat4<T> result;
		for (int i = 0; i < 16; i++)
		{
			result.data[i] = this->data[i] * other;
		}
		return result;
	}

	Mat4<T> operator/(T &other) const
	{
		Mat4<T> result;
		for (int i = 0; i < 16; i++)
		{
			result.data[i] = this->data[i] / other;
		}
		return result;
	}

	//================ Matrix/Matrix addition/subtraction  =============================
	Mat4<T> operator+(const Mat4<T> &other) const
	{
		Mat4<T> result;
		for (int i = 0; i < 16; i++)
		{
			result.data[i] = this->data[i] + other[i];
		}
		return result;
	}

	Mat4<T> operator-(const Mat4<T> &other) const
	{
		Mat4<T> result;
		for (int i = 0; i < 16; i++)
		{
			result.data[i] = this->data[i] - other[i];
		}
		return result;
	}


	//================ Matrix multiplications  =========================================
	//Vec3 have hidden w
	//
	//		[0  4  8  12 ] [ x ]     [ (0x  + 4y  + 8z  + 12w) ]
	//		[1  5  9  13 ] [ y ] =>  [ (1x  + 5y  + 9z  + 13w) ]
	//		[2  6  10 14 ] [ z ]	 [ (2x  + 6y  + 10z + 14w) ]
	//		[3  7  11 15 ] [ w ]	 [ (3x  + 7y  + 11z + 15w) ]
	//
	Vec3<T> operator*(const Vec3<T> &other) const
	{
		//data[] here is this->data but too long to type
		Vec3<T> result(data[0] * other.x + data[4] * other.y + data[8]  * other.z + data[12] * other.w,
					   data[1] * other.x + data[5] * other.y + data[9]  * other.z + data[13] * other.w,
					   data[2] * other.x + data[6] * other.y + data[10] * other.z + data[14] * other.w,
					   data[3] * other.x + data[7] * other.y + data[11] * other.z + data[15] * other.w);

		return result;
	}

	Mat4<T> operator*(const Mat4<T> &other) const
	{
		Mat4<T> result;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				T num = 0;
				for (int k = 0; k < 4; k++)
				{
					num += this->at(i, k) * other.at(k,j);
				}
				result.at(i,j) = num;
			}
		}
		return result;
	}

};

//Common type definitions
typedef class Mat4<int> Mat4i;
typedef class Mat4<float> Mat4f;

#endif
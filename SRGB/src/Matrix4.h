#pragma once

#ifndef _USE_MATH_DEFINES
	#define _USE_MATH_DEFINES
#endif
#include <math.h>

#include <iostream>
#include <type_traits>
#include <cassert>
#include "Vector3.h"



//DegToRad


template<typename T>
class Mat4 {
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
		std::memcpy(this->data, n, 16 * sizeof(T));
	}

	Mat4(const Mat4<T> & input)//Copy constructor
	{
		std::memcpy(this->data, input.data, 16 * sizeof(T));
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
		return this->data[row + col * 4];
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
		return Vec3<T>(at(0.0), at(1, 1), at(2, 2));
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
			if (std::fabs(this->data[i] - other.data[i]) >= FLT_EPSILON)
			{
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

	//Rotation around axis Vec3<T> x y z => x,y and z axis rotation in deg
	static Mat4<T> createRotAxis(const Vec3<T> &rot)
	{
		T x_rads = DegToRad(rot.x);
		T y_rads = DegToRad(rot.y);
		T z_rads = DegToRad(rot.z);

		Mat4<T> matX, matY, matZ;
		float cosX = cos(x_rads);
		float sinX = sin(x_rads);
		float cosY = cos(y_rads);
		float sinY = sin(y_rads);
		float cosZ = cos(z_rads);
		float sinZ = sin(z_rads);

		matX(1, 1) = cosX;
		matX(2, 1) = sinX;
		matX(1, 2) = -sinX;
		matX(2, 2) = cosX;

		matY(0, 0) = cosY;
		matY(2, 0) = -sinY;
		matY(0, 2) = sinY;
		matY(2, 2) = cosY;

		matZ(0, 0) = cosZ;
		matZ(1, 0) = sinZ;
		matZ(0, 1) = -sinZ;
		matZ(1, 1) = cosZ;

		/*std::cout << "RotVec = " << a << "," << b << "," << c << std::endl;
		 std::cout << "Rx = " << std::endl << ma;
		 std::cout << "Ry = " << std::endl << mb;
		 std::cout << "Rz = " << std::endl << mc;*/

		Mat4<T> rotmat = matX * matY * matZ;
		//std::cout << "Result = " << std::endl << ma * (mb * mc);

		return rotmat;
	}


	//Scaling
	static Mat4<T> createScale(const Vec3<T> &scale_param)
	{
		/*
			Scaled matrix


				         
		       scale_param.x        0                0           0

	
					 0         scale_param.y         0           0    
								                                 

					 0              0          scale_param.z     0


					 0              0                0           1



			 */
		Mat4<T> scaled_mat;

		scaled_mat(0, 0) = scale_param.x;
		scaled_mat(1, 1) = scale_param.y;
		scaled_mat(2, 2) = scale_param.z;
	
		return scaled_mat;
	}

	//Translate
	static Mat4<T> createTranslation(const Vec3<T> &trans_param)
	{
		/*
			Scaled matrix



			         1			    0              0        trans_param.x


					 0              1              0		trans_param.y


					 0              0              1		trans_param.z


					 0              0              0            1



			 */
		Mat4<T> trans_mat;

		trans_mat(0, 3) = trans_param.x;
		trans_mat(1, 3) = trans_param.y;
		trans_mat(2, 3) = trans_param.z;
		
		return trans_mat;
	}

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
		vp_trans.at(1, 1) = -(vheight - 1) / 2;
		vp_trans.at(2, 2) = 0.5;

		//translate
		vp_trans.at(0, 3) = (vwidth) / 2;
		vp_trans.at(1, 3) = (vheight) / 2;
		vp_trans.at(2, 3) = 0.5;



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
		Vec3<T> result(data[0] * other.x + data[4] * other.y + data[8] * other.z + data[12] * other.w,
			data[1] * other.x + data[5] * other.y + data[9] * other.z + data[13] * other.w,
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
					num += this->at(i, k) * other.at(k, j);
				}
				result.at(i, j) = num;
			}
		}
		return result;
	}


private:

	static T DegToRad(const T &num)
	{
		return num * (M_PI / 180);
	}
};

//Common type definitions
typedef class Mat4<int> Mat4i;
typedef class Mat4<float> Mat4f;

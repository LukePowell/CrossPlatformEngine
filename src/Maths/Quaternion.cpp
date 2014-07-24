#include "Quaternion.h"

/*!*************************************************************************************************
*\brief Constructor for quaternions
***************************************************************************************************/
Maths::Quaternion::Quaternion(float x /* = 0 */, float y /* = 0 */, float z /* = 0 */, float w /* = 0 */) : x(x), y(y), z(z), w(w)
{
}

/*!*************************************************************************************************
*\brief Converts a quaternion to a matrix to use in rotations
*\return Matrix representation of quaternion input.
***************************************************************************************************/
Matrix Maths::Quaternion::QuaternionToMatrix()
{
	Matrix Output;
	
	//////////////////////////////////////////////
	//1-2y^2-2z^2 2xy-2wz     2zx+2wy     0     //
	//2xy+2wz     1-2x^2-2z^2 2yz-2wx     0     //
	//2zx-2wy     2yz+2wx     1-2x^2-2y^2 0     //
	//0           0           0           1     //
	//////////////////////////////////////////////
	
	//Row one
	Output[0][0] = (1-2*(y*y))-2*z*z;
	Output[0][1] = 2*x*y-2*w*z;
	Output[0][2] = 2*z*x+2*w*y;
	//Row two
	Output[1][0] = 2*x*y+2*w*z;
	Output[1][1] = 1-2*x*x-2*z*z;
	Output[1][2] = 2*y*z-2*w*x;
	//Row three
	Output[2][0] = 2*z*x-2*w*y;
	Output[2][1] = 2*y*z+2*w*x;
	Output[2][2] = 1-2*x*x-2*y*y;
	//Row four
	Output[3][3] = 1;
	
	return Output;
}

Maths::Quaternion Maths::Quaternion::operator*( const Quaternion &rhs ) const
{
	Quaternion Temp(w*rhs.x+x*rhs.w+y*rhs.z-z*rhs.y,
					w*rhs.y+y*rhs.w+z*rhs.x-x*rhs.z,
					w*rhs.z+z*rhs.w+x*rhs.y-y*rhs.x,
					w*rhs.w-x*rhs.x-y*rhs.y-z*rhs.z);
	return Temp;
}

Maths::Quaternion Maths::operator*( float s, Quaternion rhs )
{
	return Quaternion(rhs.x * s, rhs.y * s, rhs.z * s, rhs.w * s);
}

Maths::Quaternion & Maths::Quaternion::operator*=( const Quaternion &rhs )
{
	Quaternion Temp = *this * rhs;

	for(int i = 0; i < 4; ++i)
		v[i] = Temp.v[i];

	return *this;
}

Maths::Quaternion & Maths::Quaternion::operator*=( float s )
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

Maths::Quaternion & Maths::Quaternion::operator+=( const Quaternion &rhs )
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
	return *this;
}

void Maths::Quaternion::Normalize()
{
	float Magnitude_ = Magnitude();
	x /= Magnitude_;
	y /= Magnitude_;
	z /= Magnitude_;
	w /= Magnitude_;
}

float Maths::Quaternion::Magnitude()
{
	return std::sqrt(x*x+y*y+z*z+w*w);
}

/*!****************************************************************************
*\author Luke Powell
*\brief Implemenation of Vector class definition in MathLib.h
******************************************************************************/
#include "MathLib.h"
#include <cmath>//sqrt,tan and other similar functions

/*!*************************************************************************************************
*\brief Constructor for Vec4 class, w is set to 0.
***************************************************************************************************/
Vec4::Vec4(float x /* = 0 */, float y /* = 0 */, float z /* = 0 */, float w /* = 0.0f */) : x(x), y(y), z(z), w(w)
{
}
Vec4::Vec4(void) : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}
Vec4::Vec4(const Vec4& rhs) : x(rhs.x), y(rhs.y) , z(rhs.z), w(rhs.w)
{
}

/*!*************************************************************************************************
*\brief Adds to vectors together.
*\return Summed Vec4
***************************************************************************************************/
Vec4 Vec4::operator+(const Vec4 &rhs)
{
	return Vec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

/*!*************************************************************************************************
*\brief Scalar multiplication
*\param scalar 
*       Scalar value to multiple Vec4 by.
*\return Vec4 multiplied by scalar
****************************************************************************************************/
Vec4 Vec4::operator*(float scalar)
{
	return Vec4(scalar * x, scalar * y, scalar * z,scalar*w);
}

/*!*************************************************************************************************
*\brief Negation operator 
*\return Negated Vec4
***************************************************************************************************/
Vec4 Vec4::operator-()
{
	return Vec4(-x,-y,-z,-w);
}

/*!****************************************************************************
*\brief Cross product
*\param rhs
*       Right hand argument to A X B
*\return Vec4 containing the cross product.
*******************************************************************************/
Vec4 Vec4::operator*(const Vec4 &rhs) const
{
	Vec4 Output;
	Output.x = (y * rhs.z) - (rhs.y * z);
	Output.y = (rhs.x * z) - (x * rhs.z);
	Output.z = (x * rhs.y) - (rhs.x * y);
	return Output;
}

/*!****************************************************************************
*\brief Side effect addition operator
*\param rhs
*       Vector to add to this
*\return reference to this
*******************************************************************************/
Vec4 &Vec4::operator+=(const Vec4& rhs)
{
	return *this = *this + rhs;
}

/*!***************************************************************************
*\brief Subtract the right hand side argument.
*\param rhs
*       Vector to subtract
*\return Vector containing the reult of this - rhs
******************************************************************************/
Vec4 Vec4::operator-(const Vec4 &rhs)
{
	return Vec4(x-rhs.x,y-rhs.y,z-rhs.z,w-rhs.w);
}

/*!****************************************************************************
*\brief Subtracts rhs from this setting this to the result
*\param rhs
*       Vector to subtract
*\return this after modification
*******************************************************************************/
Vec4 &Vec4::operator-=(const Vec4 &rhs)
{
	return *this = *this - rhs;
}

/*!****************************************************************************
*\brief Allows for # * Vec as well as Vec * #
*\param scalar
*       Scalar value to multiple by.
*\param rhs
*       Vector to apply scalar to.
*\return Resultant vector of scalar multiplication.
******************************************************************************/
Vec4 operator*(float scalar, Vec4 rhs)
{
	return Vec4(scalar * rhs.x, scalar * rhs.y, scalar * rhs.z);
}

/*!****************************************************************************
*\brief multiplies this by the scalar value.
*\param scalar
*       Value to scalar multiply this by
*\return this modified by scalar multiplication
******************************************************************************/
Vec4 &Vec4::operator*=(float scalar)
{
	return *this = *this * scalar;
}

/*!****************************************************************************
*\brief Compares two vectors for equality
*\param rhs Vector to compare to this
*\return true if equal else false
*******************************************************************************/
bool Vec4::operator==(const Vec4 &rhs) const
{
	return (EqualF(x,rhs.x) && EqualF(y,rhs.y) && EqualF(z,rhs.z) && EqualF(w,rhs.w));
}

/*!****************************************************************************
*\brief Compares two vectors for inequality
*\param rhs
*       Right hand vector
*\return true if not equal else false.
*******************************************************************************/
bool Vec4::operator!=(const Vec4 &rhs) const
{
	return !(*this == rhs);
}

void Vec4::Zero()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

float Vec4::Length()
{
	return std::sqrt(x*x+y*y+z*z+w*w);
}

float Vec4::LengthSq() const
{
	return x*x+y*y+z*z+w*w;
}

void Vec4::Normalize()
{
	float length = Length();
	if(EqualF(length,0.0f))
		return;
	
	*this /= length;
}

float Vec4::Dot(const Vec4 &rhs)
{
	return (x * rhs.x) + (y * rhs.y) + (z *rhs.z) + (w * rhs.w);
}

Vec4 Vec4::Cross(const Vec4 &rhs) const
{
	return (*this * rhs);
}

Vec4 Vec4::operator/(float scalar)
{
	return Vec4(x / scalar, y / scalar, z / scalar, w / scalar);
}

Vec4 Vec4::operator/=(float scalar)
{
	return *this = *this / scalar;
}

void Vec4::Print()
{
  printf("%5.3f, %5.3f, %5.3f, %5.3f\n",x,y,z,w);
}
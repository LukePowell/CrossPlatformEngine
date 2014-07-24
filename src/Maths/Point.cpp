#include "MathLib.h"
/*!****************************************************************************
*\brief Constructs a point sets w to 1.0f
*\param x
*       Value for x
*\param y 
*       Value for y
*\param z
*       Value for z
******************************************************************************/
Point::Point(float x /* = 0.0f */, float y /* = 0.0f */, float z /* = 0.0f */,float w /* 1.0f */ ) : x(x), y(y), z(z), w(w)
{
}
Point::Point(void) : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{
}
Point::Point(const Point &rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
{
}

Point &Point::operator=(const Point &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
	return *this;
}

Point Point::operator-(void) const
{
	return Point(-x,-y,-z,-w);
}

/*!****************************************************************************
*\brief Subtracts two points returning the resultant vector
*\param rhs
*       right hand vector to subtract
*\return Vector resulting from subtraction
******************************************************************************/
Vec4 Point::operator-(const Point &rhs) const
{
	return Vec4(x-rhs.x,y-rhs.y,z-rhs.z,w-rhs.w);
}

Point Point::operator+(const Vec4 &rhs) const
{
	return Point(x+rhs.x,y+rhs.y,z+rhs.z,w+rhs.w);
}

Point &Point::operator+=(const Vec4 &rhs)
{
	return *this = *this + rhs;
}

Point Point::operator-(const Vec4 &rhs) const
{
	return Point(x-rhs.x,y-rhs.y,z-rhs.z,w-rhs.w);
}

Point &Point::operator-=(const Vec4 &rhs)
{
	return *this = *this - rhs;
}

/*!****************************************************************************
*\brief Compares two points for equality
*\param rhs 
*        Point to compare to this
*\return true if equal else false
*******************************************************************************/
bool Point::operator==(const Point &rhs) const
{
	return (EqualF(x,rhs.x) && EqualF(y,rhs.y) && EqualF(z,rhs.z) && EqualF(w,rhs.w));
}

/*!****************************************************************************
*\brief Compares two points for inequality
*\param rhs
*       Right hand point
*\return true if not equal else false.
*******************************************************************************/
bool Point::operator!=(const Point &rhs) const
{
	return !(*this == rhs);
}

void Point::Zero()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

void Point::Print(void) const
{
  printf("%5.3f, %5.3f, %5.3f, %5.3f\n",x,y,z,w);
}

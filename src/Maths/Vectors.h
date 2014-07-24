#pragma  once
#include "Definitions.h"
#include <iostream>

struct Point;

struct  Vec4
{
	//Constructors
	Vec4(void);
	Vec4(float x, float y, float z, float w = 0.0f);
	Vec4(const Vec4 &rhs);

	//Adding Vectors
	Vec4 operator+(const Vec4 &rhs);
	Vec4 &operator +=(const Vec4 &rhs);

	//Subtraction
	Vec4 operator-(const Vec4 &rhs);
	Vec4 &operator-=(const Vec4 &rhs);
	Vec4 SubNoW(Vec4 &rhs);

	//Scalar multiplication
	Vec4 operator*(float scalar);
	friend Vec4 operator*(float scalar, Vec4 rhs);
	Vec4 operator/(float scalar);
	Vec4 operator/=(float scalar);
	Vec4 &operator*=(float scalar);

	//Operations with points
	Point operator+(const Point &rhs);

	//Negation
	Vec4 operator-();

	//Cross product
	Vec4 operator*(const Vec4 &rhs) const;

	//Products
	float Dot(const Vec4 &rhs);
	float DotNoW(const Vec4 &rhs);

	Vec4 Cross(const Vec4 &rhs) const;
	Vec4 Component(Vec4 &rhs);

	//Output
	friend   std::ostream &operator<<(std::ostream &stream, Vec4 &rhs);
	void Print();

	//Comparison
	bool operator==(const Vec4 &rhs) const;
	bool operator!=(const Vec4 &rhs) const;
	bool operator<(Vec4 rhs) const;

	//Magnitude
	float Length();
	float LengthSq() const;

	//Modifying functions
	void Normalize();
	void Zero();

	//Data
	union
	{
		float v[4];
		struct
		{
			float x,y,z,w;
		};
	};

	float &operator[](unsigned int index){return v[index];}
	bool Inside;
};

struct   Point
{
	//Will set w to 1.0f
	Point(void);
	Point(float x, float y , float z, float w = 1.0f);
	Point(const Point &rhs);

	//Assignment
	Point &operator=(const Point &rhs);

	//Negation
	Point operator-(void) const;

	//Point subtraction results in vector
	Vec4 operator-(const Point &rhs) const;

	//Point +/- Vector = Vector
	Point operator+(const Vec4 &rhs) const;
	Point &operator+=(const Vec4 &rhs);
	Point operator-(const Vec4 &rhs) const;
	Point &operator-=(const Vec4 &rhs);

	//Comparison
	bool operator==(const Point &rhs) const;
	bool operator!=(const Point &rhs) const;

	//Modifications
	void Zero(void);

	//Output
	friend   std::ostream &operator<<(std::ostream &stream, Point &rhs);
	void Print(void) const;

	float x,y,z,w;
};
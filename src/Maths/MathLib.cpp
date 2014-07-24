/*!****************************************************************************
*\author Luke Powell
*\brief Implementation of a 3D linear algebra library for use in graphics and
*       physics.
*******************************************************************************/

#include "MathLib.h"
#include <cmath>

/*!****************************************************************************
*\brief utilizes epsilon to compare two floats accounts for inaccuracy in float
*\param one 
*       one of the floats
*\param two
*       one of the floats
*\return true if the floats are "equal"
********************************************************************************/
bool EqualF(float one, float two)
{
	float Diff = one - two;

	if(Epsilon>Diff && Diff>-Epsilon)
		return true;

	return false;
}

float DegreesToRadians( float degrees )
{
	return degrees * _PI_/180.0f;
}

Matrix BuildNaiveProjection( float focal )
{
	Matrix Output = IdentityMatrix();
	Output[0][0] = focal;
	Output[1][1] = focal;
	Output[2][2] = focal;
	Output[3][3] = 0;
	Output[3][2] = -1;
	return Output;
}

Matrix BuildCamera( Vec4 up, Vec4 view, Vec4 position )
{
    Vec4 f = view - position;
    f.Normalize();
    up.Normalize();
    Vec4 s = f.Cross(up);
    
    Matrix out;
    out.m[0][0] = s[0];
    out.m[0][1] = s[1];
    out.m[0][2] = s[2];
    
    out.m[1][0] = up[0];
    out.m[1][1] = up[1];
    out.m[1][2] = up[2];
    
    out.m[2][0] = -f[0];
    out.m[2][1] = -f[1];
    out.m[2][2] = -f[2];
    
    out.m[0][3] = -position.x;
    out.m[1][3] = -position.y;
    out.m[2][3] = -position.z;
    
    return out;
    
	/*//Altered throughout for each rotation matrix
	float Cosine = 0;
	float Sine = 0;

	//View prime, project x,z plane
	Vec4 ViewPrime = View;
	ViewPrime.y = 0;

	//Translation about -Camera position
	Matrix Translation = BuildTranslationMatrix(-position);

	//Rotation about Y
	Cosine = -ViewPrime.z / ViewPrime.Length();
	Sine = ViewPrime.x / ViewPrime.Length();
	Matrix RotationY = BuildYRotationMatrix(Cosine,Sine);

	//Rotation about X 
	Cosine = ViewPrime.Length()/View.Length();
	Sine = -View.y / View.Length();
	Matrix RotationX = BuildXRotationMatrix(Cosine,Sine);

	//Calculate UP prime
	Vec4 UpPrime = RotationX * RotationY * Up;

	//Rotation about Z
	Cosine = UpPrime.y / UpPrime.Length();
	Sine = UpPrime.x / UpPrime.Length();
	Matrix RotationZ = BuildZRotationMatrix(Cosine,Sine);

	//std::cout << RotationZ << RotationX << RotationY << Translation;
	return RotationZ * RotationX * RotationY * Translation;*/
}

Matrix BuildCamera( Vec4 up, Vec4 view, Vec4 right, Vec4 position)
{
	Matrix Output;
	Output = BuildFromVecT(right,up,-view) * BuildTranslationMatrix(-position);
	return Output;
}

Matrix BuildTranslationMatrix( Vec4 trans )
{
	return BuildTranslationMatrix(trans.x,trans.y,trans.z);
}

Matrix BuildXRotationMatrix( float cosine, float sine )
{
	Matrix Output = IdentityMatrix();
	Output[1][1] = cosine;
	Output[1][2] = -sine;
	Output[2][1] = sine;
	Output[2][2] = cosine;
	return Output;
}

Matrix BuildYRotationMatrix( float cosine, float sine )
{
	Matrix Output = IdentityMatrix();
	Output[0][0] = cosine;
	Output[0][2] = sine;
	Output[2][0] = -sine;
	Output[2][2] = cosine;
	return Output;
}

Matrix BuildZRotationMatrix( float cosine, float sine )
{
	Matrix Output = IdentityMatrix();
	Output[0][0] = cosine;
	Output[0][1] = -sine;
	Output[1][0] = sine;
	Output[1][1] = cosine;
	return Output;
}

Matrix BuildProjectionMatrix( float height, float width, float Far, float Near, float focal )
{
	Matrix Output;
	
	//Bottom row [0,0,-1,0]
	Output[3][2] = -1.0f;

	//row1
	Output[0][0] = (2 * focal) / width;
	
	//row2
	Output[1][1] = (2 * focal)  / height;

	//row3
	Output[2][2] = (Far + Near) / (-Far + Near);
	Output[2][3] = (-2 * Near * Far) / (Far - Near);

	//std::cout << "Perspective\n" << Output;
	return Output;
}

Matrix BuildRotationAboutAxis( Vec4 &axis, float degrees )
{
	Matrix Output = IdentityMatrix();
	//Use a normalized vector
	Vec4 Temp = axis;
	Temp.Normalize();

	Output = cos(DegreesToRadians(degrees)) * IdentityMatrix() + (1.0f - cos(DegreesToRadians(degrees))) * IdentityMatrix() * TensorProduct(Temp, Temp)+ ((sin(DegreesToRadians(degrees)) * IdentityMatrix()) * Cross(Temp));
	Output[3][3] = 1.0f;
	return Output;
}

Matrix BuildFromVecT( Vec4 right, Vec4 up, Vec4 view )
{
	Matrix Output = IdentityMatrix();
	//Row one
	Output[0][0] = right.x;
	Output[0][1] = right.y;
	Output[0][2] = right.z;
	//Row two
	Output[1][0] = up.x;
	Output[1][1] = up.y;
	Output[1][2] = up.z;
	//Row three
	Output[2][0] = view.x;
	Output[2][1] = view.y;
	Output[2][2] = view.z;
	return Output;
}

Matrix BuildFromVec( Vec4 right, Vec4 up, Vec4 view )
{
	Matrix Output = IdentityMatrix();
	//Row one
	Output[0][0] = right.x;
	Output[0][1] = up.x;
	Output[0][2] = view.x;
	//Row two
	Output[1][0] = right.y;
	Output[1][1] = up.y;
	Output[1][2] = view.y;
	//Row three 
	Output[2][0] = right.z;
	Output[2][1] = up.z;
	Output[2][2] = view.z;
	return Output;
}

Matrix TensorProduct( Vec4 one, Vec4 two )
{
	Matrix Output = IdentityMatrix();

	for(int i = 0; i < 4; ++i)//one
		for(int j =0; j < 4; ++j)//two
		{
			Output[i][j] = one.v[i] * two.v[j];
		}

	return Output;
}

/*!****************************************************************************
*\brief Outputs a vector to the input stream
*\param stream
*       Stream to output vector to
*\param rhs
*       Vector to output
*\return stream that was passed in
*******************************************************************************/
std::ostream &operator<<(std::ostream &stream, Vec4 &rhs)
{
	stream << "Vec4: [ " << rhs.x << " , " << rhs.y << " , " << rhs.z <<  " , " << rhs.w << " ]" << std::endl;
	return stream;
}

/*!****************************************************************************
*\brief Outputs a matrix to the input stream
*\param stream
*       Stream to output matrix to
*\param rhs
*       Matrix to to output
*\return stream that was passed in
*******************************************************************************/
std::ostream &operator<<(std::ostream &stream, Matrix &rhs)
{
	stream << "Matrix:\n----------\n";
	stream << " " << rhs[0][0] << " " << rhs[0][1] << " " << rhs[0][2] << " " << rhs[0][3] << std::endl;
	stream << " " << rhs[1][0] << " " << rhs[1][1] << " " << rhs[1][2] << " " << rhs[1][3] << std::endl;
	stream << " " << rhs[2][0] << " " << rhs[2][1] << " " << rhs[2][2] << " " << rhs[2][3] << std::endl;
	stream << " " << rhs[3][0] << " " << rhs[3][1] << " " << rhs[3][2] << " " << rhs[3][3] << std::endl;
	stream << "----------\n";
	return stream;
}

/*!****************************************************************************
*\brief Outputs a point to the input stream
*\param stream
*       Stream to output point to
*\param rhs
*       point to to output
*\return stream that was passed in
*******************************************************************************/
std::ostream &operator<<(std::ostream &stream, Point &rhs)
{
	return stream << "Point: [ " << rhs.x << " , " << rhs.y << " , " << rhs.z <<  " , " << rhs.w << " ]" << std::endl;
}

Matrix & Matrix::operator*=( const Matrix &rhs )
{
	return *this = *this * rhs;
}

Matrix & Matrix::operator*=( const float rhs )
{
	for(int y = 0; y < 4; ++y)
		for(int x = 0; x < 4; ++x)
			m[y][x] *= rhs;
	return *this;
}

Matrix Matrix::operator+( const Matrix &rhs ) const
{
	Matrix Output;

	for(int y = 0; y < 4; ++y)
		for(int x = 0; x < 4; ++x)
			Output[y][x] = m[y][x] + rhs.m[y][x];

	return Output;
}

Matrix & Matrix::operator+=( const Matrix &rhs )
{
	return *this = *this + rhs;
}

Matrix Matrix::operator-( const Matrix &rhs ) const
{
	Matrix Output;

	for(int y = 0; y < 4; ++y)
		for(int x = 0; x < 4; ++x)
			Output[y][x] = m[y][x] * rhs.m[y][x];

	return Output;
}

Matrix & Matrix::operator-=( const Matrix &rhs )
{
	return *this = *this - rhs;
}

Matrix Matrix::operator*( const float rhs ) const
{
	Matrix Output;
	for(int y = 0; y < 4; ++y)
		for(int x = 0; x < 4; ++x)
			Output[y][x] = m[y][x] * rhs;
	return Output;
}

Matrix operator*( const float rhs, const Matrix &lhs )
{
	return lhs * rhs;
}

Matrix Cross( Vec4 vector )
{
	Matrix Output;
	Output[0][1] = -vector.z;
	Output[0][2] = vector.y;
	Output[1][0] = vector.z;
	Output[1][2] =  -vector.x;
	Output[2][0] = -vector.y;
	Output[2][1] = vector.x;
	return  Output;
}

float FloatAdd( float one, float two )
{
	float Ret = one + two;
	if(Ret < Epsilon && Ret > -Epsilon)
		Ret = 0.0f;
	return Ret;
}

Matrix Matrix::operator/( const float rhs ) const
{
	Matrix Output;

	for(int y = 0; y < 4; ++y)
		for(int x = 0; x < 4; ++x)
			Output[y][x] = m[x][y] / rhs;

	return Output;
}

Matrix & Matrix::operator/=( const float rhs )
{
	return *this = *this / rhs;
}

void Matrix::Zero( void )
{
	for(int y = 0; y < 4; ++y)
		for(int x = 0; x < 4; ++x)
			m[y][x] = 0.0f;
}

void Matrix::Identity( void )
{
	for(int y = 0; y < 4; ++y)
		for(int x =0; x < 4; ++x)
		{
			if(x == y)
				m[y][x] = 1.0f;
			else
				m[y][x] = 0.0f;
		}
}

Vec4 Matrix::GetRow( int row )
{
	return Vec4(m[row][0], m[row][1], m[row][2], m[row][3]);
}

float Vec4::DotNoW(const Vec4 &rhs )
{
	return (x * rhs.x) + (y * rhs.y) + (z *rhs.z);
}

Vec4 Vec4::SubNoW( Vec4 &rhs )
{
	Vec4 Out = *this - rhs;
	Out.w = 0.0f;
	return Out;
}

bool Vec4::operator<(const Vec4 rhs ) const
{
	return LengthSq() < rhs.LengthSq();
}

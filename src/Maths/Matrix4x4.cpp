#include "MathLib.h"
#include <cmath>

/*!*************************************************************************************************
*\brief Default constructor sets everything to 0
****************************************************************************************************/
Matrix::Matrix(void) 	
{
	for(int y = 0; y < 4; ++y)
		for(int x = 0; x < 4; ++x)
			m[y][x] = 0;
}
/*!***********************************************************************************************
*\brief Constructs a matrix to the given value
*************************************************************************************************/
Matrix::Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
{
	//Row one
	m[0][0] = m11;
	m[0][1] = m12;
	m[0][2] = m13;
	m[0][3] = m14;
	//Row two
	m[1][0] = m21;
	m[1][1] = m22;
	m[1][2] = m23;
	m[1][3] = m24;
	//Row three
	m[2][0] = m31;
	m[2][1] = m32;
	m[2][2] = m33;
	m[2][3] = m34;
	//Row four
	m[3][0] = m41;
	m[3][1] = m42;
	m[3][2] = m43;
	m[3][3] = m44;
}

Matrix::Matrix(const Matrix& rhs)
{
	for(int y = 0; y < 4; ++y)
		for(int x = 0; x < 4; ++x)
			m[y][x] = rhs.m[y][x];
}

Matrix &Matrix::operator=(const Matrix &rhs)
{
	for(int y = 0; y < 4; ++y)
		for(int x = 0; x < 4; ++x)
			m[y][x] = rhs.m[y][x];
	return *this;
}




/*!****************************************************************************
*\brief used to dereference into Matrix
*\param index
*       Row to index will return pointer to index into columns
*\return pointer to the requested row, dereference the pointer
*        to reach a column
*******************************************************************************/
float *Matrix::operator[](int index)
{
	return m[index];
}

/*!*****************************************************************************
*\brief Multiplies the two matrices
*\param rhs
*       Matrix to right of *
*\return Multiplied matrix
*******************************************************************************/
Matrix Matrix::operator*(const Matrix &rhs) const
{
	//0 Matrix for output
	Matrix Output;

	for(int y = 0; y < 4; ++y)
		for(int x = 0; x < 4; ++x)
			Output.m[y][x] = (m[y][0] * rhs.m[0][x]) + (m[y][1] * rhs.m[1][x]) + (m[y][2] * rhs.m[2][x]) + (m[y][3] * rhs.m[3][x]);

	return Output;
}

/*!****************************************************************************
*\brief Uses helper function MultByPtOrVec to mult rhs by this
*\param rhs
*       Vector to multiply.
*\return Resultant vec4
*******************************************************************************/
Vec4 Matrix::operator *(const Vec4 &rhs) const
{
	return MultByPtOrVec(rhs);
}

/*!****************************************************************************
*\brief Compares two matrices for equality
*\param rhs
*       Right hand matrix
*\return true if not equal else false.
*******************************************************************************/
bool Matrix::operator==(const Matrix &rhs) const
{
	for(int y = 0; y < 4; ++y)
		for(int x = 0; x < 4; ++x)
			if(!EqualF(m[y][x],rhs.m[y][x]))//If any value is not equal then the matrices are not equal
				return false;

	//If we get here all values are equal
	return true; 
}

/*!****************************************************************************
*\brief Compares two matrices for inequality
*\param rhs
*       Right hand matrix
*\return true if not equal else false.
*******************************************************************************/
bool Matrix::operator!=(const Matrix &rhs) const
{
	return !(*this==rhs);
}

/*!****************************************************************************
*\brief Uses helper function MultByPtOrVec to mult rhs by this
*\param rhs
*       Point to multiply
*\return resultant point
******************************************************************************/
Point Matrix::operator *(const Point &rhs) const
{
	return MultByPtOrVec(rhs);
}

float Matrix::Determinant()
{
	return m[0][0]*m[1][1]*m[2][2]*m[3][3]+m[0][0]*m[1][2]*m[2][3]*m[3][1]+m[0][0]*m[1][3]*m[2][1]*m[3][2]+
		   m[0][1]*m[1][0]*m[2][3]*m[3][2]+m[0][1]*m[1][2]*m[2][0]*m[3][3]+m[0][1]*m[1][3]*m[2][2]*m[3][0]+
		   m[0][2]*m[1][0]*m[2][1]*m[3][3]+m[0][2]*m[1][1]*m[2][3]*m[3][0]+m[0][2]*m[1][3]*m[2][0]*m[3][1]+
		   m[0][3]*m[1][0]*m[2][2]*m[3][1]+m[0][3]*m[1][1]*m[2][0]*m[3][2]+m[0][3]*m[1][2]*m[2][1]*m[3][0]-
		   m[0][0]*m[1][1]*m[2][3]*m[3][2]-m[0][0]*m[1][2]*m[2][1]*m[3][3]-m[0][0]*m[1][3]*m[2][2]*m[3][1]-
		   m[0][1]*m[1][0]*m[2][2]*m[3][3]-m[0][1]*m[1][2]*m[2][3]*m[3][0]-m[0][1]*m[1][3]*m[2][0]*m[3][2]-
		   m[0][3]*m[1][0]*m[2][1]*m[3][2]-m[0][3]*m[1][1]*m[2][2]*m[3][0]-m[0][3]*m[1][2]*m[2][0]*m[3][1];
}

Matrix Matrix::Inverse()
{
	float Det = Determinant();
	Matrix Inverse;
	if(EqualF(Det,0.0f))
		Inverse.Identity();//Maybe not the best option, recongizible and non-crashy however
	else
	{
	   Matrix Inverse;
	   Inverse.m00 = m12*m23*m31 - m13*m22*m31 + m13*m21*m32 - m11*m23*m32 - m12*m21*m33 + m11*m22*m33;
	   Inverse.m01 = m03*m22*m31 - m02*m23*m31 - m03*m21*m32 + m01*m23*m32 + m02*m21*m33 - m01*m22*m33;
	   Inverse.m02 = m02*m13*m31 - m03*m12*m31 + m03*m11*m32 - m01*m13*m32 - m02*m11*m33 + m01*m12*m33;
	   Inverse.m03 = m03*m12*m21 - m02*m13*m21 - m03*m11*m22 + m01*m13*m22 + m02*m11*m23 - m01*m12*m23;
	   Inverse.m10 = m13*m22*m30 - m12*m23*m30 - m13*m20*m32 + m10*m23*m32 + m12*m20*m33 - m10*m22*m33;
	   Inverse.m11 = m02*m23*m30 - m03*m22*m30 + m03*m20*m32 - m00*m23*m32 - m02*m20*m33 + m00*m22*m33;
	   Inverse.m12 = m03*m12*m30 - m02*m13*m30 - m03*m10*m32 + m00*m13*m32 + m02*m10*m33 - m00*m12*m33;
	   Inverse.m13 = m02*m13*m20 - m03*m12*m20 + m03*m10*m22 - m00*m13*m22 - m02*m10*m23 + m00*m12*m23;
	   Inverse.m20 = m11*m23*m30 - m13*m21*m30 + m13*m20*m31 - m10*m23*m31 - m11*m20*m33 + m10*m21*m33;
	   Inverse.m21 = m03*m21*m30 - m01*m23*m30 - m03*m20*m31 + m00*m23*m31 + m01*m20*m33 - m00*m21*m33;
	   Inverse.m22 = m01*m13*m30 - m03*m11*m30 + m03*m10*m31 - m00*m13*m31 - m01*m10*m33 + m00*m11*m33;
	   Inverse.m23 = m03*m11*m20 - m01*m13*m20 - m03*m10*m21 + m00*m13*m21 + m01*m10*m23 - m00*m11*m23;
	   Inverse.m30 = m12*m21*m30 - m11*m22*m30 - m12*m20*m31 + m10*m22*m31 + m11*m20*m32 - m10*m21*m32;
	   Inverse.m31 = m01*m22*m30 - m02*m21*m30 + m02*m20*m31 - m00*m22*m31 - m01*m20*m32 + m00*m21*m32;
	   Inverse.m32 = m02*m11*m30 - m01*m12*m30 - m02*m10*m31 + m00*m12*m31 + m01*m10*m32 - m00*m11*m32;
	   Inverse.m33 = m01*m12*m20 - m02*m11*m20 + m02*m10*m21 - m00*m12*m21 - m01*m10*m22 + m00*m11*m22;
	   Inverse  *= 1 / Det;
	   return Inverse;
	}

	return Inverse;
}
///////////////////////////////////////////////////////////////////////////////
//Functions to build matrices
///////////////////////////////////////////////////////////////////////////////

/*!***************************************************************************
*\brief  Returns identity matrix
*\return 4x4 Identity matrix
*****************************************************************************/
Matrix IdentityMatrix()
{
	Matrix Output;
	IdentityMatrix(Output);
	return Output;
}

/*!****************************************************************************
*\brief Changes an already existent 4x4 matrix to the 4x4 identity matrix
*\param changeMe
*       Matrix to change to identity matrix.
******************************************************************************/
void IdentityMatrix(Matrix &changeMe)
{
	for(int y = 0; y < 4; ++y)
		for(int x = 0; x < 4; ++x)
		{
			if(x == y)
				changeMe[y][x] = 1.0f;
			else
				changeMe[y][x] = 0.0f;
		}
}

Matrix Matrix::transpose()
{
    Matrix out;
    
    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            out[j][i] = m[i][j];
        }
    }
    
    return out;
}

Matrix BuildTranslationMatrix( float xTrans,float yTrans,float zTrans )
{
	Matrix Output = IdentityMatrix();
	Output[0][3] = xTrans;
	Output[1][3] = yTrans;
	Output[2][3] = zTrans;
	return Output;
}

Matrix BuildScaleMatrix(Vec4 scale)
{
    return BuildScaleMatrix(scale.x,scale.x,scale.z);
}

Matrix BuildScaleMatrix( float xScale, float yScale, float zScale )
{
	Matrix Output = IdentityMatrix();
	Output[0][0] = xScale;
	Output[1][1] = yScale;
	Output[2][2] = zScale;
	return Output;
}

Matrix BuildXRotationMatrix( float degrees )
{
	Matrix Output = IdentityMatrix();
	float Radians = DegreesToRadians(degrees);
	float Cosine = std::cos(Radians);
	float Sine = std::sin(Radians);

	Output[1][1] = Cosine;
	Output[1][2] = -Sine;
	Output[2][1] = Sine;
	Output[2][2] = Cosine;

	return Output;
}

Matrix BuildYRotationMatrix( float degrees )
{
	Matrix Output = IdentityMatrix();
	float Radians = DegreesToRadians(degrees);
	float Cosine = std::cos(Radians);
	float Sine = std::sin(Radians);

	Output[0][0] = Cosine;
	Output[0][2] = Sine;
	Output[2][0] = -Sine;
	Output[2][2] = Cosine;

	return Output;
}

Matrix BuildZRotationMatrix( float degrees)
{
	Matrix Output = IdentityMatrix();
	float Radians = DegreesToRadians(degrees);
	float Cosine = std::cos(Radians);
	float Sine = std::sin(Radians);

	return Output;
}

Matrix BuildProjectionMatrix(float fovy, float aspect, float zNear, float zFar)
{
    Matrix m;
    
    float halffovy = (fovy / 2) * (3.1415926535897932384626433832795 / 180);
    
    float f = std::cos(halffovy) / std::sin(halffovy);
    
    m[0][0] = f / aspect;
    m[1][1] = f;
    m[2][2] = (zFar + zNear) / (zNear - zFar);
    m[2][3] = -1;
    m[3][2] = (2.0f * zFar * zNear) / (zNear - zFar);
    
    return m;
}

///////////////////////////////////////////////////////////////////////////////
//End of functions to build matrices
///////////////////////////////////////////////////////////////////////////////

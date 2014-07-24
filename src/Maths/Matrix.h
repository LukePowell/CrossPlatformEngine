#pragma  once
#include "Vectors.h"

struct   Matrix
{
	//Constructors
	Matrix(float m11,float m12,float m13,float m14,float m21,float  m22,float m23,float m24,float m31,float m32,float m33,float m34,float m41,float m42,float m43,float m44);
	Matrix(void); 
	Matrix(const Matrix &rhs);

	//Assignment
	Matrix &operator=(const Matrix &rhs);

	//Dereference
	float *operator[](int index);

	//Not all implemented below remove me later
	//Matrix operations
	Matrix operator*(const Matrix &rhs) const;
	Matrix &operator*=(const Matrix &rhs);
	Matrix operator+(const Matrix &rhs) const;
	Matrix &operator+=(const Matrix &rhs);
	Matrix operator-(const Matrix &rhs) const;
	Matrix &operator-=(const Matrix &rhs);

	//Scale/Division
	Matrix operator*(const float rhs) const;
	Matrix &operator*=(const float rhs);
	Matrix operator/(const float rhs) const;
	Matrix &operator/=(const float rhs);
	friend Matrix operator*(const float rhs, const Matrix &lhs);

	//Vector and point operations
	Vec4 operator*(const Vec4 &rhs) const;
	Point operator*(const Point &rhs) const;

	//Output
	friend   std::ostream &operator<<(std::ostream &stream, Matrix &rhs);

	//Comparision
	bool operator==(const Matrix &rhs) const;
	bool operator!=(const Matrix &rhs) const;

	//Modification
	void Zero(void);
	void Identity(void);

	float Determinant();

    Matrix transpose();
	Matrix Inverse();
	//void Invert();
	//Data
	union
	{
		float m[4][4];
		struct
		{
			float m00,m01,m02,m03, 
				m10,m11,m12,m13,
				m20,m21,m22,m23,
				m30,m31,m32,m33;
		};
        float v[16];
	};

	Vec4 GetRow(int row);
private:
	template<typename T>
	T MultByPtOrVec(const T &rhs) const
	{
		float Temp[4] = {0.0f,0.0f,0.0f,0.0f};

		for(int i = 0; i < 4; ++i)
			Temp[i] = (m[i][0] * rhs.x) + (m[i][1] * rhs.y) + (m[i][2] * rhs.z) + (m[i][3] * rhs.w);

		T Output;
		Output.x = Temp[0];
		Output.y = Temp[1];
		Output.z = Temp[2];
		Output.w = Temp[3];
		return Output;
	}
};



//Need to make sure that these will work with DirectX though they should in all likely hood as
//long as the order of the matrix multiplication is changed or what not
//Matrix creation functions
//Identity
Matrix IdentityMatrix(); //Simply returns 4x4 identity matrix
void IdentityMatrix(Matrix &changeMe); //Changes input to identity;

//Rotation
//Degrees
Matrix BuildXRotationMatrix(float degrees);
Matrix BuildYRotationMatrix(float degrees);
Matrix BuildZRotationMatrix(float degreess);
//Cosine sine
Matrix BuildXRotationMatrix(float cosine, float sine);
Matrix BuildYRotationMatrix(float cosine, float sine);
Matrix BuildZRotationMatrix(float cosine, float sine);

Matrix BuildRotationAboutAxis(Vec4 &axis, float degrees);

//Translation
Matrix BuildTranslationMatrix(float xTrans,float yTrans,float zTrans);
Matrix BuildTranslationMatrix(Vec4 trans);

//Scale
Matrix BuildScaleMatrix(float xScale, float yScale, float zScale);
Matrix BuildScaleMatrix(Vec4 scale);

//Other
float DegreesToRadians(float degrees);
Matrix BuildFromVecT(Vec4 right, Vec4 up, Vec4 view);
Matrix BuildFromVec(Vec4 right, Vec4 up, Vec4 view);
Matrix TensorProduct(Vec4 one, Vec4 two);
Matrix Cross(Vec4 vector);

//Projection
Matrix BuildNaiveProjection(float focal);
Matrix BuildProjectionMatrix(float height, float width, float Far, float Near, float focal);
//Matrix BuildProjectionMatrixFOVH(int fovdegrees, int far, int near);
//Matrix BuildProjectionMatrixFOVV(int fovdegrees, int far, int near);
Matrix BuildProjectionMatrix(float fovY, float aspect, float zNear, float zFar);
//Camera
Matrix BuildCamera(Vec4 Up, Vec4 View, Vec4 position);
Matrix BuildCamera(Vec4 up, Vec4 view, Vec4 right, Vec4 left);

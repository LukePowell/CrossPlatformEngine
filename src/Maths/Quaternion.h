#pragma once
#include "Matrix.h"
#include <cmath>

namespace Maths
{
	struct   Quaternion
	{
		Quaternion(float x = 0, float y = 0, float z = 0, float w = 1.0f);
		
		float Magnitude();
		
		Quaternion operator*(const Quaternion &rhs) const;
		Quaternion &operator*=(const Quaternion &rhs);
		
		Quaternion &operator*=(float s);
		Quaternion &operator +=(const Quaternion &rhs);

		void Normalize();

		friend Quaternion operator*(float s, Quaternion rhs);
		union
		{
			float v[4];
			struct
			{
				float x,y,z,w;
			};
		};
		Matrix QuaternionToMatrix();
	};
}
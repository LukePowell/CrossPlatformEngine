#include "Geometry.h"
#include <algorithm>
#include "Definitions.h"
#include <float.h>

Vec4 Maths::SupportMappingSphere(Vec4 dir, Vec4 origin, float radius)
{
	dir.Normalize();
	return origin + radius * dir;
}

Vec4 Maths::MinimumNorm(Vec4 Q1, Vec4 Q2, Vec4 Q3, Vec4 Q4)
{
	return Vec4();
}

Vec4 Maths::ClosestPointToSegment(Vec4 segmentPt1, Vec4 segmentPt2, Vec4 point)
{
	//Vector from A to B
	Vec4 A_B = segmentPt2 - segmentPt1;

	float t = (point - segmentPt1).Dot(A_B) / A_B.Dot(A_B);

	//Clamp t to a value between 0 and 1.0
	t = std::max(0.0f,std::min(t,1.0f));

	return segmentPt1 + t * A_B;
}

//See Ecricson, Real Time Collision Detection 5.1.5
Vec4 Maths::ClosestPtTriangleToPoint(Vec4 a, Vec4 b, Vec4 c, Vec4 point)
{
	Vec4 AB = b - a;
	Vec4 AC = c - a;
	Vec4 AP = point - a;

	float d1 = AB.Dot(AP);
	float d2 = AC.Dot(AP);

	if(d1 <= 0.0f && d2 <= 0.0f)
		return a;

	Vec4 BP = point - b;
	float d3 = AB.Dot(BP);
	float d4 = AC.Dot(BP);

	if(d3 >= 0.0f && d4 <= d3)
		return b;

	float VC = d1 * d4 - d3 * d2;

	if(VC <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);

		return a + v * AB;
	}

	Vec4 CP = point - c;
	float d5 = AB.Dot(CP);
	float d6 = AC.Dot(CP);
	if(d6 >= 0.0f && d5 <= d6)
		return c;

	float VB = d5 * d2 - d1 * d6;
	if(VB <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		return a + w * AC;
	}


	float VA = d3 * d6  - d5 * d4;
	if(VA <= 0.0f && (d4-d3) >= 0.0f && (d5-d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4-d3) + (d5-d6));
		return b + w * (c-b);
	}

	float denom = 1.0f / (VA + VB + VC);

	float v = VB * denom; 
	float w = VC * denom;

	return a + AB * v + AC * w;
}

Vec4 Maths::ClosesetPointOnTetrahedronToPoint( Vec4& a, Vec4& b, Vec4& c, Vec4& d, Vec4& point )
{
	Vec4 ClosestPoint = point;
	float bestSqDst = FLT_MAX;
	Vec4 Points[4];
	float Distances[4] = {FLT_MAX,FLT_MAX,FLT_MAX,FLT_MAX};

	if(PointOutsideOfPlane(point,a,b,c,d))
	{
		Vec4 q = ClosestPtTriangleToPoint(a,b,c,point);
		Points[0] = q;
		Distances[0] = (q-point).Length();
		float sqDist = (q-point).LengthSq();
		if(sqDist < bestSqDst && Distances[0] > 0.0f)
		{
			bestSqDst = sqDist;
			ClosestPoint = q;
		}
	}
	if(PointOutsideOfPlane(point,a,c,d,b))
	{
		Vec4 q = ClosestPtTriangleToPoint(a,c,d,point);
		Points[1] = q;
		Distances[1] = (q-point).Length();
		float sqDist = (q-point).LengthSq();
		if(sqDist < bestSqDst && Distances[1] > 0.0f)
		{
			bestSqDst = sqDist;
			ClosestPoint = q;
		}
	}
	if(PointOutsideOfPlane(point,a,d,b,c))
	{
		Vec4 q = ClosestPtTriangleToPoint(a,d,b,point);
		Points[2] = q;
		Distances[2] = (q-point).Length();
		float sqDist = (q-point).LengthSq();
		if(sqDist < bestSqDst && Distances[2] > 0.0f)
		{
			bestSqDst = sqDist;
			ClosestPoint = q;
		}
	}
	if(PointOutsideOfPlane(point,b,d,c,a))
	{
		Vec4 q = ClosestPtTriangleToPoint(b,d,c,point);
		Points[3] = q;
		Distances[3] = (q-point).Length();
		float sqDist = (q-point).LengthSq();
		if(sqDist < bestSqDst && Distances[3] > 0.0f)
		{
			bestSqDst = sqDist;
			ClosestPoint = q;
		}
	}
	//if(Distances[0] < 0 && Distances[1] < 0 && Distances[2] < 0 && Distances[3] < 0)
	//	return point;
	return ClosestPoint;
}

bool Maths::PointOutsideOfPlane( Vec4 Point, Vec4 a, Vec4 b, Vec4 c )
{
	return (Point-a).Dot((b-a).Cross(c-a)) >= 0.0f;
}

bool Maths::PointOutsideOfPlane( Vec4 Point, Vec4 a, Vec4 b, Vec4 c, Vec4 d )
{
	float signp = (Point - a).Dot((b-a).Cross(c-a));
	float signd = (d-a).Dot((b-a).Cross(c-a));
	return signp * signd < 0.0f;
}

Vec4 Maths::ClosestPointToPointOBB( Vec4 point, OBB obb )
{
	Vec4 d = point - obb.m_COM;

	Vec4 q = obb.m_COM;
	Matrix Orientation = obb.m_Axis.QuaternionToMatrix();

	for(int i = 0; i < 3; i++)
	{
		float dist = Orientation[i][0] * d.x + Orientation[i][1] * d.y + d.z * Orientation[i][2];
		if(dist > obb.m_HalfExtents[i])
			dist = obb.m_HalfExtents[i];
		if(dist < -obb.m_HalfExtents[i])
			dist = -obb.m_HalfExtents[i];

		q += dist * Vec4(Orientation[i][0],Orientation[i][1],Orientation[i][2]);
	}

	return q;
}

float Maths::Clamp(float n, float min_, float max_)
{
	return std::max(min_,std::min(max_,n));
}

float Maths::ClosestPtSegementSegment(Vec4 p1, Vec4 q1, Vec4 p2, Vec4 q2, float &s, float &t, Vec4 &c1, Vec4 &c2)
{
	Vec4 d1 = q1 - p1; // Direction vector of segment S1
    Vec4 d2 = q2 - p2; // Direction vector of segment S2
    Vec4 r = p1 - p2;

	float a = d1.LengthSq(); // Squared length of segment S1, always nonnegative
	float e = d2.LengthSq(); // Squared length of segment S2, always nonnegative
	float f = d2.Dot(r);

    // Check if either or both segments degenerate into points
    if (a <= EPSILON && e <= EPSILON) {
        // Both segments degenerate into points
        s = t = 0.0f;
        c1 = p1;
        c2 = p2;
        Vec4 temp = c1-c2;
		return (c1 - c2).Dot(temp);
    }
    if (a <= EPSILON) {
        // First segment degenerates into a point
        s = 0.0f;
        t = f / e; // s = 0 => t = (b*s + f) / e = f / e
        t = Clamp(t, 0.0f, 1.0f);
    } else {
		float c = d1.Dot(r);
        if (e <= EPSILON) {
            // Second segment degenerates into a point
            t = 0.0f;
            s = Clamp(-c / a, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a
        } else {
            // The general nondegenerate case starts here
			float b = d1.Dot(d2);
            float denom = a*e-b*b; // Always nonnegative

            // If segments not parallel, compute closest point on L1 to L2, and
            // clamp to segment S1. Else pick arbitrary s (here 0)
            if (denom != 0.0f) {
                s = Clamp((b*f - c*e) / denom, 0.0f, 1.0f);
            } else s = 0.0f;

            // Compute point on L2 closest to S1(s) using
            // t = Dot((P1+D1*s)-P2,D2) / Dot(D2,D2) = (b*s + f) / e
            t = (b*s + f) / e;

            // If t in [0,1] done. Else clamp t, recompute s for the new value
            // of t using s = Dot((P2+D2*t)-P1,D1) / Dot(D1,D1)= (t*b - c) / a
            // and clamp s to [0, 1]
            if (t < 0.0f) {
                t = 0.0f;
                s = Clamp(-c / a, 0.0f, 1.0f);
            } else if (t > 1.0f) {
                t = 1.0f;
                s = Clamp((b - c) / a, 0.0f, 1.0f);
            }
        }
    }

    c1 = p1 + d1 * s;
    c2 = p2 + d2 * t;
	return (c1 - c2).Dot(c1 - c2);
}
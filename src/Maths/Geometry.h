#pragma  once
//File to handle geometric things
#include "Vectors.h"
#include "Quaternion.h"
#include <vector>

namespace Maths
{
	Vec4 MinimumNorm(Vec4 Q1, Vec4 Q2, Vec4 Q3, Vec4 Q4);
	//SuportMapping functions
	Vec4 SupportMappingSphere(Vec4 dir, Vec4 origin, float radius);
	void ExtremePointsAlongDirection(Vec4 dir, const std::vector<Vec4> &points, int &minIndex, int &maxIndex);
	//Closest Point Calculation
	Vec4 ClosestPointToSegment(Vec4 segmentPt1, Vec4 segmentPt2, Vec4 point);

	Vec4 ClosestPtTriangleToPoint(Vec4 a, Vec4 b, Vec4 c, Vec4 point);

	Vec4 ClosesetPointOnTetrahedronToPoint(Vec4 &a, Vec4 &b, Vec4 &c, Vec4 &d, Vec4 &point);

	bool PointOutsideOfPlane(Vec4 Point, Vec4 a, Vec4 b, Vec4 c);
	bool PointOutsideOfPlane(Vec4 Point, Vec4 a, Vec4 b, Vec4 c, Vec4 d);

	struct OBB
	{
		Vec4 m_COM;
		Quaternion m_Axis;
		Vec4 m_HalfExtents;
	};

	Vec4 ClosestPointToPointOBB( Vec4 point, OBB obb );

	float Clamp(float n, float min_, float max);

	float ClosestPtSegementSegment(Vec4 p1, Vec4 q1, Vec4 p2, Vec4 q2, float &s, float &t, Vec4 &c1, Vec4 &c2);
}
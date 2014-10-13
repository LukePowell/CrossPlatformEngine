//
//  CS350BoundingVolume.cpp
//  SFMLTest
//
//  Created by Luke Powell on 3/3/14.
//
//

#include "CS350BoundingVolume.h"
#include "GraphicsSystem/GraphicsSystem.h"
#include <float.h>
#include "Transform.h"
#include <iostream>

bool CS350::BoundingVolumeHierarchy::draw = false;

CS350::CS350BoundingVolume::CS350BoundingVolume() : IComponent("CS350BoundingVolume")
{
}

CS350::CS350BoundingVolume::~CS350BoundingVolume()
{
    
}

CS350::AABB *CS350::createAABB(ElysiumEngine::Mesh *points)
{
    CS350::AABB *bv = new CS350::AABB();
    
    int minx = 0, maxx = 0, miny = 0, maxy = 0, minz = 0, maxz = 0;
    
    for(int i = 0; i < points->vertexCount; ++i)
    {
        if(points->vertices[i].position.x < points->vertices[minx].position.x)
            minx = i;
        if(points->vertices[i].position.y < points->vertices[miny].position.y)
            miny = i;
        if(points->vertices[i].position.z < points->vertices[minz].position.z)
            minz = i;
        if(points->vertices[i].position.x > points->vertices[maxx].position.x)
            maxx = i;
        if(points->vertices[i].position.y > points->vertices[maxy].position.y)
            maxy = i;
        if(points->vertices[i].position.z > points->vertices[maxz].position.z)
            maxz = i;
    }
    
    
    float maxX = points->vertices[maxx].position.x;
    float minX = points->vertices[minx].position.x;
    
    float maxY = points->vertices[maxy].position.y;
    float minY = points->vertices[miny].position.y;
    
    float maxZ = points->vertices[maxz].position.z;
    float minZ = points->vertices[minz].position.z;
    
    bv->center.x = (maxX + minX) / 2;
    bv->center.y = (maxY + minY) / 2;
    bv->center.z = (maxZ + minZ) / 2;
    
    bv->halfWidth = (maxX - minX) / 2;
    bv->halfHeight = (maxY - minY) / 2;
    bv->halfDepth = (maxZ - minZ) / 2;
    
    return bv;
}

CS350::SphereBV::SphereBV(Vec4 center, float radius) : center(center), radius(radius)
{
    
}

CS350::SphereBV *CS350::createSphereCentroidMethod(ElysiumEngine::Mesh *mesh)
{
    if(!mesh || mesh->vertexCount <= 0)
        return nullptr;
    
    Vec4 centroid;
    for(int i = 0; i < mesh->vertexCount; ++i)
    {
        centroid += mesh->vertices[i].position;
    }
    
    centroid /= mesh->vertexCount;
    
    Vec4 farthest = mesh->vertices[0].position;
    float distanceToFarthest = (farthest - centroid).LengthSq();
    for(int i = 1; i < mesh->vertexCount; ++i)
    {
        float distanceSq = (mesh->vertices[i].position-centroid).LengthSq();
        if(distanceSq > distanceToFarthest)
        {
            distanceToFarthest = distanceSq;
            farthest = (mesh->vertices[i].position);
        }
    }
    
    float radius = (farthest-centroid).Length();
    
    return new CS350::SphereBV(centroid,radius);
}

void MostSeperatedPointsOnAABB(int &min, int &max, ElysiumEngine::Mesh *points)
{
    int minx = 0, maxx = 0, miny = 0, maxy = 0, minz = 0, maxz = 0;
    
    for(int i = 1; i < points->vertexCount; ++i)
    {
        if(points->vertices[i].position.x < points->vertices[minx].position.x)
            minx = i;
        if(points->vertices[i].position.y < points->vertices[miny].position.y)
            miny = i;
        if(points->vertices[i].position.z < points->vertices[minz].position.z)
            minz = i;
        if(points->vertices[i].position.x > points->vertices[maxx].position.x)
            maxx = i;
        if(points->vertices[i].position.y > points->vertices[maxy].position.y)
            maxy = i;
        if(points->vertices[i].position.z > points->vertices[maxz].position.z)
            maxz = i;
    }
    
    float dist2x = (points->vertices[maxx].position - points->vertices[minx].position).LengthSq();
    float dist2y = (points->vertices[maxy].position - points->vertices[miny].position).LengthSq();
    float dist2z = (points->vertices[maxz].position - points->vertices[minz].position).LengthSq();
    
    min = minx;
    max = maxx;
    if(dist2y > dist2x && dist2y > dist2z)
    {
        min = miny;
        max = maxy;
    }
    
    if(dist2z > dist2x && dist2z > dist2y)
    {
        min = minz;
        max = maxz;
    }
}

void SphereFromDistantPoints(CS350::SphereBV &sphere, ElysiumEngine::Mesh *points)
{
    int min, max;
    MostSeperatedPointsOnAABB(min, max, points);
    
    sphere.center = (points->vertices[min].position + points->vertices[max].position) * 0.5f;
    sphere.radius = (points->vertices[max].position-sphere.center).Length();
}

void sphereOfSphereAndSpherePoint(CS350::SphereBV &s, Vec4 &position)
{
    Vec4 d = position - s.center;
    float dist2 = d.LengthSq();
    if(dist2 > (s.radius * s.radius))
    {
        float dist = d.Length();
        float newRadius = (s.radius + dist) * 0.5f;
        float k = (newRadius - s.radius) / dist;
        s.radius = newRadius;
        s.center += d * k;
    }
}

CS350::SphereBV *CS350::createSphereRittersMethod(ElysiumEngine::Mesh *mesh)
{
    CS350::SphereBV *sphere = new CS350::SphereBV(Vec4(),0.0f);
    
    SphereFromDistantPoints(*sphere, mesh);
    
    for(int i = 0; i < mesh->vertexCount; ++i)
    {
        sphereOfSphereAndSpherePoint(*sphere, mesh->vertices[i].position);
    }
    
    return sphere;
}

float variance(float *x, int n)
{
	float u = 0.0f;
	for (int i = 0; i < n; ++i)
	{
		u += x[i];
	}

	u /= n;

	float s2 = 0.0f;
	for (int i = 0; i < n; ++i)
	{
		s2 += (x[i] - u) * (x[i] - u);
	}

	return s2 / n;
}

void CovarianceMatrix(Matrix &covariance,const std::vector<Vec4> &points)
{
	float oon = 1.0f / (float)points.size();
	Vec4 c;
	float e00 = 0.0f, e11 = 0.0f, e22 = 0.0f, e01 = 0.0f, e02 = 0.0f, e12 = 0.0f;

	for (Vec4 point : points)
	{
		c += point;
	}

	c *= oon;

	
	for (Vec4 point : points)
	{
		e00 += point.x * point.x;
		e11 += point.y * point.y;
		e22 += point.z * point.z;
		e01 += point.x * point.y;
		e02 += point.x * point.z;
		e12 += point.y * point.z;
	}

	covariance[0][0] = e00 * oon;
	covariance[1][1] = e11 * oon;
	covariance[2][2] = e22 * oon;
	covariance[0][1] = covariance[1][0] = e01 * oon;
	covariance[0][2] = covariance[2][0] = e02 * oon;
	covariance[1][2] = covariance[2][1] = e12 * oon;
}

void SymSchur2(Matrix &a, int p, int q, float &c, float &s)
{
	const float epsilon = 0.0001f;
	if (std::abs(a[p][q]) > epsilon)
	{
		float r = (a[q][q] - a[p][p]) / (2.0f * a[p][q]);
		float t;
		if (r >= 0.0f)
			t = 1.0f / (r + std::sqrtf(1.0f + r * r));
		else
			t = -1.0f / (-r + std::sqrtf(1.0f + r * r));
		c = 1.0f / std::sqrt(1.0f + t * t);
		s = t * c;
	}
	else
	{
		c = 1.0f;
		s = 0.0f;
	}
}

void jacobi(Matrix &a, Matrix &v)
{
	
	v.Identity();//Initialze to indentity

	const int MAX_ITERATIONS = 50;
	
	int n = 0, p , q;
	float prevoff, c, s;
	Matrix J, b, t;

	for (; n < MAX_ITERATIONS; ++n)
	{
		p = 0;
		q = 1;
		a.DoForEachElement3([&p, &q,&a](float value, int x, int y){
			if (x == y)
				return;
			if (std::abs(value) > std::abs(a[p][q]))
			{
				p = y;
				q = x;
			}
		});

		SymSchur2(a, p, q, c, s);
		for (int i = 0; i < 3; ++i)
		{
			J[i][0] = J[i][1] = J[i][2] = 0.0f;
			J[i][i] = 1.0f;
		}

		J[p][p] = c; 
		J[p][q] = s;
		J[q][p] = -s;
		J[q][q] = c;
			
		v *= J;

		a = (J.transpose() * a)  * J;

		float off = 0.0f;
		a.DoForEachElement3([&off](float value, int x, int y){
			if (x == y)
				return;
			off += value;
		});

		if (n > 2 && off >= prevoff)
			return;

		prevoff = off;
	}
}

void EigenSphere(CS350::SphereBV &eigSphere, std::vector<Vec4> points)
{
	Matrix m, v;
	CovarianceMatrix(m, points);
	jacobi(m, v);

	Vec4 e;
	int maxc = 0;
	float maxf, maxe = std::abs(m[0][0]);
	if ((maxf = std::abs(m[1][1])) > maxe)
	{
		maxc = 1;
		maxe = maxf;
	}
	if ((maxf = std::abs(m[2][2])) > maxe)
	{
		maxc = 2;
		maxe = maxf;
	}

	e[0] = v[0][maxc];
	e[1] = v[1][maxc];
	e[2] = v[2][maxc];

	int imin, imax;
	Maths::ExtremePointsAlongDirection(e, points, imin, imax);
	Vec4 minpt = points[imin];
	Vec4 maxpt = points[imax];

	float dist = (maxpt - minpt).Length();
	eigSphere.radius = dist * 0.5f;
	eigSphere.center = (minpt + maxpt) * 0.5f;
}


Vec4 normalsForLarson[] = { Vec4(1,0,0), Vec4(0,1,0), Vec4(0,0,1),
							Vec4(1,1,1), Vec4(1,1,-1), Vec4(1,-1,1), Vec4(1,-1,-1),
							Vec4(1,1,0), Vec4(1,-1,0), Vec4(1,0,1), Vec4(1,0,-1), Vec4(0,1,1), Vec4(0,1,-1),
                            Vec4(0,1,2), Vec4(0,2,1), Vec4(1,0,2), Vec4(2,0,1), Vec4(1,2,0), Vec4(2,1,0),
                            Vec4(0,1,-2), Vec4(0,2,-1), Vec4(1,0,-2), Vec4(2,0,-1), Vec4(1,-2,0), Vec4(2,-1,0),
                            Vec4(1,1,2), Vec4(2,1,1), Vec4(1,2,1), Vec4(1,-1,2), Vec4(1,1,-2), Vec4(1,-1,-2),
                            Vec4(2,-1,1), Vec4(2,1,-1), Vec4(2,-1,-1), Vec4(1,-2,1), Vec4(1,2,-1), Vec4(1,-2,-1),
                            Vec4(2,2,1), Vec4(1,2,2), Vec4(2,1,2), Vec4(2,-2,1), Vec4(2,2,-1), Vec4(2,-2,-1),
                            Vec4(1,-2,2), Vec4(1,2,-2), Vec4(1,-2,-2), Vec4(2,-1,2), Vec4(2,1,-2), Vec4(2,-1,-2)};

const int s = 49;

std::vector<Vec4> ExtremalPoints(const std::vector<Vec4> &points)
{
	std::vector<Vec4> output;
	for (int i = 0; i < s; ++i)
	{
		int min, max;
		Maths::ExtremePointsAlongDirection(normalsForLarson[i], points, min, max);
		output.push_back(points[min]);
		output.push_back(points[max]);
	}

	return output;
}

CS350::SphereBV *minimumSphere(std::vector<Vec4> &points)
{
	float radius = FLT_MAX;
	std::vector<Vec4> pointsout;
	//All sets of 2
	for (int i = 0; i < points.size(); ++i)
	{
		for (int j = i + 1; j < points.size(); ++j)
		{
			if ((points[i] - points[j]).Length() < radius)
			{
				pointsout.clear();
				pointsout.push_back(points[i]);
				pointsout.push_back(points[j]);
				radius = (points[1] - points[j]).Length() / 2;
			}
		}
	}

	CS350::SphereBV *bv = new CS350::SphereBV(Vec4(), 0.0f);
	bv->center = (pointsout[0] + pointsout[1]) * (1.0f / (float)pointsout.size());
	bv->radius = radius;
	return bv;

	//All sets of 3

	//All sets of 4
}
CS350::SphereBV *CS350::createSphereLarsonsMethod(ElysiumEngine::Mesh *mesh)
{
	std::vector<Vec4> points;
	for (int i = 0; i < mesh->vertexCount; ++i)
	{
		points.push_back(mesh->vertices[i].position);
	}
	CS350::SphereBV *bv = new CS350::SphereBV(Vec4(), 0.0f);
	if (mesh->vertexCount > (2 * s))
	{
		std::vector<Vec4> e = ExtremalPoints(points);
		bv = minimumSphere(e);
		for (Vec4 point : points)
		{
			sphereOfSphereAndSpherePoint(*bv, point);
		}
	}
	else
	{
		bv = minimumSphere(points);
	}
	return bv;
}

CS350::SphereBV *CS350::createSpherePCAMethod(ElysiumEngine::Mesh *mesh)
{
	CS350::SphereBV *bv = new CS350::SphereBV(Vec4(), 0.0f);

	std::vector<Vec4> points;
	for (int i = 0; i < mesh->vertexCount; ++i)
	{
		points.push_back(mesh->vertices[i].position);
	}
	EigenSphere(*bv, points);

	for (Vec4 point : points)
	{
		sphereOfSphereAndSpherePoint(*bv, point);
	}

    return bv;
}

CS350::EllipsoidBV *CS350::createEllipsoidPCAMethod(ElysiumEngine::Mesh *mesh)
{
	std::vector<Vec4> points;
	for (int i = 0; i < mesh->vertexCount; ++i)
	{
		points.push_back(mesh->vertices[i].position);
	}

	Matrix m, v;
	CovarianceMatrix(m, points);
	jacobi(m, v);

	Vec4 one(m[0][0], m[0][1], m[0][2]);
	Vec4 two(m[1][0], m[1][1], m[1][2]);
	Vec4 three(m[2][0], m[2][1], m[2][2]);


	Vec4 center;
	int imin, imax;
	Maths::ExtremePointsAlongDirection(one, points, imin, imax);
	float x = (points[imin] - points[imax]).Length() / 2.0f;
	center.x = ((points[imin] + points[imax]) * 0.5f).x;

	Maths::ExtremePointsAlongDirection(two, points, imin, imax);
	float y = (points[imin] - points[imax]).Length() / 2.0f;
	center.y = ((points[imin] + points[imax]) * 0.5f).y;

	Maths::ExtremePointsAlongDirection(three, points, imin, imax);
	float z = (points[imin] - points[imax]).Length() / 2.0f;
	center.z = ((points[imin] + points[imax]) * 0.5f).z;

	EllipsoidBV*obb = new EllipsoidBV();
	obb->extents = Vec4(x, y, z);
	obb->center = center;
	//Change the 3 axis to a quaternion for reasons. 

	return obb;
}

CS350::OBB *CS350::createOBBPCAMethod(ElysiumEngine::Mesh *mesh)
{
	std::vector<Vec4> points;
	for (int i = 0; i < mesh->vertexCount; ++i)
	{
		points.push_back(mesh->vertices[i].position);
	}

	Matrix m, v;
	CovarianceMatrix(m, points);
	jacobi(m, v);

	Vec4 one(m[0][0], m[0][1], m[0][2]);
	Vec4 two(m[1][0], m[1][1], m[1][2]);
	Vec4 three(m[2][0], m[2][1], m[2][2]);


	Vec4 center;
	int imin, imax;
	Maths::ExtremePointsAlongDirection(one, points, imin, imax);
	float x = (points[imin] - points[imax]).Length() / 2.0f;
	center.x = ((points[imin] + points[imax]) * 0.5f).x;

	Maths::ExtremePointsAlongDirection(two, points, imin, imax);
	float y = (points[imin] - points[imax]).Length() / 2.0f;
	center.y = ((points[imin] + points[imax]) * 0.5f).y;

	Maths::ExtremePointsAlongDirection(three, points, imin, imax);
	float z = (points[imin] - points[imax]).Length() / 2.0f;
	center.z = ((points[imin] + points[imax]) * 0.5f).z;

	OBB *obb = new OBB();
	obb->extents = Vec4(x, y, z);
	obb->center = center;
	//Change the 3 axis to a quaternion for reasons. 

	return obb;
}

void CS350::SphereBV::debugDraw()
{
    if(hasSibling("Transform"))
    {
        ElysiumEngine::Transform *t = getSibling<ElysiumEngine::Transform>("Transform");
        Vec4 pos = t->GetPosition();

        ElysiumEngine::DrawDebugSphere sphere(pos + (center * t->GetScale().x),radius * t->GetScale().x,Vec4(1.0f,0.0f,0.0f,1.0f),1.0f/60.0f);
        ElysiumEngine::MessagingSystem::g_MessagingSystem->broadcastMessage(&sphere);
    }
}

void CS350::AABB::debugDraw()
{
    if(hasSibling("Transform"))
    {
        ElysiumEngine::Transform *t = getSibling<ElysiumEngine::Transform>("Transform");
        Vec4 pos = t->GetPosition();
        Vec4 scale = t->GetScale();
        ElysiumEngine::DrawDebugBox box(pos + (center * t->GetScale().x),Vec4(scale.x * halfWidth,scale.y * halfHeight,scale.z * halfDepth),Vec4(1.0f,0.0f,0.0f));
        ElysiumEngine::MessagingSystem::g_MessagingSystem->broadcastMessage(&box);
    }
}

void CS350::CS350BoundingVolumeRenderer::render()
{
    if(volume)
    {
        volume->debugDraw();
    }
}

void CS350::topDownBVHTree(CS350::Node **tree,CS350::CS350BoundingVolume **objects, int numObjects)
{
    Node *node = new Node();
    *tree = node;
    //node->volume = computeBoundingVolume(objects,numObjects);
    
    /*if(numObjects <= 1)
    {
        node->leaf = true;
        node->numObjects = numObjects;
    }
    else
    {
        node->leaf = false;
        int k = partitionObjects(objects,numObjects);
        
        topDownBVHTree(&(node->left), &objects[0],k);
        topDownBVHTree(&(node->right), &objects[k],numObjects-k);
    }*/
}

void CS350::bottomUPBVHTree(CS350::Node **tree,CS350::CS350BoundingVolume **objects, int numObjects)
{
    
}

CS350::BoundingVolumeHierarchy CS350::buildBVHBottomUp(CS350BoundingVolume ** objects, int count)
{
    BoundingVolumeHierarchy bvh;
    topDownBVHTree(&(bvh.root),objects,count);
    return bvh;
}

CS350::BoundingVolumeHierarchy CS350::buildBVHTopDown(CS350BoundingVolume ** objects, int count)
{
    BoundingVolumeHierarchy bvh;
    topDownBVHTree(&(bvh.root),objects,count);
    return bvh;
}

void CS350::BoundingVolumeHierarchy::drawHelper(CS350::Node *node)
{
    if(!node)
        return;
    
    node->volume->debugDraw();
    drawHelper(node->left);
    drawHelper(node->right);
}

void CS350::BoundingVolumeHierarchy::render()
{
    if(!draw)
        return;//We are not drawing any of the hierarchys right now skip all code in this function
    drawHelper(root);
}

void CS350::OBB::debugDraw()
{
	if (hasSibling("Transform"))
	{
		ElysiumEngine::Transform *t = getSibling<ElysiumEngine::Transform>("Transform");
		Vec4 pos = t->GetPosition();
		Vec4 scale = t->GetScale();
		ElysiumEngine::DrawDebugBox box(pos + (center * t->GetScale().x), Vec4(scale.x * extents.x, scale.y * extents.y, scale.z * extents.z), Vec4(1.0f, 0.0f, 0.0f));
		ElysiumEngine::MessagingSystem::g_MessagingSystem->broadcastMessage(&box);
	}
}

void CS350::EllipsoidBV::debugDraw()
{
	if (hasSibling("Transform"))
	{
		ElysiumEngine::Transform *t = getSibling<ElysiumEngine::Transform>("Transform");
		Vec4 pos = t->GetPosition();

		ElysiumEngine::DrawDebugSphere sphere(pos + (center * t->GetScale().x), extents.Component(t->GetScale()) , Vec4(1.0f, 0.0f, 0.0f, 1.0f), 1.0f / 60.0f);
		ElysiumEngine::MessagingSystem::g_MessagingSystem->broadcastMessage(&sphere);
	}
}

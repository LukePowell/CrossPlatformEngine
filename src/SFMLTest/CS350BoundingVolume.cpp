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


CS350::SphereBV *CS350::createSphereLarsonsMethod(ElysiumEngine::Mesh *mesh)
{
    return nullptr;
}

CS350::SphereBV *CS350::createSpherePCAMethod(ElysiumEngine::Mesh *mesh)
{
    return nullptr;
}

CS350::EllipsoidBV *CS350::createEllipsoidPCAMethod(ElysiumEngine::Mesh *mesh)
{
    return nullptr;
}

CS350::OBB *CS350::createOBBPCAMethod(ElysiumEngine::Mesh *mesh)
{
    return nullptr;
}

void CS350::SphereBV::debugDraw()
{
    if(hasSibling("Transform"))
    {
        ElysiumEngine::Transform *t = getSibling<ElysiumEngine::Transform>("Transform");
        Vec4 pos = t->GetPosition();
        pos += center;

        ElysiumEngine::DrawDebugSphere sphere(pos,radius * t->GetScale().x,Vec4(1.0f,0.0f,0.0f,1.0f),1.0f/60.0f);
        ElysiumEngine::MessagingSystem::g_MessagingSystem->broadcastMessage(&sphere);
    }
}

void CS350::AABB::debugDraw()
{
    if(hasSibling("Transform"))
    {
        ElysiumEngine::Transform *t = getSibling<ElysiumEngine::Transform>("Transform");
        Vec4 pos = t->GetPosition();
        pos += center;
        Vec4 scale = t->GetScale();
        ElysiumEngine::DrawDebugBox box(pos,Vec4(scale.x * halfWidth,scale .y * halfHeight,scale .z * halfDepth),Vec4(1.0f,0.0f,0.0f));
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
//
//  CS350BoundingVolume.h
//  SFMLTest
//
//  Created by Luke Powell on 3/3/14.
//
//
#pragma once
#ifndef __SFMLTest__CS350BoundingVolume__
#define __SFMLTest__CS350BoundingVolume__
#include "GameObject.h"
#include "GraphicsSystem/Mesh.h"
#include <vector>

namespace CS350
{
    class CS350BoundingVolume : public ElysiumEngine::IComponent
    {
    public:
        CS350BoundingVolume();//All bouding volumes use same name, bit bad but fine
        virtual ~CS350BoundingVolume();
        virtual void debugDraw() = 0;
        
        virtual void serialize(FileIO::IReader &reader){}
        virtual void deserialize(){}

        virtual void initialize(){}
        virtual void lateInitialize(){}
    private:
    };
    
    class AABB : public CS350BoundingVolume
    {
    public:
        void debugDraw();
        Vec4 center;
        float halfWidth, halfHeight, halfDepth;
    };
    
    class SphereBV : public CS350BoundingVolume
    {
    public:
        SphereBV(Vec4 center, float radius);
        void debugDraw();
        Vec4 center;
        float radius;
    };
    
    class EllipsoidBV : public CS350BoundingVolume
    {
    public:
		void debugDraw();
        Vec4 center;
        Vec4 extents;
		//Maths::Quaternion oritentation;
    };
    
    class OBB : public CS350BoundingVolume
    {
	public:
		void debugDraw();
		Vec4 center;
		Vec4 extents;
		//Maths::Quaternion oritentation;
    };
    
    class CS350BoundingVolumeRenderer : public ElysiumEngine::IRenderable
    {
    public:
        CS350BoundingVolumeRenderer() : IRenderable("CS350BoundingVolumeRenderer") {volume = nullptr;}
        void render();
        
        virtual void serialize(FileIO::IReader &reader){};
        virtual void deserialize(){}
        
        
        virtual void initialize(){}
        virtual void lateInitialize(){}
        void setVolume(CS350BoundingVolume *volume){this->volume = volume;}
    private:
        CS350BoundingVolume *volume;
    };
    
    AABB *createAABB(ElysiumEngine::Mesh *mesh);
    SphereBV *createSphereCentroidMethod(ElysiumEngine::Mesh *mesh);
    SphereBV *createSphereRittersMethod(ElysiumEngine::Mesh *mesh);
    SphereBV *createSphereLarsonsMethod(ElysiumEngine::Mesh *mesh);
    SphereBV *createSpherePCAMethod(ElysiumEngine::Mesh *mesh);
    EllipsoidBV *createEllipsoidPCAMethod(ElysiumEngine::Mesh *mesh);
    OBB *createOBBPCAMethod(ElysiumEngine::Mesh *mesh);
    
    class Node
    {
    public:
        CS350BoundingVolume *volume;
        Node *left;
        Node *right;
        unsigned int numObjects;
        bool leaf;
        
    };
    
    //Will render self
    class BoundingVolumeHierarchy : public ElysiumEngine::IRenderable
    {
    public:
        BoundingVolumeHierarchy() : IRenderable("BoundingVolumeHierarchy"){}
        void render();
        virtual void serialize(FileIO::IReader &reader){};
        virtual void deserialize(){}
        virtual void initialize(){}
        virtual void lateInitialize(){}
        //Default to not rendering, simply change this value to true.
        static bool draw;
        Node *root;
    private:
        void drawHelper(Node *next);
    };
    
    BoundingVolumeHierarchy buildBVHTopDown(CS350BoundingVolume ** objects, int count);
    BoundingVolumeHierarchy buildBVHBottomUp(CS350BoundingVolume ** objects, int count);
    void topDownBVHTree(CS350::Node **tree,CS350::CS350BoundingVolume **objects, int numObjects);
    void bottomUPBVHTree(CS350::Node **tree, CS350::CS350BoundingVolume **objects, int numObjects);
}

#endif /* defined(__SFMLTest__CS350BoundingVolume__) */

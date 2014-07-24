#pragma once

#include "IRenderable.h"
#include "IShader.h"

#include <list>

namespace ElysiumEngine
{
    struct Point
    {
        Vec4 com;
        Vec4 color;
        float lifetime;
    };
    
    struct Box
    {
        Vec4 com;
        Vec4 color;
        float lifetime;
        Vec4 halfExtents;
    };
    
    struct Sphere
    {
        Vec4 com;
        Vec4 color;
        float radius;
        float lifetime;
        GameObject *obj;
    };
    
    struct Line
    {
        Vec4 begin;
        Vec4 end;
        Vec4 color;
        float lifetime;
    };
    
    class DebugDrawRenderable : public IRenderable
    {
    public:
        DebugDrawRenderable();
        
        virtual void serialize(FileIO::IReader &reader){}
        virtual void deserialize(){}
        
        virtual void initialize(){}
        virtual void lateInitialize();
        
        virtual void render();
        
        void handleMessage(DrawDebugPoint &msg);
        void handleMessage(DrawDebugLine &msg);
        void handleMessage(DrawDebugBox &msg);
        void handleMessage(DrawDebugSphere &msg);
        void update(float dt);
        
    private:
        std::list<Point> points;
        std::list<Box> boxes;
        std::list<Sphere> spheres;
        std::list<Line> lines;
        IShader *shader;
        unsigned int boxVAO;
        unsigned int lineBuffer;
        unsigned int boxBuffer;
        unsigned int sphereBuffer;
        unsigned int pointBuffer;
    };
}

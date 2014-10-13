#include "DebugDraw.h"
#include "GraphicsDebug.h"
#include "OpenGLHeaders.hpp"
#include "GameObjectFactory.h"
#include "Transform.h"
#include "Mesh.h"
#include "MeshRenderable.h"
#include "GraphicsSystem.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

static float boxVerts[] = {
	0.5f,0.5f,0.5f,
    0.5f,0.5f,-0.5f,
    
    0.5f,-0.5f,0.5f,
    0.5f,-0.5f,-0.5f,
    
    -0.5f,0.5f,0.5f,
    -0.5f,0.5f,-0.5f,
    
    -0.5f,-0.5f,0.5f,
    -0.5f,-0.5f,-0.5f,
    
    0.5f, 0.5f,0.5f,
    -0.5f,0.5f,0.5f,
    
    0.5f, 0.5f, -0.5f,
    -0.5f,0.5f,-0.5f,
    
    0.5f, -0.5f,0.5f,
    -0.5f,-0.5f,0.5f,
    
    0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    
    0.5f,0.5f,0.5f,
    0.5f,-0.5f,0.5f,
    
    0.5f,0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    
    -0.5f,0.5f,0.5f,
    -0.5f,-0.5f,0.5f,
    
    -0.5f,0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f};

extern glm::mat4x4 proj;
extern glm::mat4x4 mv;

ElysiumEngine::DebugDrawRenderable::DebugDrawRenderable() : IRenderable("DebugDrawRenderable")
{
}


void ElysiumEngine::DebugDrawRenderable::lateInitialize()
{
    shader = GraphicsSystem::g_GraphicsSystem->getShaderFactory().getShader("DebugDraw");
    GLCall(glGenBuffers(1, &boxBuffer));
    
    GLCall(glGenVertexArrays(1, &boxVAO));
    GLCall(glBindVertexArray(boxVAO));
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, boxBuffer));
    
    GLCall(glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(float),boxVerts, GL_STATIC_DRAW));
    
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0));
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, boxBuffer));
    
    GLCall(glBindVertexArray(0));
    
    GLCall(glGenBuffers(1, &pointBuffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, pointBuffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3000, nullptr, GL_DYNAMIC_DRAW));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void ElysiumEngine::DebugDrawRenderable::render()
{
	if(!shader)
		return;

    int i = 0;
    
    /*glPointSize(10.0f);
    glBegin(GL_POINTS);
    for(auto &point : points)
    {
        glColor3fv(point.color.v);
        glVertex3fv(point.com.v);
        i += 3;
    }
    glEnd();*/
    
    
    /*glBegin(GL_LINES);
    for(auto &line : lines)
    {
        glColor3fv(line.color.v);
        glVertex3fv(line.begin.v);
        glColor3fv(line.color.v);
        glVertex3fv(line.end.v);
    }
    glEnd();*/

    shader->startPass(0);
    GLCall(glBindVertexArray(boxVAO));
    for(auto &box : boxes)
    {
        Matrix trans = BuildTranslationMatrix(box.com);
        Matrix rot;
        Matrix scale = BuildScaleMatrix(box.halfExtents * 2);
        rot.Identity();
        
        shader->setUniformMat4("uTrans", trans.v, true);
        shader->setUniformMat4("uScale", scale.v, true);
        shader->setUniformMat4("uRot", rot.v, true);
        shader->setUniformVert4Data("uColor", box.color.v);
        shader->setUniformMat4("uProjection", glm::value_ptr(proj), false);
        shader->setUniformMat4("uView", glm::value_ptr(mv), false);
        
        GLCall(glDrawArrays(GL_LINES, 0, 24));
    }
    GLCall(glBindVertexArray(0));
    shader->endPass();
    

    //GLCall(glDrawArrays(GL_POINTS, 0, points.size()));
    
    //GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void ElysiumEngine::DebugDrawRenderable::update(float dt)
{
    auto pointIt = points.begin();
    while(pointIt != points.end())
    {
        pointIt->lifetime -= dt;
        if(pointIt->lifetime <= 0.0f)
        {
            pointIt = points.erase(pointIt);
        }
        else
        {
            ++pointIt;
        }
    }
    
    auto lineIt = lines.begin();
    while(lineIt != lines.end())
    {
        lineIt->lifetime -= dt;
        if(lineIt->lifetime <= 0.0f)
        {
            lineIt = lines.erase(lineIt);
        }
        else
        {
            ++lineIt;
        }
    }
    
    

    auto boxIt = boxes.begin();
    while(boxIt != boxes.end())
    {
        boxIt->lifetime -= dt;
        if(boxIt->lifetime <= 0.0f)
        {
            boxIt = boxes.erase(boxIt);
        }
        else
        {
            ++boxIt;
        }
    }
    
    auto iter = spheres.begin();
    while(iter != spheres.end())
    {
        iter->lifetime -= dt;
        if(iter->lifetime <= 0.0f)
        {
            iter->obj->kill();
            iter = spheres.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void ElysiumEngine::DebugDrawRenderable::handleMessage(ElysiumEngine::DrawDebugSphere &msg)
{
    Sphere s;
    s.com = msg.center;
    s.color = msg.color;
    s.lifetime = msg.lifetime;
    
    s.obj = GameObjectFactory::g_GameObjectFactory->createEmptyGameObject();
    
    Transform *t = new Transform();
    t->SetPosition(msg.center);
	if (msg.radius >= 0.0f)
		t->SetScale(2.0f * msg.radius, 2.0f * msg.radius, 2.0f * msg.radius);
	else
		t->SetScale(2.0f * msg.extents);

    MeshRenderable *renderable = new MeshRenderable();
    renderable->setMeshFile("unitsphere");
    renderable->setDynamic(false);
    renderable->setShader("BasicPhong");
    renderable->setColor(msg.color);
    renderable->setWireFrame(true);
    s.obj->addComponent(t);
    s.obj->addComponent(renderable);
    s.obj->initialize();
    s.obj->lateInitialize();

    spheres.push_back(s);
}

void ElysiumEngine::DebugDrawRenderable::handleMessage(DrawDebugBox &msg)
{
    Box b;
    b.com = msg.com;
    b.lifetime = msg.lifetime;
    b.color = msg.color;
    b.halfExtents = msg.halfExtents;
    
    boxes.push_back(b);
}

void ElysiumEngine::DebugDrawRenderable::handleMessage(ElysiumEngine::DrawDebugPoint &msg)
{
    Point p;
    p.com = msg.position;
    p.color = msg.color;
    p.lifetime = msg.lifetime;
    
    points.push_back(p);
}

void ElysiumEngine::DebugDrawRenderable::handleMessage(ElysiumEngine::DrawDebugLine &msg)
{
    Line l;
    l.begin = msg.start;
    l.end = msg.end;
    l.color = msg.color;
    l.lifetime = msg.lifetime;
    lines.push_back(l);
}
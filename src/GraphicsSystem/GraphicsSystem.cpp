#include "GraphicsSystem.h"
#include "GraphicsDebug.h"
#include "OpenGLHeaders.hpp"
#include "ICamera.h"
#include "DebugDraw.h"

#include <assert.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

ElysiumEngine::GraphicsSystem *ElysiumEngine::GraphicsSystem::g_GraphicsSystem = nullptr;

glm::mat4x4 proj;
glm::mat4x4 mv;

void ElysiumEngine::GraphicsSystem::update(float dt)
{
    if(!window->isOpen())
        return;

    debugDraw->update(dt);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
    float ratio = (float)window->getResX()/window->getResY();
    
    Matrix projection = BuildProjectionMatrix(45, ratio, 1.0f, 1000.0f);
    proj = glm::perspective(45.0f, ratio, 1.0f, 1000.0f);
    stack.setProjection(projection);
	
    Matrix view;
    if(!camera)
    {
        view = BuildCamera(Vec4(0,1,0),Vec4(0,0,0),Vec4(0,1,5));
    }
    else
    {
        Vec4 eye = camera->getEye();
        Vec4 lookat = camera->getLookAt();
        view = BuildCamera(Vec4(0,1,0), lookat, eye);
        
        glm::vec3 eyeG(eye.x,eye.y,eye.z);
        glm::vec3 lookatG(lookat.x,lookat.y,lookat.z);
        glm::vec3 up(0.0f,1.0f,0.0f);
        
        mv = glm::lookAt(eyeG,lookatG,up);
    }
    stack.setView(view);
    
    
    //Should utilize uniform blocks to setup lighting once
    for(auto renderable : renderables)
    {
        renderable->render();
    }
}

ElysiumEngine::GraphicsSystem::GraphicsSystem(IWindow *window) : ISystem("GraphicsSytstem"), window(window)
{
    assert(g_GraphicsSystem == nullptr && "Can only have a single graphics system");

    // Set the viewport to be the entire window
	GLCall(glViewport(0, 0, window->getResX(), window->getResY()));
    
    //glEnable(GL_CULL_FACE);
    g_GraphicsSystem = this;
}

void ElysiumEngine::GraphicsSystem::initialize()
{
    debugDraw = new DebugDrawRenderable();
    debugDraw->lateInitialize();
    
    //renderables.push_back(debugDraw);
    
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
    glEnable (GL_DEPTH_TEST);
}

void ElysiumEngine::GraphicsSystem::addRenderable(ElysiumEngine::IRenderable *renderable)
{
    if(renderable)
        renderables.push_back(renderable);
}

void ElysiumEngine::GraphicsSystem::removeRenderable(ElysiumEngine::IRenderable *renderable)
{
    renderables.remove(renderable);
}

ElysiumEngine::ShaderFactory &ElysiumEngine::GraphicsSystem::getShaderFactory()
{
    return shaderFactory;
}

void ElysiumEngine::GraphicsSystem::handleMessage(ElysiumEngine::WindowResizedMessage &msg)
{
    glViewport(0,0,msg.newX,msg.newY);
}

void ElysiumEngine::GraphicsSystem::addCamera(ElysiumEngine::ICamera *camera)
{
    this->camera = camera;
}

void ElysiumEngine::GraphicsSystem::removeCamera(ElysiumEngine::ICamera *camera)
{
    if(this->camera == camera)
    {
        this->camera = nullptr;
    }
}

void ElysiumEngine::GraphicsSystem::addLight(Light *light)
{
    lights.push_back(light);
}

void ElysiumEngine::GraphicsSystem::removeLight(Light *light)
{
    lights.remove(light);
}

void ElysiumEngine::GraphicsSystem::sendToChildren(ElysiumEngine::IMessage *message)
{
    for(auto &renderable : renderables)
    {
        message->send(*renderable);
    }
}

ElysiumEngine::ModelLibrary &ElysiumEngine::GraphicsSystem::getModelLibrary()
{
    return modelLibrary;
}

ElysiumEngine::MaterialLibrary &ElysiumEngine::GraphicsSystem::getMaterialLibrary()
{
	return materialLibrary;
}

void ElysiumEngine::GraphicsSystem::setWireframe(bool wireframe)
{
    if(wireframe)
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    else
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
}
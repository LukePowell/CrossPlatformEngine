#include "IRenderable.h"
#include "GraphicsSystem.h"

ElysiumEngine::IRenderable::IRenderable(std::string name) : IComponent(name)
{
    GraphicsSystem::g_GraphicsSystem->addRenderable(this);
}

ElysiumEngine::IRenderable::~IRenderable()
{
    GraphicsSystem::g_GraphicsSystem->removeRenderable(this);
}

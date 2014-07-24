#include "Light.h"
#include "GraphicsSystem.h"

ElysiumEngine::Light::Light() : IComponent("Light")
{
    GraphicsSystem::g_GraphicsSystem->addLight(this);
}

ElysiumEngine::Light::~Light()
{
    GraphicsSystem::g_GraphicsSystem->removeLight(this);
}

void ElysiumEngine::Light::serialize(FileIO::IReader &reader)
{
    reader.GetFirstChild();
    while(reader.ReadNextChild())
    {
        if(reader.GetTag() == "diffuse")
        {
            reader.GetFloatValueAtr("r", &diffuse.x);
            reader.GetFloatValueAtr("g", &diffuse.y);
            reader.GetFloatValueAtr("b", &diffuse.z);
        }
        else if(reader.GetTag() == "specular")
        {
            reader.GetFloatValueAtr("r", &specular.x);
            reader.GetFloatValueAtr("g", &specular.y);
            reader.GetFloatValueAtr("b", &specular.z);
        }
        else if(reader.GetTag() == "ambient")
        {
            reader.GetFloatValueAtr("r", &ambient.x);
            reader.GetFloatValueAtr("g", &ambient.y);
            reader.GetFloatValueAtr("b", &ambient.z);
        }
    }
    reader.GetParent();
}

void ElysiumEngine::Light::lateInitialize()
{
    trans = getSibling<Transform>("Transform");
}
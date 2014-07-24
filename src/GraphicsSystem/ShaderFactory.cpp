#include "ShaderFactory.h"
#include "XMLReader.h"
#include <assert.h>
#include <iostream>

ElysiumEngine::IShader *ElysiumEngine::ShaderFactory::createShader(const char *filename)
{
    IShader *newShader = nullptr;
    FileIO::XMLReader *reader = new FileIO::XMLReader();
    
    if(!reader->Open(filename))
    {
        //Log Error
        assert(0 && "Could not open shader file");
        return nullptr;
    }
    
    std::string type = "";
    std::string ID = "";
    
    reader->GetStringValueAtr("type",&type);
    reader->GetStringValueAtr("ID",&ID);
    
    if(type.empty() || ID.empty())
    {
        //TODO: Log Error
        return newShader;
    }
    
    newShader = shaderAllocators[type]->allocate();
        
    newShader->serialize(*reader);
    
    if(!newShader->create())
    {
        delete newShader;
        newShader = nullptr;
    }
    else
    {
        shaders[ID] = newShader;
    }
    
    reader->Close();
    return newShader;
}

void ElysiumEngine::ShaderFactory::registerShader(std::string name,ElysiumEngine::IShaderAllocator *allocator)
{
    shaderAllocators[name] = allocator;
}

ElysiumEngine::IShader *ElysiumEngine::ShaderFactory::getShader(std::string id) const
{
    auto shader = shaders.find(id);
    if(shader == shaders.end())
    {
        assert("Shader does not exist");
        return nullptr;
    }
    else
    {
        return shader->second;
    }
}

void ElysiumEngine::ShaderFactory::loadShaders(const char *filename)
{
    FileIO::XMLReader *reader = new FileIO::XMLReader();
    
    if(!reader->Open(filename))
    {
        //Log Error
        assert(0 && "Could not open shader file");
        return;
    }
    
    reader->GetFirstChild();
    while(reader->ReadNextChild())
    {
        if(reader->GetTag() == "file")
        {
            createShader(reader->GetText().c_str());
        }
    }
}
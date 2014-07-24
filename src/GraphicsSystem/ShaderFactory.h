#pragma once
#include "IShader.h"
#include "IReader.h"
#include "TemplateConstraints.h"
#include <string>
#include <map>
#include <memory>
#include <unordered_map>

namespace ElysiumEngine
{
#ifdef ANDROID
#pragma message("    typedef std::tr1::unordered_map<std::string, IShader *> ShaderMap;")
    typedef std::tr1::unordered_map<std::string, IShader *> ShaderMap;
#else
#pragma message("typedef std::unordered_map<std::string, IShader *> ShaderMap;")
    typedef std::unordered_map<std::string, IShader *> ShaderMap;
#endif
    
    class IShaderAllocator
    {
    public:
        virtual IShader* allocate() = 0;
        virtual ~IShaderAllocator(){}
    private:
    };
    
    template<typename T>
    class TShaderAllocator : public IShaderAllocator
    {
    public:
        IShader* allocate()
        {
            Derives<T,IShader>();
            T* newShader = new T();
            return newShader;
        }
    };
    
    class ShaderFactory
    {
    public:
        //Use an xml to preload all the shaders needed
        void loadShaders(const char *filename);
        //void unloadShaders();
        IShader *getShader(std::string id) const;
        IShader *createShader(const char *filename);
        void registerShader(std::string name,IShaderAllocator *allocator);
    private:
        std::map<std::string, IShaderAllocator *> shaderAllocators;
        ShaderMap shaders;
    };
}
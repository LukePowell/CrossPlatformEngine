#pragma once
#include "IReader.h"
#include "ICamera.h"

namespace ElysiumEngine
{
    class Light;
    
    class ILightHandles
    {
        
    };
    
    class IShader
    {
    public:
        virtual void setMVP(void *) = 0;
        virtual void setIMVP(void *) = 0;
        virtual void setCamera(ICamera *cam) = 0;
        
        virtual unsigned int numberOfPasses() = 0;
        virtual void startPass(unsigned int pass) = 0;
        virtual void endPass() = 0;
        virtual ~IShader(){}
        virtual void serialize(FileIO::IReader &reader) = 0;
        virtual bool create() = 0;
        
        virtual void setUniformVert4Data(std::string name, float *data) = 0;
        virtual void setUniformVert3Data(float *data){};
        
        virtual void setVaryingVert4Data(std::string name, float *data, int stride){};
        virtual void setVaryingVert3Data(std::string name, float *data){};
     
        virtual void setVaryingMat4(float *data, bool transpose){}
        virtual void setUniformMat4(std::string name, float *data, bool transpose){}
        
        virtual void applyLights(std::list<Light *> &apply) = 0;
    private:
    };
    
}
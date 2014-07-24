#pragma once
#include "IShader.h"
#include "IReader.h"
#include "GLSLCompiler.h"

#include <string>
#include <vector>
#include <map>

namespace ElysiumEngine
{
    enum ShaderIndices
    {
        VERTICES = 1,
        NORMALS,
        TEXTURE,
        TOTAL
    };
    
    struct LightHandle
    {
        unsigned int postion;
        unsigned int direction;
        unsigned int type;
        
        unsigned int diffuse;
        unsigned int specular;
        unsigned int ambient;
    };
    
    class GLSLShader : public IShader, public FileIO::IReaderVistor
    {
    public:
        GLSLShader();
    	void setMVP(void *){}
    	void setIMVP(void *){}
        void setCamera(ICamera *cam);
    	unsigned int numberOfPasses();
    	void startPass(unsigned int pass);
        void endPass();
    	void serialize(FileIO::IReader &writer);
    	bool create();
        
        
        void applyLights(std::list<Light *> &lights);
        
        void setUniformMat4(std::string name, float *data, bool transpose);
        void setUniformVert4Data(std::string name, float *data);
        
        void setVaryingVert4Data(std::string name, float *data, int stride);
    
    private:
        unsigned int numPasses;
        unsigned int nextHandle;
        unsigned int currentPass;
        
        std::string vertexShader;
        std::string fragmentShader;
        std::vector<std::string> utilityShaders;
        
        std::vector<GLSLShader> passes;
        
        std::vector<LightHandle> lights;
        
        //Light names
        std::string lightDiffuse;
        std::string lightAmbient;
        std::string lightSpecular;
        std::string lightPosition;
        std::string lightDirection;
        std::string lightType;
        std::string numLights;
        
        unsigned int shaderProgram;
        
        
        std::string vertexAttribute;
        std::string normalAttribute;
        std::string textureAttribute;
        std::string camPos;
        
        unsigned int cameraPosHandle;
        
        std::map<std::string, unsigned int> m_OtherHandles;
        
        
        static GLSLCompiler compiler;
        
        friend class GLSLCompiler;
    };
}
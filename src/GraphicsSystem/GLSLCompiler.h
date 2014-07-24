#pragma once
#include "OpenGLHeaders.hpp"


#include <vector>
#include <string>
#include <map>

namespace ElysiumEngine
{
    class GLSLShader;
    
    class GLSLCompiler
    {
    public:
        void loadStringToFile(const char *filename, char* &output);
        bool compile(GLSLShader *shader);
        unsigned int create(std::string vertexShader, std::string fragmentShader, std::vector<std::string> &utilityShaders);
    private:
    };
}
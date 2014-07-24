#include "GLSLShader.h"
#include "GraphicsDebug.h"
#include <iostream>
#include "Light.h"

ElysiumEngine::GLSLCompiler ElysiumEngine::GLSLShader::compiler;

ElysiumEngine::GLSLShader::GLSLShader() : numPasses(1), nextHandle(TOTAL)
{
    currentPass = -1;
}

void ElysiumEngine::GLSLShader::startPass(unsigned int pass)
{
    if(passes[pass].shaderProgram != -1)
    {
        GLCall(glUseProgram(passes[pass].shaderProgram));
        currentPass = pass;
    }
}

void ElysiumEngine::GLSLShader::endPass()
{
    GLCall(glUseProgram(0));
    currentPass = -1;
}
unsigned int ElysiumEngine::GLSLShader::numberOfPasses()
{
    return numPasses;
}

void ElysiumEngine::GLSLShader::setCamera(ElysiumEngine::ICamera *cam)
{
    //GLCall(glUniform3fv(cameraPosHandle,1,cam->getEye().v));
}

void ElysiumEngine::GLSLShader::serialize(FileIO::IReader &reader)
{
    reader.GetUnsignedIntAtr("numberOfPasses",&numPasses);
    passes.resize(numPasses);
    
    reader.GetFirstChild();
    while(reader.ReadNextChild())
    {
        if(reader.GetTag() == "pass")
        {
            unsigned int pass = 0;
            reader.GetUnsignedIntAtr("num", &pass);
            reader.GetFirstChild();
            while(reader.ReadNextChild())
            {
                //std::cout << reader.GetTag() << std::endl;
                if(reader.GetTag() == "vertexShader")
                {
                    reader.GetFirstChild();
                    while(reader.ReadNextChild())
                    {
                        //std::cout << "\t" << reader.GetTag() << std::endl;
                        if(reader.GetTag() == "file")
                        {
                            passes[pass].vertexShader = reader.GetText();
                        }
                    }
                    reader.GetParent();
                }
                else if(reader.GetTag() == "fragmentShader")
                {
                    reader.GetFirstChild();
                    while(reader.ReadNextChild())
                    {
                        //std::cout << "\t" << reader.GetTag() << std::endl;
                        if(reader.GetTag() == "file")
                        {
                            passes[pass].fragmentShader = reader.GetText();   
                        }
                    }
                    reader.GetParent();
                }
                else if(reader.GetTag() == "utilityShader")
                {
                    reader.GetFirstChild();
                    while(reader.ReadNextChild())
                    {
                        //std::cout << "\t" << reader.GetTag() << std::endl;
                        passes[pass].utilityShaders.push_back(reader.GetText());
                    }
                    reader.GetParent();
                }
                else if(reader.GetTag() == "uniforms")
                {
                    reader.GetFirstChild();
                    while(reader.ReadNextChild())
                    {
                        if(reader.GetTag() == "other")
                        {
                            
                        }
                        else if(reader.GetTag() == "camera")
                        {
                            reader.GetFirstChild();
                            while(reader.ReadNextChild())
                            {
                                if(reader.GetTag() == "position")
                                {
                                    passes[pass].camPos = reader.GetText();
                                }
                            }
                            reader.GetParent();
                        }
                        else if(reader.GetTag() == "light")
                        {
                            int num = 0;
                            reader.GetIntValueAtr("num", &num);
                            passes[pass].lights.resize(num);
                            reader.GetFirstChild();
                            while(reader.ReadNextChild())
                            {
                                if(reader.GetTag() == "position")
                                {
                                    passes[pass].lightPosition = reader.GetText();
                                }
                                else if(reader.GetTag() == "diffuse")
                                {
                                    passes[pass].lightDiffuse = reader.GetText();
                                }
                                else if(reader.GetTag() == "ambient")
                                {
                                    passes[pass].lightAmbient = reader.GetText();
                                }
                                else if(reader.GetTag() == "specular")
                                {
                                    passes[pass].lightSpecular = reader.GetText();
                                }
                                else if(reader.GetTag() == "direction")
                                {
                                    passes[pass].lightDirection = reader.GetText();
                                }
                                else if(reader.GetTag() == "type")
                                {
                                    passes[pass].lightType = reader.GetText();
                                }
                                else if(reader.GetTag() == "number")
                                {
                                    passes[pass].numLights = reader.GetText();
                                }
                            }
                            reader.GetParent();
                        }
                    }
                    reader.GetParent();
                }
            }
            reader.GetParent();
        }
    }
    reader.GetParent();
    
    
}

bool ElysiumEngine::GLSLShader::create()
{
    bool allCompiled = true;//If a single pass does not compile return an error
    for(int i = 0; i < passes.size(); ++i)
    {
        allCompiled = allCompiled && compiler.compile(&passes[i]);
    }
    return allCompiled;
}

void ElysiumEngine::GLSLShader::applyLights(std::list<Light *> &apply)
{
    
    if(apply.size() == 0 || passes[currentPass].lights.empty())
        return;

    int i = 0;
    
    for(auto light : apply)
    {
        GLCall(glUniform4fv(passes[currentPass].lights[i].diffuse,1,light->getDiffuse().v));
        GLCall(glUniform4fv(passes[currentPass].lights[i].specular,1,light->getSpecular().v));
        GLCall(glUniform4fv(passes[currentPass].lights[i].ambient,1,light->getAmbient().v));
        GLCall(glUniform3fv(passes[currentPass].lights[i].postion, 1, light->getPosition().v));
        ++i;
    }
}

void ElysiumEngine::GLSLShader::setVaryingVert4Data(std::string name, float *data, int stride)
{
    int temp = glGetAttribLocation(passes[currentPass].shaderProgram, name.c_str());
    glEnableVertexAttribArray(temp);
    glVertexAttribPointer(temp, 3, GL_FLOAT, false, stride, data);
}

void ElysiumEngine::GLSLShader::setUniformMat4(std::string name, float *data, bool transpose)
{
    int temp = glGetUniformLocation(passes[currentPass].shaderProgram,name.c_str());
    glUniformMatrix4fv(temp, 1, transpose, data);
    
}

void ElysiumEngine::GLSLShader::setUniformVert4Data(std::string name, float *data)
{
    int temp = glGetUniformLocation(passes[currentPass].shaderProgram,name.c_str());
    glUniform4fv(temp, 1, data);
}
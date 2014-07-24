#include "GLSLShader.h"
#include "GLSLCompiler.h"
#include "GraphicsDebug.h"
#include <sstream>
//////////////
//REMOVE LATER
#include <iostream>
//REMOVE LATER
//////////////

//TODO: Add in error handling
unsigned int ElysiumEngine::GLSLCompiler::create(std::string vertexShader, std::string fragmentShader, std::vector<std::string> &utilityShaders)
{
    //Create the two main shaders
    int vertex = -1;
    GLCall(vertex = glCreateShader(GL_VERTEX_SHADER));
    int fragment = -1;
    GLCall(fragment = glCreateShader(GL_FRAGMENT_SHADER));
    
    char *vertexText = nullptr;
    char *fragmentText = nullptr;
    char **utilityTexts;
    
    loadStringToFile(vertexShader.c_str(),vertexText);
    loadStringToFile(fragmentShader.c_str(), fragmentText);
    
    utilityTexts = new char*[utilityShaders.size() + 1];
    
    for(int i = 0; i < utilityShaders.size(); ++i)
    {
        loadStringToFile(utilityShaders[i].c_str(), utilityTexts[i]);
    }
    
    utilityTexts[utilityShaders.size()] = vertexText;
    GLCall(glShaderSource(vertex,1+utilityShaders.size(),const_cast<const char**>(utilityTexts),nullptr));
    GLCall(glCompileShader(vertex));
    int CompiledFlag = 0;
	GLCall(glGetShaderiv(vertex,GL_COMPILE_STATUS,&CompiledFlag));
	if(!CompiledFlag)
	{
		std::string Type;
        Type = "Vertex Shader";
        
		int LogLength = 0;
		GLsizei StringLength = 0;
    
		//Get the length needed to store the log
		GLCall(glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &LogLength));
		char *Log = new char[LogLength];
		GLCall(glGetShaderInfoLog(vertex,LogLength,&StringLength,Log));
		
		std::cout << "Could not compile " << Type << ": " << vertexShader
        << "\nLog:\n" << Log << std::endl;
		delete [] Log;
		return -1;
	}
    
    utilityTexts[utilityShaders.size()] = fragmentText;
    GLCall(glShaderSource(fragment,1+utilityShaders.size(),const_cast<const char **>(utilityTexts),nullptr));
    GLCall(glCompileShader(fragment));
    GLCall(glGetShaderiv(fragment,GL_COMPILE_STATUS,&CompiledFlag));
	if(!CompiledFlag)
	{
		std::string Type;
        Type = "Fragment Shader";
        
		int LogLength = 0;
		GLsizei StringLength = 0;
        
		//Get the length needed to store the log
		GLCall(glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &LogLength));
		char *Log = new char[LogLength];
		GLCall(glGetShaderInfoLog(fragment,LogLength,&StringLength,Log));
		
		std::cout << "Could not compile " << Type << ": " << fragmentShader
        << "\nLog:\n" << Log << std::endl;
		delete [] Log;
		return -1;
	}
    
    int program = glCreateProgram();
    
    GLCall(glAttachShader(program,vertex));
    GLCall(glAttachShader(program,fragment));
    
    //Free the char memory allocated by creating these shaders
    delete[] vertexText;
    delete[] fragmentText;
    
    for(int i = 0; i < utilityShaders.size(); ++i)
    {
        delete[] utilityTexts[i];
    }
    
    delete[] utilityTexts;
    
    return program;
}

bool ElysiumEngine::GLSLCompiler::compile(GLSLShader *shader)
{
    unsigned int program = create(shader->vertexShader,shader->fragmentShader,shader->utilityShaders);
    
    if(program == -1)
        return false;
    
    shader->shaderProgram = program;
    
    //Bind our locations these are using the standard enumerated values
    //located in GLSLShader.h other vertex attributes can be found later
    
    GLCall(glLinkProgram(shader->shaderProgram));
    
    GLint LinkFlag = 0;
	GLCall(glGetProgramiv(shader->shaderProgram,GL_LINK_STATUS,&LinkFlag));
	if(!LinkFlag)//Linking failed
	{
		int LogLength = 0;
		GLsizei StringLength = 0;
        
		GLCall(glGetProgramiv(shader->shaderProgram, GL_INFO_LOG_LENGTH, &LogLength));
		char *Log = new char[LogLength];
		GLCall(glGetProgramInfoLog(shader->shaderProgram,LogLength,&StringLength,Log));
        
		std::cout << "Could not link the specified program.\n" << Log << std::endl;
        
        return false;
	}
    
    //Bind up our uniforms
    
    std::stringstream str;
    for(int i = 0; i < shader->lights.size(); ++i)
    {
   		char Array[100];
        
		sprintf(Array,"uLightPositions[%i]",i);
		GLCall(shader->lights[i].postion = glGetUniformLocation(shader->shaderProgram,Array));
        
		sprintf(Array,"uLightsDiffuse[%i]",i);
		GLCall(shader->lights[i].diffuse = glGetUniformLocation(shader->shaderProgram,Array));
        
		sprintf(Array,"uLightsAmbient[%i]",i);
		GLCall(shader->lights[i].ambient = glGetUniformLocation(shader->shaderProgram,Array));
        
		sprintf(Array,"uLightsSpecular[%i]",i);
		GLCall(shader->lights[i].specular = glGetUniformLocation(shader->shaderProgram,Array));
        
		sprintf(Array,"uLightType[%i]",i);
		GLCall(shader->lights[i].type = glGetUniformLocation(shader->shaderProgram,Array));
        

		sprintf(Array,"uLightsDirection[%i]",i);
		GLCall(shader->lights[i].direction = glGetUniformLocation(shader->shaderProgram,Array));
    }
    
    return program != -1;
}

void ElysiumEngine::GLSLCompiler::loadStringToFile(const char *filename, char* &output)
{
    
	FILE *cFile = fopen(filename,"rb");
    
	if(!cFile)
	{
		std::cout << "Could not open file: " << filename << " to be read into string " << __FILE__ << std::endl;
        return;
	}
    
	fseek(cFile,0,SEEK_END);
	unsigned int fileSize = ftell(cFile);
	output = new char[fileSize+1];
	rewind(cFile);//Set the file back to the beginning
	fread(output,1,fileSize,cFile);
	output[fileSize] = '\0';

	//Clean up
	fclose(cFile);
}

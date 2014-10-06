#include "Mesh.h"
#include <cctype>
#include <cstdlib>
#include "GraphicsSystem.h"

void CalculateNormals(ElysiumEngine::Mesh *mesh);

ElysiumEngine::Mesh *ElysiumEngine::loadMeshFromBin(const char *filename)
{
    FILE *file = fopen(filename,"rb");
    if(!file)
        return nullptr;
    
    Mesh *output = new Mesh();
    fread(&output->vertexCount,sizeof(unsigned int),1,file);
    fread(&output->indexCount,sizeof(unsigned int),1,file);
    
    output->vertices = new VertexData[output->vertexCount];
    output->indices = new unsigned int[3 * output->indexCount];
    
    //Read in our vetex data
    fread(output->vertices,sizeof(VertexData),output->vertexCount,file);
    //Read in our index data
    fread(output->indices,sizeof(unsigned int),output->indexCount*3,file);
    
    fclose(file);
    
    return output;
}

void getObjSize(FILE *file, unsigned int &vertexCount, unsigned int &indexCount)
{
    char buffer[1000];
    while(!feof(file))
    {
        if(!fgets(buffer,1000,file))
            return;
        
        if(strstr(buffer,"#"))
            continue;
        else if(strstr(buffer,"vt"))
            continue;
        else if(strstr(buffer,"vn"))
            continue;
        else if(strstr(buffer,"v"))
        {
            vertexCount++;
        }
        else if(strstr(buffer,"f"))
        {
            indexCount++;
        }
    }
}

void print(ElysiumEngine::Mesh *mesh)
{
    std::cout << "Vertices:\n";
    for(int i = 0; i  < mesh->vertexCount; ++i)
    {
        std::cout << mesh->vertices[i].position << std::endl;
    }
    std::cout << "Normals:\n";
    for(int i = 0; i < mesh->vertexCount; ++i)
    {
        std::cout << mesh->vertices[i].normal << std::endl;
    }
    std::cout << "Indices:\n";
    for(int i = 0; i < mesh->indexCount * 3; i += 3)
    {
        std::cout << mesh->indices[i] << " " << mesh->indices[i+1] << " " << mesh->indices[i+2] << std::endl;
    }
}

void customTokenize(char *string, std::vector<std::string> &output)
{
    size_t length = strlen(string);
    
    size_t i = 0;
    
    while(i < length)
    {
        std::string temp;
        while(!isspace(string[i]))
        {
            if(string[i] == '#')
                return;//Ignore coments
            
            temp += string[i];
            ++i;
        }
        
        if(temp.size() > 0)
            output.push_back(temp);
        
        if(i < length)
            ++i;
    }
}

void customTokenize(const char *string, std::vector<std::string> &output, char delim)
{
	size_t Length = strlen(string);
	size_t i = 0;
	while(i < Length)
	{
		std::string Temp;
		while(string[i] != delim && i < Length)
		{
			Temp += string[i];
			i++;
		}
        
		if(Temp.size() > 0)
			output.push_back(Temp);
        
		if(i < Length)
			i++;
	}
}

int countInstanceOf(const std::string &string, char letter)
{
    int count = 0;
    for(auto c : string)
    {
        if(c == letter)
        {
            ++count;
        }
    }
    return count;
}


void parseOBJ(ElysiumEngine::Mesh *output, FILE *input)
{
    rewind(input);
    
    std::vector<std::string> tokens;
    
    
    //Tempoary storage to hold vertex data before duplication
    std::vector<Vec4> vertices;
    std::vector<Vec4> indices;//0: position, 1: normal, 2: texture
    std::vector<Vec4> normals;
    std::vector<Vec4> texturecoords;
    
    //0: position, 1: normal, 2: texture, 3: actual
   // std::vector<std::tuple<int,int,int,int>> usedIndices;
    
    std::vector<ElysiumEngine::VertexData> final;
    
    int vertexIndex = 0;
    int normalIndex = 0;
    int faceIndex = 0;
    int textureIndex = 0;

    char buffer[1000];
    
	ElysiumEngine::Material *current = nullptr;
    while(!feof(input))
    {
        tokens.clear();
        
        if(!fgets(buffer,1000,input))
        {
            break;
        }
        
        if(buffer[0] == '#')
            continue;

        customTokenize(buffer, tokens);
        
        if(tokens.empty())
            continue;
        

		if (tokens[0] == "mtllib")
		{
			ElysiumEngine::GraphicsSystem::g_GraphicsSystem->getMaterialLibrary().loadMaterialsFromFile(tokens[1]);
			//TODO: Additionally apply the current material to vertices as specifed
			continue;
		}

        if(tokens[0] == "v")
        {
            if(tokens.size() < 4)
            {
                std::cout << "Error each vertex must have a least 3 elements: " << buffer;
                continue;
            }
            
            output->vertices[vertexIndex].position.x = std::atof(tokens[1].c_str());
            output->vertices[vertexIndex].position.y = std::atof(tokens[2].c_str());
            output->vertices[vertexIndex].position.z = std::atof(tokens[3].c_str());
            output->vertices[vertexIndex].position.w = 0.0f;
            
            vertices.push_back(Vec4(std::atof(tokens[1].c_str()),std::atof(tokens[2].c_str()),std::atof(tokens[3].c_str())));
            
            vertexIndex++;
        }
        else if(tokens[0] == "vt")
        {
            if(tokens.size() < 3)
            {
                std::cout << "Error texture coords must have at least 2 elements" << buffer;
                continue;
            }
            
           // output->vertices[textureIndex].texture[0] = std::atof(tokens[1].c_str());
           // output->vertices[textureIndex].texture[1] = std::atof(tokens[2].c_str());
            
            texturecoords.push_back(Vec4(std::atof(tokens[1].c_str()),std::atof(tokens[2].c_str()),0));
            textureIndex++;
        }
        else if(tokens[0] == "vn")
        {
            if(tokens.size() < 4)
            {
                std::cout << "Error nomal must have at least 3 elements: " << buffer;
                continue;
            }
            
            output->vertices[normalIndex].normal.x = std::atof(tokens[1].c_str());
            output->vertices[normalIndex].normal.y = std::atof(tokens[2].c_str());
            output->vertices[normalIndex].normal.z = std::atof(tokens[3].c_str());
            
            
            normals.push_back(Vec4(std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str())));
            
            normalIndex++;
        }
        else if(tokens[0] == "f")
        {
            if(tokens.size() < 4)
            {
                std::cout << "Invalid face: " << buffer;
                continue;
                
            }
            
            int count = countInstanceOf(tokens[1], '/');
            

            
            std::vector<std::string> subTokens;
            
            if(count == 0)
            {
                indices.push_back(Vec4(std::atof(tokens[1].c_str()),0,0));
                indices.push_back(Vec4(std::atof(tokens[2].c_str()),0,0));
                indices.push_back(Vec4(std::atof(tokens[3].c_str()),0,0));
            }
            else if(count == 1)
            {
                customTokenize(tokens[1].c_str(), subTokens, '/');
                indices.push_back(Vec4(std::atof(subTokens[0].c_str()),std::atof(subTokens[1].c_str()),0));
                subTokens.clear();
                customTokenize(tokens[2].c_str(), subTokens, '/');
                indices.push_back(Vec4(std::atof(subTokens[0].c_str()),std::atof(subTokens[1].c_str()),0));
                subTokens.clear();
                customTokenize(tokens[3].c_str(), subTokens, '/');
                indices.push_back(Vec4(std::atof(subTokens[0].c_str()),std::atof(subTokens[1].c_str()),0));

            }
            else if(count == 2)
            {
                
            }
            output->indices[faceIndex] = std::atof(tokens[1].c_str())-1;
            output->indices[faceIndex+1] = std::atof(tokens[2].c_str())-1;
            output->indices[faceIndex+2] = std::atof(tokens[3].c_str())-1;
            
			if (current)
			{
				int one = output->indices[faceIndex];
				int two = output->indices[faceIndex + 1];
				int three = output->indices[faceIndex + 2];
				
				output->vertices[one].Ka = current->Ka;
				output->vertices[one].Ks = current->Ks;
				output->vertices[one].Ns = current->Ns;
				output->vertices[one].color = current->Kd;

				output->vertices[two].Ka = current->Ka;
				output->vertices[two].Ks = current->Ks;
				output->vertices[two].Ns = current->Ns;
				output->vertices[two].color = current->Kd;

				output->vertices[three].Ka = current->Ka;
				output->vertices[three].Ks = current->Ks;
				output->vertices[three].Ns = current->Ns;
				output->vertices[three].color = current->Kd;
			}
            
            faceIndex += 3;
        }
		else if (tokens[0] == "usemtl")
		{
			current = ElysiumEngine::GraphicsSystem::g_GraphicsSystem->getMaterialLibrary().getMaterial(tokens[1]);
		}
    }
    CalculateNormals(output);
}

ElysiumEngine::Mesh *ElysiumEngine::loadMeshFromObj(const char *filename)
{
    FILE *file = fopen(filename,"rt");
    if(!file)
        return nullptr;
    
    Mesh *output = new Mesh();
    
    getObjSize(file,output->vertexCount,output->indexCount);
    output->vertices = new VertexData[output->vertexCount];
    output->indices = new unsigned int[3 * output->indexCount];
    
    parseOBJ(output,file);
    
    fclose(file);
    
    //print(output);
    
    return output;
}

void ElysiumEngine::saveObjToBin(const char *filename, const char *outputFile)
{
    
    Mesh *mesh = loadMeshFromObj(filename);
    
    FILE *output = fopen(outputFile,"wb");
    
    fwrite(&mesh->vertexCount, sizeof(unsigned int),1,output);
    fwrite(&mesh->indexCount, sizeof(unsigned int),1, output);
    
    fwrite(mesh->vertices,sizeof(VertexData),mesh->vertexCount,output);
    fwrite(mesh->indices,sizeof(unsigned int), mesh->indexCount * 3,output);
    
    fclose(output);
}
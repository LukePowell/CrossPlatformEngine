#pragma once
#include "IRenderable.h"
#include <vector>

namespace ElysiumEngine
{
    struct Mesh;
    class VertexData;
    
    Mesh *loadMeshFromObj(const char *filename);
    Mesh *loadMeshFromBin(const char *filename);
    void saveObjToBin(const char *filename, const char *outputFile);//removes .obj and replaces it with .bin
    
    struct Mesh
    {
        unsigned int vertexCount;
        unsigned int indexCount;
        
        VertexData* vertices;
        unsigned int *indices;
    };
}

void customTokenize(char *string, std::vector<std::string> &output);

#pragma once
#include "IRenderable.h"
#include "IShader.h"
#include "Transform.h"
#include "Mesh.h"
#include "HalfEdgeMesh.h"

namespace ElysiumEngine
{
	class MeshRenderable : public IRenderable
	{
    public:
        
        MeshRenderable(const MeshRenderable& meshRenderable);
        
        MeshRenderable();
        ~MeshRenderable();
        
        void render();

        void serialize(FileIO::IReader &reader);
        void deserialize(){}

        void initialize();//Create the mesh
        void lateInitialize();//Grab the transform for convinence
        
        HalfEdgeMesh *getHalfEdge(){return halfEdge;}
        Mesh *getMesh(){return mesh;}
        
        void setHalfEdgeMesh(HalfEdgeMesh *newHalfEdgeMesh);
        void setMesh(Mesh *newMesh);
        
        
        void setShader(std::string id){shaderID = id;};
        void setDynamic(bool isDynamic){dynamic = isDynamic;}
        void setMeshFile(std::string mesh){meshFile = mesh;}
        void setWireFrame(bool wireframe);
        void setColor(Vec4 color){this->color = color;}
    private:

        IShader* shader;

        Transform *transform;
        Mesh *mesh;
        HalfEdgeMesh *halfEdge;

        unsigned int vao;
        unsigned int buffer;
        unsigned int indexBuffer;
        
        bool dynamic;
        bool wireframe;
        Vec4 color;
        std::string shaderID;
        std::string meshFile;
	};
}
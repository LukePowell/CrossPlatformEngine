#include "MeshRenderable.h"
#include "GraphicsSystem.h"
#include "GraphicsDebug.h"
#include "OpenGLHeaders.hpp"
#include "glm/Matrix.hpp"

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

void CalculateNormals(ElysiumEngine::Mesh *mesh);

extern glm::mat4x4 proj;
extern glm::mat4x4 mv;

ElysiumEngine::MeshRenderable::MeshRenderable() : IRenderable("MeshRenderable")
{
    transform = nullptr;
    shader = nullptr;
    dynamic = false;
    mesh = nullptr;
    halfEdge = nullptr;
	wireframe = false;
	color = Vec4(1.0f,1.0f,1.0f,1.0f);
}

ElysiumEngine::MeshRenderable::MeshRenderable(const MeshRenderable& meshRenderable) : IRenderable("MeshRenderable")
{
    dynamic = meshRenderable.dynamic;
    shader = meshRenderable.shader;
    shaderID = meshRenderable.shaderID;
	color = meshRenderable.color;
	wireframe = meshRenderable.wireframe;
}

ElysiumEngine::MeshRenderable::~MeshRenderable()
{
    GraphicsSystem::g_GraphicsSystem->removeRenderable(this);
}

float *verts;
float other[] = {1.0000, 1.0000, 1.0000,
      -1.0000 ,1.0000 ,1.0000,
      -1.0000, -1.0000, 1.00002,
      1.0000 ,-1.0000, 1.0000,
      1.0000 ,1.0000, -1.0000,
      1.0000 ,-1.0000, -1.0000,
      -1.0000, 1.0000, -1.0000,
      -1.0000, -1.0000, -1.0000};

void ElysiumEngine::MeshRenderable::render()
{
    if(!shader)
        return;
    
    Matrix foo = transform->GetOrientation().QuaternionToMatrix();

    float mat[16] = {foo.m00,foo.m10,foo.m20,foo.m30,
                     foo.m01,foo.m11,foo.m21,foo.m31,
                     foo.m02,foo.m12,foo.m22,foo.m32,
                     foo.m03,foo.m13,foo.m23,foo.m33};
    
    for(int i = 0; i < shader->numberOfPasses(); ++i)
    {
        shader->startPass(i);
        
        RenderMatrixStack &stack = GraphicsSystem::g_GraphicsSystem->getRenderMatrixStack();
        stack.pushState();
        
        Matrix trans = BuildTranslationMatrix(transform->GetPosition());
        Matrix scale = BuildScaleMatrix(transform->GetScale().x, transform->GetScale().y, transform->GetScale().z);
        
        stack.pushMatrix(BuildTranslationMatrix(transform->GetPosition()));
        stack.pushMatrix(BuildScaleMatrix(transform->GetScale().x,transform->GetScale().y,transform->GetScale().z));
        stack.pushMatrix(foo);
        
        
        Matrix view = stack.getView();
        Matrix proj2 = stack.getProjection();
        
        if(wireframe)
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        }
            
        shader->setCamera(GraphicsSystem::g_GraphicsSystem->getCamera());
        std::list<Light *>& lights = GraphicsSystem::g_GraphicsSystem->getLights();
        shader->applyLights(lights);
        shader->setUniformMat4("uTestTest",glm::value_ptr(mv),false);
        shader->setUniformVert4Data("uDiffuse",color.v);
            
        Matrix m = stack.getTop();
        //m = m.transpose();
        
        shader->setUniformMat4("uMVP",m.v,true);
        
        
        //view.transpose();
        
        float *check1 = glm::value_ptr(mv);
        float *check2 = view.v;
        
        /*for(int i = 0; i < 16; ++i)
        {
            std::cout << "i: " << i << " " << check1[i] << " : " << check2[i] << std::endl;
        }*/
        
        proj2 = proj2.transpose();
        
        /*for(int i = 0; i < 16; ++i)
        {
            std::cout << "i: " << i << " " << check1[i] << " : " << check2[i] << std::endl;
        }*/
        
        shader->setUniformMat4("uProjection",proj2.v, true);
        shader->setUniformMat4("uModelView",glm::value_ptr(mv), false);
        
        shader->setUniformMat4("trans", trans.v, true);
        shader->setUniformMat4("scale", scale.v, true);
        
        glm::mat4x4 normal = mv;
        normal = glm::inverse(normal);
        shader->setUniformMat4("uNormalMatrix", glm::value_ptr(normal), true);
        
        GLCall(glBindVertexArray(vao));
        GLCall(glDrawElements(GL_TRIANGLES,3 * mesh->indexCount, GL_UNSIGNED_INT, 0));
        GLCall(glBindVertexArray(0));


        shader->endPass();
            
        if(wireframe)
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
        
        stack.popState();
    }
}


void ElysiumEngine::MeshRenderable::serialize(FileIO::IReader &reader)
{
    reader.GetFirstChild();
    while(reader.ReadNextChild())
    {
        if(reader.GetTag() == "file")
        {
            meshFile = reader.GetText();
        }
        else if(reader.GetTag() == "shader")
        {
            shaderID = reader.GetText();
        }
        else if (reader.GetTag() == "dynamic")
        {
            reader.GetBoolValueAttr("value", &dynamic);
        }
    }
    reader.GetParent();
}


void ElysiumEngine::MeshRenderable::initialize()
{
    //Load Mesh
    mesh = GraphicsSystem::g_GraphicsSystem->getModelLibrary().loadModel(meshFile, !dynamic);
    if(dynamic)
    {
        halfEdge = GraphicsSystem::g_GraphicsSystem->getModelLibrary().loadHalfEdgeMesh(meshFile);
    }
    
    shader = GraphicsSystem::g_GraphicsSystem->getShaderFactory().getShader(shaderID);
    
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));
    
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, mesh->vertexCount * 6 * sizeof(float), nullptr, GL_STATIC_DRAW));
    
    GLCall(glEnableVertexAttribArray(0));//Position
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, false, 6*sizeof(float),(float *)0));
    
    GLCall(glEnableVertexAttribArray(1));//Normal
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, false, 6*sizeof(float),(float *)(3*sizeof(float))));

    int indexVert = 0;
    int indexNorm = 3;
    for(int i = 0; i < mesh->vertexCount; ++i)
    {
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, indexVert * sizeof(float), 3 * sizeof(float), mesh->vertices[i].position.v));
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, indexNorm * sizeof(float), 3 * sizeof(float), mesh->vertices[i].normal.v));
        
        indexVert += 6;
        indexNorm += 6;
    }
    
    GLCall(glGenBuffers(1, &indexBuffer));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * mesh->indexCount, mesh->indices, GL_STATIC_DRAW));

    GLCall(glBindVertexArray(0));
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER,0));
}

void ElysiumEngine::MeshRenderable::lateInitialize()
{
    if(hasSibling("Transform"))
    {
        transform = getSibling<Transform>("Transform");
    }
}

void ElysiumEngine::MeshRenderable::setHalfEdgeMesh(HalfEdgeMesh *newHalfEdgeMesh)
{
    halfEdge = newHalfEdgeMesh;
    mesh = GraphicsSystem::g_GraphicsSystem->getModelLibrary().meshFromHalfEdge(halfEdge);
    
    GLCall(glGenBuffers(1, &buffer));
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    
    GLCall(glBufferData(GL_ARRAY_BUFFER, mesh->vertexCount * 6 * sizeof(float), nullptr, GL_STATIC_DRAW));
    
    
    int indexVert = 0;
    int indexNorm = 3;
    for(int i = 0; i < mesh->vertexCount; ++i)
    {
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, indexVert * sizeof(float), 3 * sizeof(float), mesh->vertices[i].position.v));
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, indexNorm * sizeof(float), 3 * sizeof(float), mesh->vertices[i].normal.v));
        
        indexVert += 6;
        indexNorm += 6;
    }
    
    /*GLCall(glGenBuffers(1, &indexBuffer));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * mesh->indexCount, mesh->indices, GL_STATIC_DRAW));*/
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER,0));
}

void ElysiumEngine::MeshRenderable::setMesh(Mesh *newMesh)
{
    mesh = newMesh;
}

void ElysiumEngine::MeshRenderable::setWireFrame(bool wireframe)
{
    this->wireframe = wireframe;
}

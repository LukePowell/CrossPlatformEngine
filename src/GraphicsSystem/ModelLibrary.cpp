#include "ModelLibrary.h"
#include "GraphicsSystem.h"
#include "Messaging.h"

void CalculateNormals(ElysiumEngine::Mesh *mesh);

static bool fexists(const char *name)
{
    FILE *exists = fopen(name,"r");
	if(exists)
		fclose(exists);
	return exists != nullptr;
}


ElysiumEngine::Mesh *ElysiumEngine::ModelLibrary::loadModel(std::string name, bool isStatic)
{
	std::string finalname = name + ".bin";
	bool saveBinary = false;

	if(!fexists(finalname.c_str()))
	{
		saveBinary = true;
		finalname = name + ".obj";
		if(!fexists(finalname.c_str()))
		{
			std::cout << "Cannot load " << finalname;
		}
	}

	if(isStatic)
	{
		auto mesh = meshes.find(name);
		if(mesh != meshes.end())
		{
			return mesh->second;
		}
		else
		{
			Mesh *mesh;
			if(saveBinary)//Loading an bin file
			{
                std::string output = name + ".bin";
				saveObjToBin(finalname.c_str(),output.c_str());
                finalname = name + ".bin";
			}	
			
			mesh = loadMeshFromBin(finalname.c_str());
            
            if(mesh != nullptr)
            {
                meshes[name] = mesh;
            }
            
            return mesh;
		}
	}
    else
    {
        Mesh *mesh;

        if(saveBinary)//Loading an bin file
        {
            std::string output = name + ".bin";
            saveObjToBin(finalname.c_str(),output.c_str());
            finalname = name + ".bin";
        }
			
        finalname = name + ".bin";
        mesh = loadMeshFromBin(finalname.c_str());
        return mesh;
    }
}

ElysiumEngine::HalfEdgeMesh *ElysiumEngine::ModelLibrary::loadHalfEdgeMesh(std::string name)
{
    std::string finalname = name + ".half";
	bool saveBinary = false;
    
	if(!fexists(finalname.c_str()))
	{
		saveBinary = true;
	}
    
    HalfEdgeMesh *halfEdge = new HalfEdgeMesh();
    
    Mesh *mesh = loadModel(name, true);//Grab the mesh data
    

    for(int i = 0; i < mesh->indexCount * 3; i +=3)
    {
        HalfEdge *one = new HalfEdge(), *two = new HalfEdge(), *three = new HalfEdge();
        
        
        HalfEdgeFace *face = new HalfEdgeFace();
        face->edge = one;
        
        one->face = face;
        two->face = face;
        one->face = face;
        
        one->endPt = new HalfEdgeVertex(one,mesh->indices[i+1],0);
        two->endPt = new HalfEdgeVertex(two,mesh->indices[i+2],0);
        three->endPt = new HalfEdgeVertex(three,mesh->indices[i],0);
        
        one->endPt->vertex = mesh->vertices[one->endPt->index].position;
        two->endPt->vertex = mesh->vertices[two->endPt->index].position;
        three->endPt->vertex = mesh->vertices[three->endPt->index].position;
        
        one->endPt->vertex.w = 1.0f;
        two->endPt->vertex.w = 1.0f;
        three->endPt->vertex.w = 1.0f;
        
        one->previous = three;
        two->previous = one;
        three->previous = two;
        
        one->next = two;
        two->next = three;
        three->next = one;
        
        halfEdge->halfEdges.push_back(one);
        halfEdge->halfEdges.push_back(two);
        halfEdge->halfEdges.push_back(three);
        
        halfEdge->faces.push_back(face);
    }
    
    
    //N^2 algorithm, very slow for large meshes running offline would
    //be ideal need to save output so this can be done.
    for(int i = 0; i < halfEdge->halfEdges.size(); ++i)
    {
        Vec4 startPt = halfEdge->halfEdges[i]->getSelf()->vertex;
        
        for(int j = i+1; j < halfEdge->halfEdges.size(); ++j)
        {
            if(halfEdge->halfEdges[j]->endPt->vertex == startPt)
            {
                Vec4 otherStartPt = halfEdge->halfEdges[j]->getSelf()->vertex;
                
                if(otherStartPt == halfEdge->halfEdges[i]->endPt->vertex)
                {
                    
                    halfEdge->halfEdges[j]->opposite = halfEdge->halfEdges[i];
                    halfEdge->halfEdges[i]->opposite = halfEdge->halfEdges[j];
                }
            }
        }
    }
    
    for(int i = 0; i< halfEdge->faces.size(); ++i)
    {
        HalfEdge *one = halfEdge->faces[i]->edge->opposite;
        HalfEdge *two = halfEdge->faces[i]->edge->next->opposite;
        HalfEdge *three = halfEdge->faces[i]->edge->next->next->opposite;
        
        halfEdge->faces[i]->edge->opposite = two;
        halfEdge->faces[i]->edge->next->opposite = three;
        halfEdge->faces[i]->edge->next->next->opposite = one;
    }
    
    if(saveBinary)//Save out name.half
    {
    }
    return halfEdge;
}

ElysiumEngine::Mesh *ElysiumEngine::ModelLibrary::meshFromHalfEdge(HalfEdgeMesh *halfEdgeMesh)
{
    std::vector<Vec4> data;
    std::vector<int> indices;
    Mesh *mesh = new Mesh();
    
    int i = 0;
    for(auto &face : halfEdgeMesh->faces)
    {
        HalfEdge *iter = face->edge;
        HalfEdge *end = face->edge;
        DrawDebugPoint p(Vec4(),Vec4(1.0f,1.0f,1.0f),150);
        do {
            auto vertIter = std::find(data.begin(),data.end(),iter->endPt->vertex);
            
            if(i > 0)
            {
                p.position = iter->endPt->vertex;

                //MessagingSystem::g_MessagingSystem->broadcastMessage(&p);
            }
            if(vertIter == data.end())
            {
                data.push_back(iter->endPt->vertex);
                indices.push_back(data.size()-1);
            }
            else
            {
                indices.push_back(vertIter - data.begin());
            }
            iter = iter->next;
            
        } while(iter != end);
        ++i;
    }
    
    mesh->indexCount = indices.size() / 3;
    mesh->indices = new unsigned int[mesh->indexCount * 3];
    
    mesh->vertexCount = data.size();
    mesh->vertices = new VertexData[mesh->vertexCount];
    
    for(int i = 0; i < mesh->vertexCount; ++i)
    {
        mesh->vertices[i].position = data[i];
    }
    
    for(int i = 0; i < mesh->indexCount * 3; ++i)
    {
        mesh->indices[i] = indices[i];
    }
    
    CalculateNormals(mesh);
    return mesh;
}

void CalculateNormals(ElysiumEngine::Mesh *mesh)
{
    float *m_FaceNormals = new float[mesh->indexCount * 3];
	//Calculate face normals always done
	for(int i =0; i < mesh->indexCount * 3; i+=3)
	{
		unsigned int IndexOne = mesh->indices[i];
		unsigned int IndexTwo = mesh->indices[i+1];
		unsigned int IndexThree = mesh->indices[i+2];
        
		Vec4 One = mesh->vertices[IndexOne].position;
        Vec4 Two = mesh->vertices[IndexTwo].position;
        Vec4 Three = mesh->vertices[IndexThree].position;
        
		Vec4 Norm = (One - Two) * (One - Three);
        
		Norm.Normalize();
        
		m_FaceNormals[i] = Norm.x;
		m_FaceNormals[i+1] = Norm.y;
		m_FaceNormals[i+2] = Norm.z;
        
        /*Vec4 Debug = Vec4(m_FaceNormals[i],m_FaceNormals[i+1],m_FaceNormals[i+2]);
        ElysiumEngine::DrawDebugLine one(Debug, Debug * 1.5,Vec4(1.0f,0.0f,0.0f),15.0f);
        ElysiumEngine::MessagingSystem::g_MessagingSystem->broadcastMessage(&one);*/
	}
    
	{
		Vec4 *NormalArray = new Vec4[mesh->vertexCount];
        
		for(int k = 0; k < mesh->indexCount * 3; k+=3)
		{
			//The w component of the vector is the number of indices
			Vec4 Temp(m_FaceNormals[k],m_FaceNormals[k+1],m_FaceNormals[k+2],1.0f);
            
			NormalArray[mesh->indices[k]]   += Temp;
			NormalArray[mesh->indices[k+1]] += Temp;
			NormalArray[mesh->indices[k+2]] += Temp;
		}
        
		//We now have the normal for everything
		//i is NormalArray index j is index int mesh->m_VertNormals k index into draw array for normals
		for(int i = 0, j = 0; i < mesh->vertexCount; ++i, ++j)
		{
			//NormalArray[i] /= NormalArray[i].w;
			NormalArray[i].w = 0.0f;
			NormalArray[i].Normalize();
            
			mesh->vertices[j].normal.x = NormalArray[i].x;
			mesh->vertices[j].normal.y = NormalArray[i].y;
			mesh->vertices[j].normal.z = NormalArray[i].z;
		}
	}
}

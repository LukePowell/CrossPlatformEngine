#include "ModelLibrary.h"
#include "GraphicsSystem.h"
#include "Messaging.h"
#include <sstream>
#include <ctime>

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
    std::clock_t start = std::clock();
    std::cout << "Creating half edge mesh " << name << std::endl;
    
    std::string finalname = name + ".half";
	bool saveBinary = false;
    
	if(!fexists(finalname.c_str()))
	{
		saveBinary = true;
	}
    
    HalfEdgeMesh *halfEdge = new HalfEdgeMesh();
    
    Mesh *mesh = loadModel(name, true);//Grab the mesh data
    
    //Edges that are located on the boundary of the mesh
    std::map<std::string,HalfEdge *> halfedges;
    std::list<HalfEdge *> boundaries;//TODO: Setup a means to render these as a different color
    std::vector<HalfEdgeFace *> faces(mesh->indexCount);
	std::vector<HalfEdgeVertex *> vertices(mesh->vertexCount);
	
	for (int i = 0; i < mesh->vertexCount; ++i)
	{
		vertices[i] = (new HalfEdgeVertex(0, 0, 0));
	}

//#define VERBOSE
    
    std::cout << mesh->indexCount << " faces.\n";
    for(int i = 0; i < mesh->indexCount * 3 ; i += 3)//Loop over our faces
    {
        faces[i/3] = new HalfEdgeFace();
        for(int j = 0; j < 3; ++j)
        {
            faces[i/3]->indices[j] = mesh->indices[i+j];
        }
        
        std::stringstream str;
		HalfEdge *previous = nullptr;
        for(int j = 0; j < 3; ++j)
        {
            int indexOne = faces[i/3]->indices[j];
            int indexTwo = faces[i/3]->indices[(j + 1) % 3];
            
#ifdef VERBOSE
            std::cout << "Edge: " << indexOne << "-" << indexTwo << std::endl;
#endif
            str << indexOne << "-" << indexTwo;
#ifdef VERBOSE
            std::cout << str.str() << std::endl;
            for(auto edge : halfedges)
            {
                std::cout << edge.first << " ";
            }
            std::cout << std::endl;
#endif
            auto halfEdge = halfedges.find(str.str());
            if(halfEdge == halfedges.end())
            {
                HalfEdge *one = new HalfEdge();
                one->face = faces[i/3];
                one->endPt = new HalfEdgeVertex(one,indexTwo,0);
                
                halfedges[str.str()] = one;
                
                if(j == 0)
                {
                    faces[i/3]->edge = one;
                }
                
				if (vertices[indexOne]->edge == nullptr)
				{
					vertices[indexOne]->edge = one;
					vertices[indexOne]->index = indexOne;
				}

                //Opposite
                str.str(std::string());
                str << indexTwo << "-" << indexOne;
                auto opp = halfedges.find(str.str());
                if(opp == halfedges.end())
                {
                    HalfEdge *opposite = new HalfEdge();

					one->opposite = opposite;
					opposite->opposite = one;

                    halfedges[str.str()] = opposite;

                    boundaries.push_back(opposite);
                    opposite->endPt = new HalfEdgeVertex(opposite,indexTwo,0);
					if (vertices[indexTwo]->edge == nullptr)
					{
						vertices[indexTwo]->edge = opposite;
						vertices[indexTwo]->index = indexTwo;
					}
                }
                else
                {
					one->opposite = opp->second;
					opp->second->opposite = one;

                    opp->second->face = faces[i/3];
                    boundaries.remove(opp->second);//Remove it if it exists
                    if(j > 0)//Update next pointers
                    {
                        opp->second->next = one;
                        opp->second->previous = previous;
                    }
                }
                
                if(j > 0)//Update next pointers
                {
                    previous->next = one;
                    one->previous = previous;
                }
                
                previous = one;
            }
            else//The Edge already exists
            {
				if (j == 0)
				{
					faces[i / 3]->edge = halfEdge->second;
				}

                if(halfEdge->second->face == nullptr)
                {
                    halfEdge->second->face = faces[i/3];
                }
                
                boundaries.remove(halfEdge->second);
                
                if(j > 0)//Update next pointers
                {
                    previous->next = halfEdge->second;
                    halfEdge->second->previous = previous;
                }

				previous = halfEdge->second;
            }
            str.str(std::string());
        }
    }
    
    std::cout << "Completed half edge mesh generation in " << (float)(std::clock() - start) / CLOCKS_PER_SEC << " seconds." << " With " << boundaries.size() << " boundary edges." << std::endl;
    
    for(auto edge : boundaries)
    {
       mesh->vertices[edge->endPt->index].color = Vec4(1.0f,0.0f,0.0f);
    }
     
    if(saveBinary)//Save out name.half
    {
        //TODO: Save out a binary representation of the half-edge mesh allowing O(1) loading
    }

	for (auto pair : halfedges)
	{
		halfEdge->halfEdges.push_back(pair.second);
	}
	halfEdge->halfEdgeVertices = vertices;
	halfEdge->faces = faces;

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

typedef std::vector<ElysiumEngine::HalfEdgeFace *> FaceList;

FaceList findNeighbors(ElysiumEngine::HalfEdgeFace *face, ElysiumEngine::Mesh *mesh, ElysiumEngine::HalfEdgeMesh *halfEdge)
{
	std::vector<ElysiumEngine::HalfEdgeFace *> neighbors;

	ElysiumEngine::HalfEdge *start = face->edge;
	ElysiumEngine::HalfEdge *stop = face->edge;
	do
	{
		if (start->opposite->face)
		{
			neighbors.push_back(start->opposite->face);
		}
		start = start->next;
	} while (start != stop && start != nullptr);
	/*for (auto start : face->indices)
	{
		ElysiumEngine::HalfEdge *h = halfEdge->halfEdgeVertices[start]->edge;
		ElysiumEngine::HalfEdge *stop = h;
		do
		{
			if (h->isBoundary())
				continue;

			if (std::find(neighbors.begin(), neighbors.end(), h->face) == neighbors.end() && std::find(faces.begin(),faces.end(),h->face) != faces.end())
			{
				neighbors.push_back(h->face);
			}

			h = h->opposite->next;
		} while (h != stop && h != nullptr);
	}*/

	return neighbors;
}


int countNeighbors(const FaceList &faces, ElysiumEngine::HalfEdgeFace *face, ElysiumEngine::Mesh * mesh, ElysiumEngine::HalfEdgeMesh *halfEdge)
{
	return findNeighbors(face, mesh, halfEdge).size();
}

struct Face
{
	ElysiumEngine::HalfEdgeFace *halfEdgeFace;
	FaceList neighbors;

	bool operator==(const Face &f)
	{
		return (halfEdgeFace == f.halfEdgeFace);
	}

	bool operator==(const ElysiumEngine::HalfEdgeFace *&rhs)
	{
		return (halfEdgeFace == rhs);
	}
};
int count(std::list<Face> &faces)
{
	int count = 0;
	for (Face f : faces)
	{
		if (f.halfEdgeFace->marker != -1)
			++count;
	}
	return count;
}

int count(FaceList &faces)
{
	int count = 0;
	for (ElysiumEngine::HalfEdgeFace *face : faces)
	{
		if (face->marker != -1)
			++count;
	}
	return count;
}

Face leastNeighbors(ElysiumEngine::Mesh *mesh, ElysiumEngine::HalfEdgeMesh *halfEdge, std::list<Face> &faces)
{
	int minNeighbors = INT_MAX;
	Face least;

	for (auto face : faces)
	{
		if (face.halfEdgeFace->marker == -1)
			continue;

		int neighbors = count(face.neighbors);
		if (neighbors < minNeighbors)
		{
			minNeighbors = neighbors;
			least = face;
		}
	}

	return least;
}



std::vector<ElysiumEngine::Strip> ElysiumEngine::ModelLibrary::stripeMesh(Mesh *mesh, HalfEdgeMesh *halfEdge)
{
	std::vector<std::vector<unsigned int>> stripes;
	std::clock_t start = std::clock();
	//Set up a means to track which faces we were adjacent to
	std::list<Face> faces;
	std::map<HalfEdgeFace *, Face> faceMap;

	for (auto face : halfEdge->faces)
	{
		Face f = { face };
		f.neighbors = findNeighbors(face, mesh, halfEdge);
		faces.push_back(f);
		faceMap[face] = faces.back();
	}
	std::cout << "Built neighbors in " << (float)(std::clock() - start) / CLOCKS_PER_SEC << " seconds.\n";

	while (count(faces))
	{
		Face face = leastNeighbors(mesh, halfEdge,faces);

		std::vector<unsigned int> strip;
		do
		{
			strip.insert(strip.end(), std::begin(face.halfEdgeFace->indices), std::end(face.halfEdgeFace->indices));

			face.halfEdgeFace->marker = -1;

			//Remove face from the list of faces that we can use
			faces.remove(face);

			//Chose a new tirangle
			HalfEdgeFace *tri = nullptr;
			if (count(face.neighbors))
			{
				FaceList l = findNeighbors(face.halfEdgeFace, mesh, halfEdge);// face.neighbors[rand() % face.neighbors.size()];
				while (count(l) && !tri)
				{
					tri = l[rand() % face.neighbors.size()];
					if (tri->marker == -1)
					{
						//face.neighbors.erase(std::find(face.neighbors.begin(), face.neighbors.end(), tri));
						tri = nullptr;
					}
                    else{
                        face.halfEdgeFace = tri;
                        face.neighbors = findNeighbors(tri, mesh, halfEdge);
                    }
				}
			}

			if (!count(face.neighbors))
				break;
			//std::cout << count(faces) << std::endl;
			//face = faceMap[tri];
		} while (count(face.neighbors));

 		stripes.push_back(strip);
		//std::cout << faces.size() << std::endl;
	}	
	std::cout << "Completed mesh striping in " << (float)(std::clock() - start) / CLOCKS_PER_SEC << " seconds.\n";
    
	std::vector<ElysiumEngine::Strip> stripesArrays;
	for (auto stripe : stripes)
	{
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		Vec4 color(r, g, b, 1.0f);
		
		Strip s;
		s.indices = new unsigned int[stripe.size()];
		s.count = stripe.size();

		for (int i = 0; i < stripe.size(); ++i)
		{
			s.indices[i] = stripe[i];
			mesh->vertices[s.indices[i]].color = color;
		}
		stripesArrays.push_back(s);
	}

	return stripesArrays;
}

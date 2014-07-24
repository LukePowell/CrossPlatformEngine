#include "HalfEdgePhysics.h"
#include "Messaging.h"
#include "GameObjectFactory.h"
#include "GraphicsSystem/MeshRenderable.h"
#include "RigidBody.h"
#include "Collider.h"

#include <map>
#include <cassert>
namespace ElysiumEngine
{
    struct Adjacency
    {
        EdgeToSplit e;
        float dpResult;
    };
    
    bool compareAdjacency(Adjacency one, Adjacency two)
    {
        return one.dpResult < two.dpResult;
    }
}

std::list<ElysiumEngine::EdgeToSplit> ElysiumEngine::calculateHalfEdgesToSplit(Vec4 &normal, float offset, HalfEdgeMesh *mesh, Matrix &matrix, std::vector<EdgeToSplit> &duplicates)
{
    std::list<EdgeToSplit> edgesToSplit;
    
    for(auto &face : mesh->faces)
    {
        HalfEdge *edge = face->edge;
        HalfEdge *start = face->edge;
        do
        {
            Vec4 v1 = matrix * edge->endPt->vertex;
            HalfEdge *edge2 = edge->next;
            Vec4 v2 = matrix * edge2->endPt->vertex;
            
            float t = (offset - normal.DotNoW(v1))/(normal.DotNoW(v2-v1));
            
            if(t >= 0.0f && t <= 1.0f)//The edge crosses the plain
            {
                EdgeToSplit split;
                split.edge = edge;
                split.t = t;
                split.point = (1-t) * v1 + t * v2;
                split.point.w = 1.0f;
                
                bool copy = false;
                for(auto &iter : edgesToSplit)
                {
                    if(iter.point == split.point)
                        copy = true;
                }
                
                edgesToSplit.push_back(split);

                duplicates.push_back(split);
                
                DrawDebugPoint pt(split.point,Vec4(0.0f,1.0f,0.0f),120.0f);
                MessagingSystem::g_MessagingSystem->broadcastMessage(&pt);
            }
            edge = edge->next;
        } while (edge != start);
    }
    
    return edgesToSplit;
}

void print(ElysiumEngine::HalfEdge *loop)
{
    ElysiumEngine::HalfEdge *start = loop;
    do {
        std::cout << start->getSelf()->marker << " ";
        start = start->next;
    } while (start != loop);
    std::cout << std::endl;
}

void ElysiumEngine::splitHalfEdgeMesh(Vec4 &normal, float offset, ElysiumEngine::HalfEdgeMesh *mesh, ElysiumEngine::Transform &trans)
{
    Matrix translate;
    
    translate.Identity();
    translate *= BuildTranslationMatrix(trans.GetPosition().x, trans.GetPosition().y, trans.GetPosition().z);
    translate *= BuildScaleMatrix(trans.GetScale().x, trans.GetScale().y, trans.GetScale().z);
    translate *= trans.GetOrientation().QuaternionToMatrix();
    
    Matrix inverse = translate.Inverse();
    
 
    std::vector<EdgeToSplit> duplicates;
    std::list<EdgeToSplit> splitList = calculateHalfEdgesToSplit(normal, offset, mesh, translate, duplicates);
    
    
    if(splitList.empty())
        return;
    
    std::map<HalfEdgeFace *,std::pair<EdgeToSplit*, EdgeToSplit *> > facesToSplit;
    
    for(auto &splitEdge : splitList)
    {
        auto it = facesToSplit.find(splitEdge.edge->face);
        
        if(it == facesToSplit.end())
        {
            facesToSplit[splitEdge.edge->face] = std::pair<EdgeToSplit *, EdgeToSplit *>(&splitEdge,nullptr);
        }
        else
        {
            it->second.second = &splitEdge;
        }
    }
    
    HalfEdgeMesh *objOneHEM = new HalfEdgeMesh();
    HalfEdgeMesh *objTwoHEM = new HalfEdgeMesh();
    
    for(auto &face : mesh->faces)
    {
        HalfEdge *begin = face->edge;
        HalfEdge *change = face->edge;
        
        bool oneSided = true;
        
        do
        {
            change = change->next;
            
            HalfEdgeVertex *start = change->getSelf();
            HalfEdgeVertex *end = change->endPt;
            
            if(((translate * start->vertex).DotNoW(normal) - offset) > 0.0f)
            {
                start->marker = 1;
            }
            else if(((translate * start->vertex).DotNoW(normal) - offset) < 0.0f)
            {
                start->marker = 2;
            }
            else
            {
                start->marker = 3;
            }
            
            
            if(((translate * end->vertex).DotNoW(normal) - offset) > 0.0f)
            {
                end->marker = 1;
            }
            else if(((translate * end->vertex).DotNoW(normal) - offset) < 0.0f)
            {
                end->marker = 2;
            }
            else
            {
                end->marker = 3;
            }
            
            if((start->marker != end->marker))
            {
                oneSided = false;
            }
            
        } while (begin != change);
        
        if(oneSided)
        {
            begin = face->edge;
            change = face->edge;

            if(face->edge->endPt->marker == 1){
                objOneHEM->faces.push_back(face);
            }
            else if(face->edge->endPt->marker == 2){
                objTwoHEM->faces.push_back(face);
            }
            do
            {
                change = change->next;
                
                if(change->endPt->marker == 1)
                {
                    objOneHEM->halfEdges.push_back(change);
                }
                else if (change->endPt->marker == 2)
                {
                    objTwoHEM->halfEdges.push_back(change);
                }
                
            } while (begin != change);
        }
    }
    
    Vec4 centroid;
    
    //Calculate the centroid
    for(auto &splitEdge : splitList)
    {
        centroid += splitEdge.point;
    }
    
    centroid /= centroid.w;
    
    DrawDebugPoint pt(centroid,Vec4(1.0f,0.0f,1.0f),100);
    MessagingSystem::g_MessagingSystem->broadcastMessage(&pt);
    
    HalfEdge *newEdges[2];//Used to store the new splits;
    
    DrawDebugPoint p(Vec4(),Vec4(1.0f,0.0f,0.0f),20);
    
    for(auto &splitFace : facesToSplit)
    {
        if(splitFace.second.second == nullptr || splitFace.second.first == nullptr)
            continue;
        
        EdgeToSplit *one = splitFace.second.first;
        EdgeToSplit *two = splitFace.second.second;
        
        HalfEdge *top = new HalfEdge();
        HalfEdge *bottom = new HalfEdge();
        
        top->endPt = new HalfEdgeVertex(top,inverse * two->point,0);
        bottom->endPt = new HalfEdgeVertex(bottom,inverse * one->point,0);
        
        //Split the first edge
        HalfEdge *edge1 = one->edge;
        HalfEdge *edge2 = one->edge->opposite;
        
        if(!edge2)
            return;
        
        HalfEdge *edge1_b = new HalfEdge();
        HalfEdge *edge2_b = new HalfEdge();
        
        newEdges[0] = edge1_b;
        
        edge1_b->endPt = edge1->endPt;
        edge1_b->endPt->edge = edge1_b;
        edge1_b->face = edge1->face;
        edge1_b->next = edge1->next;
        edge1->endPt = new HalfEdgeVertex(edge1,inverse * one->point,3);
        edge1->next = edge1_b;
        
        edge2_b->endPt = edge2->endPt;
        edge2_b->endPt->edge = edge2_b;
        edge2_b->face = edge2->face;
        edge2_b->next = edge2->next;
        edge2->endPt = new HalfEdgeVertex(edge2,inverse * one->point,3);
        edge2->next = edge2_b;
        
        edge2_b->opposite = edge1;
        edge2->opposite = edge1_b;
        edge1_b->opposite = edge2;
        edge1->opposite = edge2_b;

        //Split the second edge
        edge1 = two->edge;
        edge2 = two->edge->opposite;
        edge1_b = new HalfEdge();
        edge2_b = new HalfEdge();
        
        newEdges[1] = edge1_b;
        
        edge1_b->endPt = edge1->endPt;
        edge1_b->endPt->edge = edge1_b;
        edge1_b->face = edge1->face;
        edge1_b->next = edge1->next;
        edge1->endPt = new HalfEdgeVertex(edge1,inverse * two->point,3);
        edge1->next = edge1_b;
        
        edge2_b->endPt = edge2->endPt;
        edge2_b->endPt->edge = edge2_b;
        edge2_b->face = edge2->face;
        edge2_b->next = edge2->next;
        edge2->endPt = new HalfEdgeVertex(edge2,inverse * two->point,3);
        edge2->next = edge2_b;

        edge2_b->opposite = edge1;
        edge2->opposite = edge1_b;
        edge1_b->opposite = edge2;
        edge1->opposite = edge2_b;
        
        
        std::vector<HalfEdgeVertex *> objectOne;
        std::vector<HalfEdgeVertex *> objectTwo;
        
        HalfEdgeFace *faceOne = new HalfEdgeFace();
        HalfEdgeFace *faceTwo = new HalfEdgeFace();
        
        //Loop over the newly created loop of vertices and seperate into two objects
        HalfEdge *sent = one->edge;
        HalfEdge *iter = one->edge;
        do
        {
            if(iter->getSelf()->marker == 1)
            {
                objectOne.push_back(new HalfEdgeVertex(*iter->getSelf()));
            }
            else if(iter->getSelf()->marker == 2)
            {
                objectTwo.push_back(new HalfEdgeVertex(*iter->getSelf()));
            }
            else if(iter->getSelf()->marker == 3)
            {
                bool oneFound = false;
                bool twoFound = false;
                
                for(auto &iter2 : objectOne)
                {
                    if(iter2->vertex == iter->getSelf()->vertex)
                        oneFound = true;
                }
                for(auto &iter2 : objectTwo)
                {
                    if(iter2->vertex == iter->getSelf()->vertex)
                        twoFound = true;
                }
                if(!oneFound && !twoFound)
                {
                    objectOne.push_back(new HalfEdgeVertex(*iter->getSelf()));                  objectTwo.push_back(new HalfEdgeVertex(*iter->getSelf()));
                }
            }
            iter = iter->next;
        }while(iter != sent);
        

        std::vector<HalfEdge *> edgesOne;
        std::vector<HalfEdge *> edgesTwo;
        
        static  bool first2 = false;
        DrawDebugPoint p(Vec4(),Vec4(0.0f,0.0f,1.0f),15);
        
        //Iterate over the elements found above
        if(objectOne.size() == 3)
        {
            for(int i = 0; i < objectOne.size(); i++)
            {
                HalfEdge *edge = new HalfEdge();
                edge->face  = faceOne;
                edge->endPt = objectOne[(i + 1) % objectOne.size()];
                edge->endPt->edge = edge;
                edgesOne.push_back(edge);
            
            }
            for(int i = 0; i < edgesOne.size(); ++i)
            {
                edgesOne[i]->next = edgesOne[(i+1) % edgesOne.size()];
                edgesOne[i]->userData = (void *)10;
                objOneHEM->halfEdges.push_back(edgesOne[i]);
            }
            
            faceOne->edge = edgesOne[0];
            //if(faceOne->edge->getCount() == 3)
                objOneHEM->faces.push_back(faceOne);
        }
        else if(objectOne.size() == 4)
        {
            HalfEdgeFace *faceOne = new HalfEdgeFace();
            HalfEdgeFace *faceTwo = new HalfEdgeFace();
            
            HalfEdgeVertex *one[3] = {objectOne[0],objectOne[1],objectOne[2]};
            HalfEdgeVertex *two[3] = {objectOne[2],objectOne[3],objectOne[0]};
            
            HalfEdge *newEdgeOnes[3];
            HalfEdge *newEdgeTwos[3];
            
            for(int i = 0; i < 3; ++i)
            {
                HalfEdge *edge1 = new HalfEdge();
                edge1->face = faceOne;
                
                HalfEdge *edge2 = new HalfEdge();
                edge2->face = faceTwo;
                
                edge1->endPt = new HalfEdgeVertex(*one[(i+1)%3]);
                edge1->endPt->edge = edge1;
                edge2->endPt = new HalfEdgeVertex(*two[(i+1)%3]);
                edge2->endPt->edge = edge2;
                
                newEdgeOnes[i] = edge1;
                newEdgeTwos[i] = edge2;
            }
            
            for(int i = 0; i < 3; ++i)
            {
                newEdgeOnes[i]->next = newEdgeOnes[(i+1) % 3];
                newEdgeTwos[i]->next = newEdgeTwos[(i+1) % 3];
                objOneHEM->halfEdges.push_back(newEdgeOnes[i]);
                objOneHEM->halfEdges.push_back(newEdgeTwos[i]);
                
                newEdgeOnes[i]->userData = (void *)20;
                newEdgeTwos[i]->userData = (void *)20;
            }
            
            
            faceOne->edge = newEdgeOnes[0];
            faceTwo->edge = newEdgeTwos[0];
            
            //if(faceOne->edge->getCount() == 3)
                objOneHEM->faces.push_back(faceOne);
            //if(faceTwo->edge->getCount() == 3)
                objOneHEM->faces.push_back(faceTwo);
        }
        else
        {
         Vec4 centroid;
         
         for(int i = 0; i < objectOne.size(); ++i)
         {
             if(!first2)
             {
                 p.position = translate * objectOne[i]->vertex;
                 MessagingSystem::g_MessagingSystem->broadcastMessage(&p);
             }
             centroid += objectOne[i]->vertex;
         }
            
         first2 = true;
         
         centroid /= centroid.w;
         DrawDebugPoint pt(translate * centroid,Vec4(1.0f,0.0f,0.0),15);
         MessagingSystem::g_MessagingSystem->broadcastMessage(&pt);
         for(int i = 0; i < objectOne.size(); ++i)
         {
             
             HalfEdgeFace *face = new HalfEdgeFace();
             HalfEdge *hOne = new HalfEdge();
             HalfEdge *hTwo = new HalfEdge();
             HalfEdge *hThree = new HalfEdge();
             
             face->edge = hOne;
             
             hOne->endPt = new HalfEdgeVertex(hOne,centroid,0);
             hTwo->endPt = new HalfEdgeVertex(*objectOne[i]);
             hThree->endPt = new HalfEdgeVertex(*objectOne[(i+1)%objectOne.size()]);
             
             hOne->userData = (void *)30;
             hTwo->userData = (void *)30;
             hThree->userData = (void *)30;
             
             hTwo->endPt->edge = hTwo;
             hThree->endPt->edge = hThree;
             
             hOne->next = hTwo;
             hTwo->next = hThree;
             hThree->next = hOne;
             
             objOneHEM->halfEdges.push_back(hOne);
             objOneHEM->halfEdges.push_back(hTwo);
             objOneHEM->halfEdges.push_back(hThree);
             //if(face->edge->getCount() == 3)
                 objOneHEM->faces.push_back(face);
         }
        }
        if(objectTwo.size() == 3)
        {
            for(int i = 0; i < objectTwo.size(); i++)
            {
                HalfEdge *edge = new HalfEdge();
                edge->face  = faceTwo;
                edge->endPt = objectTwo[(i + 1) % objectTwo.size()];
                edge->endPt->edge = edge;
                edgesTwo.push_back(edge);
            }
            
            for(int i = 0; i < edgesTwo.size(); ++i)
            {
                edgesTwo[i]->next = edgesTwo[(i+1) % edgesTwo.size()];
                edgesTwo[i]->userData = (void *)10;
                objTwoHEM->halfEdges.push_back(edgesTwo[i]);
            }
            faceTwo->edge = edgesTwo[0];
            //if(faceTwo->edge->getCount() == 3)
                objTwoHEM->faces.push_back(faceTwo);
        }
        else if(objectTwo.size() == 4)
        {
            HalfEdgeFace *faceOne = new HalfEdgeFace();
            HalfEdgeFace *faceTwo = new HalfEdgeFace();
            
            HalfEdgeVertex *one[3] = {objectTwo[0],objectTwo[1],objectTwo[2]};
            HalfEdgeVertex *two[3] = {objectTwo[2],objectTwo[3],objectTwo[0]};
            
            HalfEdge *newEdgeOnes[3];
            HalfEdge *newEdgeTwos[3];
            
            for(int i = 0; i < 3; ++i)
            {
                HalfEdge *edge1 = new HalfEdge();
                edge1->face = faceOne;
                
                HalfEdge *edge2 = new HalfEdge();
                edge2->face = faceTwo;
                
                edge1->endPt = new HalfEdgeVertex(*one[(i+1)%3]);
                edge1->endPt->edge = edge1;
                edge2->endPt = new HalfEdgeVertex(*two[(i+1)%3]);
                edge2->endPt->edge = edge2;
                
                newEdgeOnes[i] = edge1;
                newEdgeTwos[i] = edge2;
            }
            
            for(int i = 0; i < 3; ++i)
            {
                newEdgeOnes[i]->next = newEdgeOnes[(i+1) % 3];
                newEdgeTwos[i]->next = newEdgeTwos[(i+1) % 3];
                objTwoHEM->halfEdges.push_back(newEdgeOnes[i]);
                objTwoHEM->halfEdges.push_back(newEdgeTwos[i]);
                
                newEdgeOnes[i]->userData = (void *)20;
                newEdgeTwos[i]->userData = (void *)20;
            }
            
            faceOne->edge = newEdgeOnes[0];
            faceTwo->edge = newEdgeTwos[0];
            
            //if(faceOne->edge->getCount() == 3)
                objTwoHEM->faces.push_back(faceOne);
            //if(faceTwo->edge->getCount() == 3)
                objTwoHEM->faces.push_back(faceTwo);
            
            std::cout << faceOne->edge->getCount() << std::endl;
        }
        else
        {
            Vec4 centroid;
            
            for(int i = 0; i < objectTwo.size(); ++i)
            {
                centroid += objectTwo[i]->vertex;
            }
            
            assert(centroid.w == objectTwo.size());
            centroid /= centroid.w;
            DrawDebugPoint pt(translate * centroid,Vec4(1.0f,0.0f,0.0),15);
            MessagingSystem::g_MessagingSystem->broadcastMessage(&pt);
            
            for(int i = 0; i < objectTwo.size(); ++i)
            {
                HalfEdgeFace *face = new HalfEdgeFace();
                HalfEdge *hOne = new HalfEdge();
                HalfEdge *hTwo = new HalfEdge();
                HalfEdge *hThree = new HalfEdge();
                
                
                hOne->userData = (void *)30;
                hTwo->userData = (void *)30;
                hThree->userData = (void *)30;
                
                face->edge = hOne;
                
                hOne->endPt = new HalfEdgeVertex(hOne,centroid,0);
                hTwo->endPt = new HalfEdgeVertex(*objectTwo[i]);
                hThree->endPt = new HalfEdgeVertex(*objectTwo[(i+1) % objectTwo.size()]);
                
                hTwo->endPt->edge = hTwo;
                hThree->endPt->edge = hThree;
                
                hOne->next = hTwo;
                hTwo->next = hThree;
                hThree->next = hOne;
                
                objTwoHEM->halfEdges.push_back(hOne);
                objTwoHEM->halfEdges.push_back(hTwo);
                objTwoHEM->halfEdges.push_back(hThree);
                
                //if(face->edge->getCount() == 3)
                objTwoHEM->faces.push_back(face);
            }
        }

    }

    for(int i = 0; i <duplicates.size(); i++)
    {
        HalfEdgeFace *face = new HalfEdgeFace();
        
        EdgeToSplit one = duplicates[i];
        
        EdgeToSplit two = duplicates[(i+1)%duplicates.size()];
        
        HalfEdge *hOne = new HalfEdge();
        HalfEdge *hTwo = new HalfEdge();
        HalfEdge *hThree = new HalfEdge();
        
        face->edge = hOne;
        
        hOne->endPt = new HalfEdgeVertex(hOne,inverse * centroid,0);
        hTwo->endPt = new HalfEdgeVertex(hTwo,inverse * one.point,0);
        hThree->endPt = new HalfEdgeVertex(hThree,inverse * two.point,0);
        
        hOne->next = hTwo;
        hTwo->next = hThree;
        hThree->next = hOne;
        
        hOne->userData = (void *)40;
        hTwo->userData = (void *)40;
        hThree->userData = (void *)40;
        
        objOneHEM->halfEdges.push_back(hOne);
        objOneHEM->halfEdges.push_back(hTwo);
        objOneHEM->halfEdges.push_back(hThree);
        objOneHEM->faces.push_back(face);
        
        face = new HalfEdgeFace();
        hOne = new HalfEdge();
        hTwo = new HalfEdge();
        hThree = new HalfEdge();
        
        
        hOne->userData = (void *)40;
        hTwo->userData = (void *)40;
        hThree->userData = (void *)40;
        
        face->edge = hOne;
        
        hOne->endPt = new HalfEdgeVertex(hOne,inverse * centroid,0);
        hTwo->endPt = new HalfEdgeVertex(hTwo,inverse * one.point,0);
        hThree->endPt = new HalfEdgeVertex(hThree,inverse * two.point,0);
        
        hOne->next = hTwo;
        hTwo->next = hThree;
        hThree->next = hOne;
    
        objTwoHEM->halfEdges.push_back(hOne);
        objTwoHEM->halfEdges.push_back(hTwo);
        objTwoHEM->halfEdges.push_back(hThree);
        
        //if(face->edge->getCount() == 3)
        objTwoHEM->faces.push_back(face);
    }
    
    for(auto &face : objOneHEM->faces)
    {
        print(face->edge);
        assert(face->edge->getCount() == 3);
    }
    
    for(auto &face : objTwoHEM->faces)
    {
        print(face->edge);
        assert(face->edge->getCount() == 3);
    }

    GameObject *baseObject = trans.getOwner();
    baseObject->kill();
    
    GameObject *obj1 = GameObjectFactory::g_GameObjectFactory->createEmptyGameObject();
    
    Transform *trans1 = new Transform();
    trans1->SetPosition(trans.GetPosition());
    trans1->GetPosition() += normal * 0.001f;
    trans1->SetScale(trans.GetScale());
    trans1->SetOrientation(trans.GetOrientation());
    obj1->addComponent(trans1);
    
    MeshRenderable *one = new MeshRenderable(*trans.getSibling<MeshRenderable>("MeshRenderable"));
    obj1->addComponent(one);
    obj1->addComponent(new ElysiumEngine::RigidBody(trans.getSibling<RigidBody>("RigidBody")));
    obj1->addComponent(new ElysiumEngine::MeshCollider());
    obj1->initialize();
    one->setHalfEdgeMesh(objOneHEM);
    obj1->lateInitialize();
    
    GameObject *obj2 = GameObjectFactory::g_GameObjectFactory->createEmptyGameObject();
    Transform *trans2 = new Transform();
    trans2->SetPosition(trans.GetPosition());
    trans2->SetOrientation(trans.GetOrientation());
    obj2->addComponent(trans2);
  
    
    MeshRenderable *two = new MeshRenderable(*trans.getSibling<MeshRenderable>("MeshRenderable"));
    obj2->addComponent(two);
    obj2->addComponent(new ElysiumEngine::RigidBody(trans.getSibling<RigidBody>("RigidBody")));
    obj2->addComponent(new ElysiumEngine::MeshCollider());
    obj2->initialize();
    two->setHalfEdgeMesh(objTwoHEM);
    obj2->lateInitialize();
}


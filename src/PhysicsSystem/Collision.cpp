#include "Collision.h"
#include "Maths/MathLib.h"
#include "PhysicsSystem.h"
#include "Messaging.h"
#include <deque>
static const float COLLISION_EPSILON = 0.04f;
static Vec4 m_Origin = Vec4(0.0f,0.0f,0.0f);


void EPA(std::deque<Vec4> simplex, Vec4 &contactPoint, Vec4 &normal, float &penetration)
{
    while(true)
    {
        break;
    }
}

Vec4 FindFurthest(ElysiumEngine::ICollider *first, ElysiumEngine::ICollider *second, Vec4 dir)
{
	Vec4 PointA = first->SupportMapping(dir);
	Vec4 PointB = second->SupportMapping(-dir);
    
	//Vec4 DirNorm = dir;
	//DirNorm.Normalize();
	//Message::DBox boxA(PointA.x,PointA.y,PointA.z,0.05,0.05,0.05,dir.x,dir.y,dir.z);
	//Message::DBox boxB(PointB.x,PointB.y,PointB.z,0.05,0.05,0.05,dir.x,dir.y,dir.z);
	//Message::g_MessageHandler->BroadcastMessage(&boxA);
	//Message::g_MessageHandler->BroadcastMessage(&boxB);
    
	return PointA - PointB;
}

//GJK Algorithim
bool GJK(ElysiumEngine::ICollider *First, ElysiumEngine::ICollider *Second, std::deque<Vec4> &m_Simplex)
{

	Vec4 Dir0 = Second->GetPosition() - First->GetPosition();
	//1. Initialize the simplex set Q to one or more points (up to d+1 points, where d is the dimenson) from the Minkowski difference of A and B.
	m_Simplex.push_back(FindFurthest(First,Second,Dir0));
    
	for(int i = 0; i < 20; ++i)//Cap the algorithm at a maximum iteration count
	{
		Vec4 P = Vec4(1.0f,0.0,0.0);//Some non-origin value
        
		//2. Compute the m_Origin P of minimum norm in CH(Q)
		if(m_Simplex.size() == 1)
		{
			P =  m_Simplex[0];
		}
		else if(m_Simplex.size() == 2)
		{
			P = Maths::ClosestPointToSegment(m_Simplex[0],m_Simplex[1],m_Origin);
		}
		else if(m_Simplex.size() == 3)
		{
			P = Maths::ClosestPtTriangleToPoint(m_Simplex[0],m_Simplex[1],m_Simplex[2],m_Origin);
		}
		else if(m_Simplex.size() == 4)
		{
			//Find closest m_Origin to tetrahedron
			P = Maths::ClosesetPointOnTetrahedronToPoint(m_Simplex[0],m_Simplex[1],m_Simplex[2],m_Simplex[3],m_Origin);
		}
        
		//3. If P is the origin itself, the origin is clearly contained in the Minkowski difference of A and B. Stop and return A and B as intersecting
		if(P == Vec4(0.0f,0.0f,0.0f))
		{
			//DrawSimplex(m_Simplex);
			if(m_Simplex.size() == 4)
			{
				//EPA(m_Simplex,First,Second);
				return true;
			}
			else
				return false;
		}
        
		//4. Reduce Q to the smallest subset Q' of Q such that P is an element of CH(Q`), i.e remove any points from the simplex not needed to define a simplex containing P
		if(m_Simplex.size() == 3)//Triangle
		{
            if((m_Simplex[0]-m_Simplex[2]).Dot(-P) >= 0.0f)
            {
                m_Simplex[1] = m_Simplex[2];
                m_Simplex.pop_back();
            }
            else if((m_Simplex[1] - m_Simplex[2]).Dot(-P) >= 0.0f)
            {
                m_Simplex[0] = m_Simplex[1];
                m_Simplex[1] = m_Simplex[2];
                m_Simplex.pop_back();
            }
		}
		else if(m_Simplex.size() == 4)//Tetrahedron
		{
			int Triangle = -1;//Inside the tetrahedron
			Vec4 ClosestPoint = Vec4();
			float bestSqDst = FLT_MAX;
			Vec4 Points[4];
			float Distances[4] = {FLT_MAX,FLT_MAX,FLT_MAX,FLT_MAX};
            
			Vec4 a = m_Simplex[0];
			Vec4 b = m_Simplex[1];
			Vec4 c = m_Simplex[2];
			Vec4 d = m_Simplex[3];
			if(Maths::PointOutsideOfPlane(P,a,b,c,d))
			{
				Vec4 q = Maths::ClosestPtTriangleToPoint(a,b,c,P);
				Points[0] = q;
				Distances[0] = (q-P).Length();
				float sqDist = (q-P).LengthSq();
				if(sqDist < bestSqDst && Distances[0] > 0.0f)
				{
					bestSqDst = sqDist;
					ClosestPoint = q;
					Triangle = 0;
				}
			}
			if(Maths::PointOutsideOfPlane(P,a,c,d,b))
			{
				Vec4 q = Maths::ClosestPtTriangleToPoint(a,c,d,P);
				Points[1] = q;
				Distances[1] = (q-P).Length();
				float sqDist = (q-P).LengthSq();
				if(sqDist < bestSqDst && Distances[1] > 0.0f)
				{
					bestSqDst = sqDist;
					ClosestPoint = q;
					Triangle = 1;
				}
			}
			if(Maths::PointOutsideOfPlane(P,a,d,b,c))
			{
				Vec4 q = Maths::ClosestPtTriangleToPoint(a,d,b,P);
				Points[2] = q;
				Distances[2] = (q-P).Length();
				float sqDist = (q-P).LengthSq();
				if(sqDist < bestSqDst && Distances[2] > 0.0f)
				{
					bestSqDst = sqDist;
					ClosestPoint = q;
					Triangle  = 2;
				}
			}
			if(Maths::PointOutsideOfPlane(P,b,d,c,a))
			{
				Vec4 q = Maths::ClosestPtTriangleToPoint(b,d,c,P);
				Points[3] = q;
				Distances[3] = (q-P).Length();
				float sqDist = (q-P).LengthSq();
				if(sqDist < bestSqDst && Distances[3] > 0.0f)
				{
					bestSqDst = sqDist;
					ClosestPoint = q;
					Triangle = 3;
				}
			}
            
			switch(Triangle)
			{
                case -1:
                    //We are inside the tetrahedron ???
                    //		return false;
                    break;
                case 0://a point on triangle a b c is closest
                    m_Simplex.pop_back();//No longer need d
                    break;
                case 1://a point on triangle a c d is closest
                    m_Simplex[0] = a;
                    m_Simplex[1] = c;
                    m_Simplex[2] = d;
                    m_Simplex.pop_back();
                    break;
                case 2://a point on triangle a d b is closest
                    m_Simplex[0] = a;
                    m_Simplex[1] = d;
                    m_Simplex[2] = b;
                    m_Simplex.pop_back();
                    break;
                case 3://a point on triangle b d c is closest
                    m_Simplex[0] = b;
                    m_Simplex[1] = d;
                    m_Simplex[2] = c;
                    m_Simplex.pop_back();
                    break;
			}
			if((m_Simplex[0]-m_Simplex[2]).Dot(-P) >= 0.0f)
			{
				m_Simplex[1] = m_Simplex[2];
				m_Simplex.pop_back();
			}
			else if((m_Simplex[1] - m_Simplex[2]).Dot(-P) >= 0.0f)
			{
				m_Simplex[0] = m_Simplex[1];
				m_Simplex[1] = m_Simplex[2];
				m_Simplex.pop_back();
			}
		}
        
		//5. Let V = S(A(-)B)(-P)=S_A(-P)-S_B(P) be a supporting m_Origin in direction -P
		Vec4 V = FindFurthest(First,Second,-P);
        
		//6. IF V is no more external in direction -P then P itself, stop and return A and B as not intersecting. The length of the vector from the origin to P is the separation distance of A and B
		if(V.Dot(-P) < 0)
		{
			//DrawSimplex(m_Simplex);
			return false;
		}
        
		//7. Add V to Q and go to 7
		m_Simplex.push_back(V);
	}
    
	//DrawSimplex(m_Simplex);
	return false;
}


bool spherePlane(ElysiumEngine::SphereCollider *one, ElysiumEngine::PlaneCollider *two, Vec4 &normal, Vec4 &collisionPoint, float &penetration)
{
    
    float dist = one->GetPosition().Dot(two->getNormal());
    
    normal = two->getNormal();
    collisionPoint = -normal * one->getRadius() + one->GetPosition();
    
    penetration = collisionPoint.Dot(two->getNormal());
    
    return dist <= one->getRadius();
}

bool obbPlane(ElysiumEngine::OBBCollider *one, ElysiumEngine::PlaneCollider *two)
{

    Maths::OBB One = one->GetOBB();
    
    Matrix A = One.m_Axis.QuaternionToMatrix();
    
    std::list<Vec4> contactPointsOne;
    
    Vec4 obj1Verts[8];
    
    Vec4 extentsOne = One.m_HalfExtents;

    
    int sign = -1;
    
    for(int i = 0; i < 8; i += 2)
    {
        if(sign >= 0)
        {
            extentsOne[sign] = -extentsOne[sign];
        }
        
        obj1Verts[i] = extentsOne;
        
        obj1Verts[i+1] = -extentsOne;
        
        
        obj1Verts[i] = A * obj1Verts[i];
        
        obj1Verts[i+1] = A * obj1Verts[i+1];
        
        obj1Verts[i] += One.m_COM;
        obj1Verts[i+1] += One.m_COM;
        
        if(sign >= 0)
        {
            extentsOne[sign] = -extentsOne[sign];
        }
        
        sign++;
    }
    
    bool collision = false;
    for(Vec4& vector : obj1Verts)
    {
        float distance = vector.Dot(two->getNormal());
        
        if(distance <= two->getOffset())
        {
            collision = true;
            
            Vec4 normal = two->getNormal();
            normal *= -1;
            
            ElysiumEngine::DrawDebugPoint point(vector,Vec4(1.0f,0.0f,1.0f));
            ElysiumEngine::MessagingSystem::g_MessagingSystem->broadcastMessage(&point);

            ElysiumEngine::PhysicsSystem::g_PhysicsSystem->addCollisionPair(ElysiumEngine::ColliderPair(one,two,normal,vector,distance));
        }
    }
    
    return collision;
}

bool sphereSphere(ElysiumEngine::SphereCollider *one, ElysiumEngine::SphereCollider *two)
{
    Vec4 distance = one->GetPosition() - two->GetPosition();
	float distSQ = distance.Dot(distance);
	return distSQ <= (one->getRadius() + two->getRadius()) * (one->getRadius() + two->getRadius());
}

bool sphereBox(ElysiumEngine::SphereCollider *one, ElysiumEngine::OBBCollider *two, Vec4 &closestPoint)
{
	closestPoint = Maths::ClosestPointToPointOBB(one->GetPosition(),two->GetOBB());
    
	Vec4 v = closestPoint - one->GetPosition();
    
	return v.LengthSq() <= one->getRadius() * one->getRadius();
}

bool OBBOBB(ElysiumEngine::OBBCollider *First, ElysiumEngine::OBBCollider *Second, Vec4 &contact, Vec4 &Normal)
{
	Maths::OBB One = First->GetOBB();
	Maths::OBB Two = Second->GetOBB();
    
	bool EdgeEdge = false;
	bool ExistsParallel = false;
    
	float Min = FLT_MAX;
	float ra, rb;
	Matrix R, AbsR;
    
	Matrix A = One.m_Axis.QuaternionToMatrix();
	Matrix B = Two.m_Axis.QuaternionToMatrix();
    
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
		{
			R[i][j] = A[i][0] * B[j][0] + A[i][1] * B[j][1] + A[i][2] * B[j][2];
            
			if(R[i][j] > 0.9999f)
				ExistsParallel = true;
		}
    
	A[3][3] = 1.0f;
	B[3][3] = 1.0f;
    
	Vec4 T = Two.m_COM - One.m_COM;
    
	T = A * T;
	
	for(int i = 0; i < 3; ++i)
		for(int j =0; j < 3; ++j)
			AbsR[i][j] = std::abs(R[i][j]) + Epsilon;
	
	// L = A0, A1, A2
	for(int i =0; i < 3; ++i)
	{
		ra = One.m_HalfExtents[i];
		rb = Two.m_HalfExtents[0] * AbsR[i][0] + Two.m_HalfExtents[1] * AbsR[i][1] + Two.m_HalfExtents[2] * AbsR[i][2];
		if(std::abs(T[i]) > ra + rb)
			return false;
		else
		{
			if((ra + rb) - std::abs(T[i]) < Min)
			{
				Normal = Vec4(R[i][0],R[i][1],R[i][2]);
				EdgeEdge = false;
				Min = (ra + rb) - std::abs(T[i]);
			}
		}
	}
    
	//B0, B1, B2
	for(int i = 0; i < 3; ++i)
	{
		rb = Two.m_HalfExtents[i];
		ra = One.m_HalfExtents[0] * AbsR[0][i] + One.m_HalfExtents[1] * AbsR[1][i] + One.m_HalfExtents[2] * AbsR[2][i];
		if(std::abs(T[0] * R[0][i] + T[1] * R[1][i] + T[2] * R[2][i]) > ra + rb)
			return false;
		else
		{
			if((ra + rb) - std::abs(T[i]) < Min)
			{
				Normal = Vec4(AbsR[0][i],AbsR[1][i],AbsR[2][i]);
				EdgeEdge = false;
				Min = (ra + rb) - std::abs(T[i]);
			}
		}
	}
    
    // Test axis L = A0 x B0
    ra = One.m_HalfExtents[1] * AbsR[2][0] + One.m_HalfExtents[2] * AbsR[1][0];
    rb = Two.m_HalfExtents[1] * AbsR[0][2] + Two.m_HalfExtents[2] * AbsR[0][1];
    if (std::abs(T[2] * R[1][0] - T[1] * R[2][0]) > ra + rb) return false;
	else
	{
        float value = (ra + rb) -  std::abs(T[2] * R[1][0] - T[1] * R[2][0]);
        


		if(value < Min)
		{
                    if(!EqualF(std::abs(T[2] * R[1][0] - T[1] * R[2][0]) , 0.0f))
                    {
                        Normal = Vec4(R[1][0],0,R[2][0]);
                        EdgeEdge = true;
                        Min = (ra + rb) - std::abs(T[2] * R[1][0] - T[1] * R[2][0]);
                    }
		}
	}
    
    // Test axis L = A0 x B1
    ra = One.m_HalfExtents[1] * AbsR[2][1] + One.m_HalfExtents[2] * AbsR[1][1];
    rb = Two.m_HalfExtents[0] * AbsR[0][2] + Two.m_HalfExtents[2] * AbsR[0][0];
    if (std::abs(T[2] * R[1][1] - T[1] * R[2][1]) > ra + rb) return false;
	else
	{
		if((ra + rb) -  std::abs(T[2] * R[1][1] - T[1] * R[2][1]) < Min)
		{
            if(!EqualF(std::abs(T[2] * R[1][1] - T[1] * R[2][1]) , 0.0f))
            {
                Normal = Vec4(0,R[1][1],R[2][1]);
                EdgeEdge = true;
                Min = (ra + rb) - std::abs(T[2] * R[1][1] - T[1] * R[2][1]);
            }
		}
	}
    
    // Test axis L = A0 x B2
    ra = One.m_HalfExtents[1] * AbsR[2][2] + One.m_HalfExtents[2] * AbsR[1][2];
    rb = Two.m_HalfExtents[0] * AbsR[0][1] + Two.m_HalfExtents[1] * AbsR[0][0];
    if (std::abs(T[2] * R[1][2] - T[1] * R[2][2]) > ra + rb) return false;
	else
	{
		if((ra + rb) -  std::abs(T[2] * R[1][2] - T[1] * R[2][2]) < Min)
		{
            if(!EqualF(std::abs(T[2] * R[1][2] - T[1] * R[2][2]) , 0.0f))
            {
                Normal = Vec4(0,R[1][2],R[2][2]);
                EdgeEdge = true;
                Min = (ra + rb) - std::abs(T[2] * R[1][2] - T[1] * R[2][2]);
            }
		}
	}
    
    // Test axis L = A1 x B0
    ra = One.m_HalfExtents[0] * AbsR[2][0] + One.m_HalfExtents[2] * AbsR[0][0];
    rb = Two.m_HalfExtents[1] * AbsR[1][2] + Two.m_HalfExtents[2] * AbsR[1][1];
    if (std::abs(T[0] * R[2][0] - T[2] * R[0][0]) > ra + rb) return false;
	else
	{
		if((ra + rb) - std::abs(T[0] * R[2][0] - T[2] * R[0][0]) < Min)
		{
            if(!EqualF(std::abs(T[0] * R[2][0] - T[2] * R[0][0]) , 0.0f))
            {
                Normal = Vec4(R[0][0],0,R[2][0]);
                EdgeEdge = true;
                Min = (ra + rb) - std::abs(T[0] * R[2][0] - T[2] * R[0][0]);
            }
		}
	}
    
    // Test axis L = A1 x B1
    ra = One.m_HalfExtents[0] * AbsR[2][1] + One.m_HalfExtents[2] * AbsR[0][1];
    rb = Two.m_HalfExtents[0] * AbsR[1][2] + Two.m_HalfExtents[2] * AbsR[1][0];
    if (std::abs(T[0] * R[2][1] - T[2] * R[0][1]) > ra + rb) return false;
	else
	{
		if((ra + rb) - std::abs(T[0] * R[2][1] - T[2] * R[0][1]) < Min)
		{
            if(!EqualF(std::abs(T[0] * R[2][1] - T[2] * R[0][1]) , 0.0f))
            {
                Normal = Vec4(R[0][1],0,R[2][1]);
                EdgeEdge = true;
                Min = (ra + rb) - std::abs(T[0] * R[2][1] - T[2] * R[0][1]);
            }
		}
	}
    // Test axis L = A1 x B2
    ra = One.m_HalfExtents[0] * AbsR[2][2] + One.m_HalfExtents[2] * AbsR[0][2];
    rb = Two.m_HalfExtents[0] * AbsR[1][1] + Two.m_HalfExtents[1] * AbsR[1][0];
    if (std::abs(T[0] * R[2][2] - T[2] * R[0][2]) > ra + rb) return false;
	else
	{
		if((ra + rb) - std::abs(T[0] * R[2][2] - T[2] * R[0][2]) < Min)
		{
            if(!EqualF(std::abs(T[0] * R[2][2] - T[2] * R[0][2]) , 0.0f))
            {
                Normal = Vec4(R[0][1],0,R[2][2]);
                EdgeEdge = true;
                Min = (ra + rb) - std::abs(T[0] * R[2][2] - T[2] * R[0][2]);
            }
		}
	}
    
    // Test axis L = A2 x B0
    ra = One.m_HalfExtents[0] * AbsR[1][0] + One.m_HalfExtents[1] * AbsR[0][0];
    rb = Two.m_HalfExtents[1] * AbsR[2][2] + Two.m_HalfExtents[2] * AbsR[2][1];
    if (std::abs(T[1] * R[0][0] - T[0] * R[1][0]) > ra + rb) return false;
	else
	{
		if((ra + rb) - std::abs(T[1] * R[0][0] - T[0] * R[1][0]) < Min)
		{
            if(!EqualF(std::abs(T[1] * R[0][0] - T[0] * R[1][0]) , 0.0f))
            {
                Normal = Vec4(R[0][0],R[1][0],0);
                EdgeEdge = true;
                Min = (ra + rb) - std::abs(T[1] * R[0][0] - T[0] * R[1][0]);
            }
		}
	}
    
    // Test axis L = A2 x B1
    ra = One.m_HalfExtents[0] * AbsR[1][1] + One.m_HalfExtents[1] * AbsR[0][1];
    rb = Two.m_HalfExtents[0] * AbsR[2][2] + Two.m_HalfExtents[2] * AbsR[2][0];
    if (std::abs(T[1] * R[0][1] - T[0] * R[1][1]) > ra + rb) return false;
	else
	{
		if((ra + rb) - std::abs(T[1] * R[0][1] - T[0] * R[1][1]) < Min)
		{
            if(!EqualF(std::abs(T[1] * R[0][1] - T[0] * R[1][1]) , 0.0f))
            {
                Normal = Vec4(R[0][1],R[1][1],0);
                EdgeEdge = true;
                Min = (ra + rb) - std::abs(T[1] * R[0][1] - T[0] * R[1][1]);
            }
		}
	}
    
    // Test axis L = A2 x B2
    ra = One.m_HalfExtents[0] * AbsR[1][2] + One.m_HalfExtents[1] * AbsR[0][2];
    rb = Two.m_HalfExtents[0] * AbsR[2][1] + Two.m_HalfExtents[1] * AbsR[2][0];
    if (std::abs(T[1] * R[0][2] - T[0] * R[1][2]) > ra + rb) return false;
	else
	{
		if((ra + rb) - std::abs(T[1] * R[0][2] - T[0] * R[1][2]) < Min)
		{
            if(!EqualF(std::abs(T[1] * R[0][2] - T[0] * R[1][2]) , 0.0f))
               {
			Normal = Vec4(R[0][2],R[1][2],0);
			EdgeEdge = true;
			Min = (ra + rb) - std::abs(T[1] * R[0][2] - T[0] * R[1][2]);
               }
		}
	}
    
    // Since no separating axis found, the OBBs must be intersecting
    
    A = One.m_Axis.QuaternionToMatrix();
	B = Two.m_Axis.QuaternionToMatrix();
    
    std::vector<Vec4> contactPointsOne;
    std::vector<Vec4> contactPointsTwo;
    
    Vec4 obj1Verts[8], obj2Verts[8];
    
    Vec4 extentsOne = One.m_HalfExtents;
    Vec4 extentsTwo = Two.m_HalfExtents;
    
    int sign = -1;
    
    for(int i = 0; i < 8; i += 2)
    {
        if(sign >= 0)
        {
            extentsOne[sign] = -extentsOne[sign];
            extentsTwo[sign] = -extentsTwo[sign];
        }
        
        obj1Verts[i] = extentsOne;
        obj2Verts[i] = extentsTwo;
        
        obj1Verts[i+1] =  -extentsOne;
        obj2Verts[i+1] =  -extentsTwo;
        
        obj1Verts[i] = A * obj1Verts[i];
        obj2Verts[i] = B * obj2Verts[i];
        
        obj1Verts[i+1] = A * obj1Verts[i+1];
        obj2Verts[i+1] = B * obj2Verts[i+1];
        
        
        obj1Verts[i] += One.m_COM;
        obj1Verts[i+1] += One.m_COM;
        
        obj2Verts[i] += Two.m_COM;
        obj2Verts[i+1] += Two.m_COM;
        
        if(sign >= 0)
        {
            extentsOne[sign] = -extentsOne[sign];
            extentsTwo[sign] = -extentsTwo[sign];
        }
        
        sign++;
    }
    
    
    Vec4 maxVec,minVec;
    float max = -FLT_MAX, min = FLT_MAX;
    
    std::cout << "Normal: " << Normal;
    
    for(auto &objectOne : obj1Verts)
    {
        float dot = objectOne.Dot(Normal);
        if(dot > max)
        {
            max = dot;
            maxVec = objectOne;
        }
    }
    
    contactPointsOne.push_back(maxVec);
    
    for(auto &objectOne : obj1Verts)
    {
        if(objectOne == maxVec)
            continue;
        
        if(objectOne.Dot(Normal) > max - COLLISION_EPSILON)
        {
            contactPointsOne.push_back(objectOne);
        }
    }
    
    for(auto &objectTwo : obj2Verts)
    {
        float dot = objectTwo.Dot(Normal);
        
        if(dot < min)
        {
            min = dot;
            minVec = objectTwo;
        }
    }
    
    contactPointsTwo.push_back(minVec);
    
    for(auto &objectTwo : obj2Verts)
    {
        if(objectTwo == minVec)
            continue;
        if(objectTwo.Dot(Normal) < min + COLLISION_EPSILON)
        {
            contactPointsTwo.push_back(objectTwo);
        }
    }
    
    std::cout << "Collison!: \n";

    if(contactPointsOne.size() == 1)
    {
        ElysiumEngine::PhysicsSystem::g_PhysicsSystem->addCollisionPair(ElysiumEngine::ColliderPair(First,Second,Normal,contactPointsOne.front(),Min));
        
        ElysiumEngine::DrawDebugPoint point(contactPointsOne.front(),Vec4(1.0f,0.0f,1.0f));
        ElysiumEngine::MessagingSystem::g_MessagingSystem->broadcastMessage(&point);
    }
    else if(contactPointsTwo.size() == 1)
    {
        ElysiumEngine::PhysicsSystem::g_PhysicsSystem->addCollisionPair(ElysiumEngine::ColliderPair(First,Second,Normal,contactPointsTwo.front(),Min));
        
        ElysiumEngine::DrawDebugPoint point(contactPointsTwo.front(),Vec4(1.0f,0.0f,1.0f));
        ElysiumEngine::MessagingSystem::g_MessagingSystem->broadcastMessage(&point);
    }
    
    for(int i = 0; i < contactPointsOne.size(); ++i)
    {
        Vec4 one1 = contactPointsOne[i];
        Vec4 one2 = contactPointsOne[i+1%contactPointsOne.size()];
        for(int j = 0; j < contactPointsOne.size(); ++j)
        {
            Vec4 two1 = contactPointsTwo[j];
            Vec4 two2 = contactPointsTwo[j+1%contactPointsTwo.size()];
        }
    }
    
    for(Vec4 &vec : contactPointsOne)
    {
        ElysiumEngine::PhysicsSystem::g_PhysicsSystem->addCollisionPair(ElysiumEngine::ColliderPair(First,Second,Normal,vec,-Min));
        ElysiumEngine::DrawDebugPoint point(vec,Vec4(1.0f,0.0f,1.0f));
        ElysiumEngine::MessagingSystem::g_MessagingSystem->broadcastMessage(&point);
    }
	return true;
}

void ElysiumEngine::narrowPhase(ElysiumEngine::ICollider *one, ElysiumEngine::ICollider *two)
{
    if(one->getType() == E_SPHERE && two->getType() == E_SPHERE)
    {
        SphereCollider *sphereOne = dynamic_cast<SphereCollider *>(one);
        SphereCollider *sphereTwo = dynamic_cast<SphereCollider *>(two);
        
	 	if(sphereSphere(sphereOne,sphereTwo))
	 	{
			Vec4 collisionNormal = (sphereTwo->GetPosition() - sphereOne->GetPosition());
			collisionNormal.Normalize();
			Vec4 collisionPoint = sphereOne->GetPosition() + collisionNormal * sphereOne->getRadius();
            
            float penetrationDepth = (sphereOne->GetPosition()-sphereTwo->GetPosition()).Length()-(sphereOne->getRadius()+sphereTwo->getRadius());
                                                                                                   
            PhysicsSystem::g_PhysicsSystem->addCollisionPair(ColliderPair(sphereOne,sphereTwo,collisionNormal,collisionPoint,penetrationDepth));
	 	}
    }
    else if((one->getType() == E_SPHERE && two->getType() == E_BOX) || (one->getType() == E_BOX && two->getType() == E_SPHERE))
    {
        if(two->getType() == E_SPHERE)
            std::swap(one,two);
        
        SphereCollider *sphere = dynamic_cast<SphereCollider *>(one);
        OBBCollider *obb = dynamic_cast<OBBCollider *>(two);
        
        Vec4 closestPoint;
        
        if(sphereBox(sphere, obb,closestPoint))
        {
            Vec4 collisonNormal = closestPoint - sphere->GetPosition();
            collisonNormal.Normalize();
            Vec4 collisionPoint = sphere->getRadius() * collisonNormal + sphere->GetPosition();
            float penetration = -(closestPoint - collisionPoint).Length();
            
            PhysicsSystem::g_PhysicsSystem->addCollisionPair(ColliderPair(sphere,obb,collisonNormal,collisionPoint,penetration));
        }
    }
    else if(one->getType() == E_BOX && two->getType() == E_BOX)
    {
        OBBCollider *boxOne = dynamic_cast<OBBCollider *>(one);
        OBBCollider *boxTwo = dynamic_cast<OBBCollider *>(two);
        
        Vec4 contactPoint;
        Vec4 collisionNormal;
        
        if(OBBOBB(boxOne, boxTwo, contactPoint, collisionNormal))
        {
            //PhysicsSystem::g_PhysicsSystem->addCollisionPair(ColliderPair(boxOne,boxTwo,collisionNormal,contactPoint,0));
        }
    }
    else if((one->getType() == E_SPHERE && two->getType() == E_PLANE) || (one->getType() == E_PLANE && two->getType() == E_SPHERE))
    {
        if(two->getType() == E_SPHERE)
            std::swap(one,two);
        
        SphereCollider *sphere = dynamic_cast<SphereCollider *>(one);
        PlaneCollider *plane = dynamic_cast<PlaneCollider *>(two);
        
        Vec4 contactPoint;
        Vec4 collisionNormal;
        float penetration = 0.0f;
        
        if(spherePlane(sphere, plane, collisionNormal, contactPoint,penetration))
        {
            PhysicsSystem::g_PhysicsSystem->addCollisionPair(ColliderPair(sphere,plane,-collisionNormal,contactPoint,penetration));
        }
    }
    else if((one->getType() == E_BOX && two->getType() == E_PLANE) || (one->getType() == E_PLANE && two->getType() == E_BOX))
    {
        if(two->getType() == E_BOX)
            std::swap(one,two);
        
        OBBCollider *sphere = dynamic_cast<OBBCollider *>(one);
        PlaneCollider *plane = dynamic_cast<PlaneCollider *>(two);
        
        
        if(obbPlane(sphere, plane))
        {

        }
    }
    else
    {
        std::deque<Vec4> m_Simplex;
        if(GJK(one,two,m_Simplex))
        {
            Vec4 contactPoint;
            Vec4 normal;
            float penetration = 0.0f;
            EPA(m_Simplex,contactPoint,normal,penetration);
            PhysicsSystem::g_PhysicsSystem->addCollisionPair(ColliderPair(one,two,normal,contactPoint,penetration));
        }
    }
}

bool rayToSphere(ElysiumEngine::SphereCollider *collider, Vec4 &dir, Vec4 &origin)
{
    return false;
}

bool rayToOBB(ElysiumEngine::OBBCollider *collider, Vec4 &dir, Vec4 &origin)
{
    return false;
}

bool ElysiumEngine::intersectRay(ElysiumEngine::ICollider *collider, Vec4 dir, Vec4 origin)
{
    if(collider->getType() == E_SPHERE)
    {
        SphereCollider *sphere = dynamic_cast<SphereCollider *>(collider);
        return rayToSphere(sphere, dir, origin);
    }
    else if(collider->getType() == E_BOX)
    {
        OBBCollider *box = dynamic_cast<OBBCollider *>(collider);
        return rayToOBB(box, dir, origin);
    }
    else
    {
        return false;
    }
}
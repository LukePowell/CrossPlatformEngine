
#include "Collider.h"
#include "PhysicsSystem.h"
#include "RigidBody.h"
#include "Transform.h"
#include "Messaging.h"
#include "GraphicsSystem/Mesh.h"
#include "GraphicsSystem/MeshRenderable.h"
#include "GraphicsSystem/GraphicsSystem.h"

#include <algorithm>
#include <cmath>

void ElysiumEngine::ICollider::initialize()
{
	VInitialize();
}

void ElysiumEngine::ICollider::lateInitialize()
{
	m_Trans = getSibling<Transform>("Transform");
    
	if(hasSibling("RigidBody"))
	{
		RigidBody *Body = getSibling<RigidBody>("RigidBody");
		m_Static = Body->getStatic();
	}
    
	VLateInitialize();
}

ElysiumEngine::ICollider::~ICollider()
{
    PhysicsSystem::g_PhysicsSystem->removeCollider(this);
}

ElysiumEngine::ICollider::ICollider(std::string name) : IComponent(name)
{
    m_Static = false;
    PhysicsSystem::g_PhysicsSystem->addCollider(this);
}

ElysiumEngine::ICollider::ICollider( std::string name,const ICollider &rhs ) : IComponent(name)
{
	m_Type = rhs.m_Type;
	m_Static = false;
    PhysicsSystem::g_PhysicsSystem->addCollider(this);
}

void ElysiumEngine::SphereCollider::VLateInitialize()
{
	Vec4 &Scale = m_Trans->GetScale();
	//In the cause of a non-uniform scale choose the largest value
	m_RadiusScaled = m_Radius * std::max(std::max(Scale.x,Scale.y),Scale.z);
}

Vec4 ElysiumEngine::SphereCollider::SupportMapping( Vec4 direction )
{
	return Maths::SupportMappingSphere(direction,m_Trans->GetPosition(),m_RadiusScaled);
}

ElysiumEngine::SphereCollider::SphereCollider( const SphereCollider &rhs ) :ICollider("SphereCollider",rhs)
{
	m_Radius = rhs.m_Radius;
	m_RadiusScaled = rhs.m_RadiusScaled;
}

void ElysiumEngine::SphereCollider::serialize( FileIO::IReader &reader )
{
	reader.GetFloatValueAtr("radius",&m_Radius);
}

void ElysiumEngine::SphereCollider::deserialize()
{

}

float ElysiumEngine::SphereCollider::getRadius()
{
	return m_RadiusScaled;
}

Vec4 &ElysiumEngine::ICollider::GetPosition()
{
	return m_Trans->GetPosition();
}

void ElysiumEngine::MeshCollider::serialize( FileIO::IReader &reader )
{

}

void ElysiumEngine::MeshCollider::deserialize()
{

}

Vec4 ElysiumEngine::MeshCollider::SupportMapping( Vec4 direction )
{
	Matrix World = BuildScaleMatrix(m_Trans->GetScale().x,m_Trans->GetScale().y,m_Trans->GetScale().z) * m_Trans->GetOrientation().QuaternionToMatrix();
	direction = World.Inverse() * direction;

	//Message::DBox Msg(m_Vertices[0].x,m_Vertices[0].y,m_Vertices[0].z,0.1f,0.1f,0.1f,0.0f,0.0f,1.0f);
	//Message::g_MessageHandler->BroadcastMessage(&Msg);

	float max = direction.Dot(m_Vertices[0]);
	Vec4 vec = m_Vertices[0];
	for(int i = 1; i < m_Vertices.size(); ++i)
	{
		//Message::DBox Msg(m_Vertices[i].x,m_Vertices[i].y,m_Vertices[i].z,0.1f,0.1f,0.1f,0.0f,0.0f,1.0f);
		//Message::g_MessageHandler->BroadcastMessage(&Msg);

		if(direction.Dot(m_Vertices[i]) > max)
		{
			max = direction.Dot(m_Vertices[i]);
			vec = m_Vertices[i];
		}
	}
	World = BuildTranslationMatrix(m_Trans->GetPosition()) * BuildScaleMatrix(m_Trans->GetScale().x,m_Trans->GetScale().y,m_Trans->GetScale().z) * m_Trans->GetOrientation().QuaternionToMatrix();
	vec.w = 1.0f;
	vec = World * vec;
	vec.w = 0.0f;
	return vec;
}

void ElysiumEngine::MeshCollider::VLateInitialize()
{
    Mesh *mesh = getSibling<MeshRenderable>("MeshRenderable")->getMesh();
    
    for(int i = 0; i < mesh->vertexCount; ++i)
    {
        this->m_Vertices.push_back(mesh->vertices[i].position);
    }
	//Message::RequestMesh msg(this);
	//Message::g_MessageHandler->BroadcastMessage(&msg);
}

void ElysiumEngine::MeshCollider::VInitialize()
{

}

/*void ElysiumEngine::MeshCollider::HandleMessage(Message::SendMesh &message )
{
	float *data = static_cast<float *>(message.m_Vertices);
	unsigned int j = 0 ;
	unsigned int NumFloats = message.m_BytesPerVertex / sizeof(float);

	for(int i = 0; i < message.m_NumVerts; ++i, j+=NumFloats)
	{
		Vec4 Temp;
		Temp.x = data[j];
		Temp.y = data[j+1];
		Temp.z = data[j+2];
		if(std::find(m_Vertices.begin(),m_Vertices.end(),Temp) == m_Vertices.end())
			m_Vertices.push_back(Temp);
	}
}*/

ElysiumEngine::OBBCollider::OBBCollider( OBBCollider *rhs ) : ICollider("OBBCollider")
{

}

Vec4 ElysiumEngine::OBBCollider::SupportMapping( Vec4 direction )
{
	return Vec4();
}

void ElysiumEngine::OBBCollider::VLateInitialize()
{
	//m_Obb.m_Axis = {Vec4(1.0f,0.0f,0.0f),Vec4(0.0f,1.0f,0.0f},Vec4(0.0f,0.0f,1.0f)};
		m_Obb.m_HalfExtents = m_Trans->GetScale() * 0.5f;
}

void ElysiumEngine::OBBCollider::serialize( FileIO::IReader &reader )
{

}

void ElysiumEngine::OBBCollider::deserialize()
{

}

Vec4 ElysiumEngine::OBBCollider::GetDimensions()
{
	return m_Trans->GetScale();
}

Maths::OBB ElysiumEngine::OBBCollider::GetOBB()
{
	m_Obb.m_COM = m_Trans->GetPosition();
	m_Obb.m_Axis  = m_Trans->GetOrientation();
	m_Obb.m_HalfExtents = m_Trans->GetScale() * 0.5f;
	return m_Obb;
}

ElysiumEngine::ColliderPair::ColliderPair( ICollider *one, ICollider *two, Vec4 normal, Vec4 point, float penetrationDistance) : m_One(one), m_Two(two), m_CollisionNormal(normal), m_CollisionPoint(point), penetrationDistance(penetrationDistance)
{
}

Vec4 ElysiumEngine::PlaneCollider::SupportMapping(Vec4 direction)
{
    return Vec4();
}

void ElysiumEngine::PlaneCollider::serialize(FileIO::IReader &reader)
{
    reader.GetFloatValueAtr("offset", &offset);
    reader.GetFirstChild();
    while(reader.ReadNextChild())
    {
        if(reader.GetTag() == "normal")
        {
            reader.GetFloatValueAtr("x", &normal.x);
            reader.GetFloatValueAtr("y", &normal.y);
            reader.GetFloatValueAtr("z", &normal.z);
        }
    }
    reader.GetParent();
}

void ElysiumEngine::PlaneCollider::deserialize()
{
    
}




#include "RigidBody.h"
#include <assert.h>
#include "Forces.h"
#include "PhysicsSystem.h"
#include "GraphicsSystem/MeshRenderable.h"
#include "GraphicsSystem/GraphicsSystem.h"

#define ToString(x) #x

void Subexpressions(float &w0, float &w1, float &w2, float &f1, float &f2, float &f3, float &g0, float &g1, float &g2)
{
    float temp0 = w0 + w1;
    f1 = temp0 + w2;
    float temp1 = w0 * w0;
    float temp2 = temp1+w1*temp0;
    f2 = temp2+w2*f1;
    f3 = w0 * temp1+w1*temp2+w2*f2;
    g0 = f2 + w0 * (f1 + w0);
    g1 = f2 + w1 * (f1 + w1);
    g2 = f2 + w2 * (f1 + w2);
}

static const float mult[10] = {1.0f/6.0f,1.0f/24.0f,1.0f/24.0f,1.0f/24.0f,1.0f/60.0f,1.0f/60.0f,1.0f/60.0f,1.0f/120.0f,1.0f/120.0f,1.0f/120.0f};

void compute(Vec4 p[], int tMax, int index[],float &mass, Vec4 &com, Matrix &tensor)
{
    float intg[10] = {0,0,0,0,0,0,0,0,0,0};
    
    for(int t = 0; t < tMax; ++t)
    {
        int i0 = index[3*t], i1 = index[3 * t + 1], i2 = index[3 * t + 2];
        
        float x0 = p[i0].x, y0 = p[i0].y, z0 = p[i0].z;
        float x1 = p[i1].x, y1 = p[i1].y, z1 = p[i1].z;
        float x2 = p[i2].x, y2 = p[i2].y, z2 = p[i2].z;
        
        float a1 = x1-x0, b1 = y1-y0, c1 = z1-z0, a2 = x2-x0, b2 = y2 -y0, c2 = z2-z0;
        float d0 = b1*c2-b2*c1,d1 = a2 * c1 - a1 * c2, d2 = a1 * b2 - a2 * b1;
        
        float f1x,f1y,f1z;
        float f2x,f2y,f2z;
        float f3x,f3y,f3z;
        
        float g0x,g0y,g0z;
        float g1x,g1y,g1z;
        float g2x,g2y,g2z;
        
        Subexpressions(x0, x1, x2, f1x, f2x, f3x, g0x, g1x, g2x);
        Subexpressions(y0, y1, y2, f1y, f2y, f3y, g0y, g1y, g2y);
        Subexpressions(z0, z1, z2, f1z, f2z, f3z, g0z, g1z, g2z);
        
        intg[0] += d0 * f1x;
        intg[1] += d0 * f2x;
        intg[2] += d1 * f2y;
        
        intg[3] += d2 * f2z;
        intg[4] += d0 * f3x;
        intg[5] += d1 * f3y;
        
        intg[6] += d2 * f3z;
        intg[7] += d0 * (y0 * g0x + y1 * g1x + y2 * g2x);
        intg[8] += d1 * (z0 * g0y + z1 * g1y + z2 * g2y);
        intg[9] += d2 * (x0 * g0z + x1 * g1z + x2 * g2z);
    }
    
    for(int i = 0; i < 10; ++i)
    {
        intg[i] *= mult[i];
    }
    
    mass = intg[0];
    
    com.x = intg[1] / mass;
    com.y = intg[2] / mass;
    com.z = intg[3] / mass;
    
    //Inertia tensor
    
    tensor[0][0] = intg[5] + intg[6] - mass * (com.y * com.y + com.z * com.z);
    tensor[1][1] = intg[4] + intg[6] - mass * (com.x * com.x + com.y * com.y);
    tensor[2][2] = intg[4] + intg[5] - mass * (com.x * com.x + com.y * com.y);
    
    tensor[0][1] = tensor[1][0] = -(intg[7] - mass * com.x * com.y);
    tensor[1][2] = tensor[2][1] = -(intg[8] - mass * com.y * com.z);
    tensor[2][0] = tensor[0][2] = -(intg[9] - mass * com.z * com.x);
    
}

namespace ElysiumEngine
{
    static std::string EnumStrings[E_TOTAL] = {ToString(E_BOX),ToString(E_SPHERE),ToString(E_MESH)};
}

ElysiumEngine::RigidBody::RigidBody() : IComponent("RigidBody"), isStatic(false), ignorePause(false)
{
    m_GravMag = 0.0f;
    m_Gravity = true;
    density = 1.0f;
    asleep = false;
    //this->m_AngularVelocity.x = 1.0f;'
    //m_AngularVelocity.y = 0.1f;
}

ElysiumEngine::RigidBody::RigidBody(const RigidBody *body) : IComponent("RigidBody")
{
    m_GravMag = body->m_GravMag;
    m_Gravity = body->m_Gravity;
    density = body->density;
    asleep = false;
    ignorePause = body->ignorePause;
    isStatic = body->isStatic;
    m_Type = E_MESH;
}

ElysiumEngine::RigidBody::~RigidBody()
{
    PhysicsSystem::g_PhysicsSystem->removeRigidBody(this);
}

void ElysiumEngine::RigidBody::serialize(FileIO::IReader &reader)
{
    reader.GetBoolValueAttr("ignorePause", &ignorePause);
	reader.GetFirstChild();
	while(reader.ReadNextChild())
	{
        if(reader.GetTag() == "static")
        {
            reader.GetBoolValueAttr("value",&isStatic);
        }
        else if(reader.GetTag() == "gravity")
        {
            reader.GetBoolValueAttr("bool", &m_Gravity);
            reader.GetFloatValueAtr("mag", &m_GravMag);
        }
        else if(reader.GetTag() == "tensor")
        {
            std::string Type = EnumStrings[E_MESH];
			reader.GetStringValueAtr("type",&Type);
			for(int i = 0; i < E_TOTAL; ++i)
			{
				if(EnumStrings[i] == Type)
				{
					m_Type = i;
					break;
				}
			}
        }
        else if(reader.GetTag() == "mass")
        {
            //Calculate the inverse mass
            reader.GetFloatValueAtr("mass", &m_Mass);
            m_InverseMass = 1.0f / m_Mass;
        }
        else if(reader.GetTag() == "velocity")
        {
            //Set the starting velocity of a rigid body
            reader.GetFloatValueAtr("x", &m_Velocity.x);
            reader.GetFloatValueAtr("y", &m_Velocity.y);
            reader.GetFloatValueAtr("z", &m_Velocity.z);
        }
        else if(reader.GetTag() == "density")
        {
            reader.GetFloatValueAtr("value", &density);
        }
	}
	reader.GetParent();
}

void ElysiumEngine::RigidBody::initialize()
{
    PhysicsSystem::g_PhysicsSystem->addRigidBody(this);
}
void ElysiumEngine::RigidBody::lateInitialize()
{
    if(hasSibling("Transform"))
    {
        transform = getSibling<Transform>("Transform");
    }
    else
    {
        assert(0 && "Rigid Body relies on Transform");
    }
    
    if(m_Gravity)
    {
        accumulator.AddForce(new Gravity(Vec4(0.0f,m_GravMag,0.0f),m_Mass));
    }
    
    switch(m_Type)
	{
        case E_SPHERE:
		{
			if(!hasSibling("SphereCollider"))
				return;
			SphereCollider *Collider = getSibling<SphereCollider>("SphereCollider");
			m_IntertiaTensor[0][0] = m_IntertiaTensor[1][1] = m_IntertiaTensor[2][2] = Collider->getRadius() * Collider->getRadius();
			m_IntertiaTensor *= (2.0f / 5.0f)  * m_Mass;
			m_IntertiaTensor[3][3] = 1.0f;
		}
            break;
        case E_BOX:
		{
			if(!hasSibling("OBBCollider"))
				return;
			Vec4 Scale = transform->GetScale();
			m_IntertiaTensor[0][0] = Scale.y * Scale.y + Scale.z * Scale.z;
			m_IntertiaTensor[1][1] = Scale.x * Scale.x + Scale.z * Scale.z;
			m_IntertiaTensor[2][2] = Scale.x * Scale.x + Scale.y * Scale.y;
			m_IntertiaTensor *= 1.0f / 12.0f * m_Mass;
			m_IntertiaTensor[3][3] = 1.0f;
			break;
		}
        case E_MESH:
            break;
	}
	m_InverseTensor = m_IntertiaTensor.Inverse();
    
    if(hasSibling("MeshRenderable"))
    {
        MeshRenderable *renderable = getSibling<MeshRenderable>("MeshRenderable");
        Mesh *mesh = renderable->getMesh();
        Matrix tensor;
        tensor.Identity();
        this->m_IntertiaTensor.Identity();
        
        Vec4 *verts = new Vec4[mesh->vertexCount];
        int *indices = new int[mesh->indexCount * 3];
        
        for(int i = 0; i < mesh->vertexCount; ++i)
        {
            verts[i] = mesh->vertices[i].position;
        }
        
        for(int i = 0; i < mesh->indexCount * 3; ++i)
        {
            indices[i] = mesh->indices[i];
        }
        
        compute(verts, mesh->indexCount , indices, m_Mass, com, this->m_IntertiaTensor);
        
        m_Mass *= density;
        m_InverseMass = 1.0f / m_Mass;

        m_IntertiaTensor *= density;
        
        m_InverseTensor = m_IntertiaTensor.Inverse();
        
        std::cout << "\nMass: " << m_Mass << std::endl;
        delete []verts;
        delete []indices;
    }
}

void ElysiumEngine::RigidBody::updateAcceleration()
{
    m_Acceleration = accumulator.Calculate() * m_InverseMass;
}

void ElysiumEngine::RigidBody::integratePosition(float dt)
{
    if(!isStatic)
    {
        transform->GetPosition() += dt * m_Velocity;
        transform->GetOrientation() += 0.5f * dt * transform->GetOrientation() * Maths::Quaternion(m_AngularVelocity.x,m_AngularVelocity.y,m_AngularVelocity.z,0.0f);
		transform->GetOrientation().Normalize();
    }
}

void ElysiumEngine::RigidBody::integrateVelocity(float dt)
{
    if(!isStatic)
    {
        m_Velocity += dt * m_Acceleration;
    }
}

void ElysiumEngine::RigidBody::updateAndIntegrate(float dt)
{
    updateAcceleration();
    integrateVelocity(dt);
}

void ElysiumEngine::RigidBody::addForce(ElysiumEngine::IForce *force)
{
    accumulator.AddForce(force);
}

void ElysiumEngine::RigidBody::removeForce(ElysiumEngine::IForce *force)
{
    accumulator.RemoveForce(force);
}

Vec4 ElysiumEngine::RigidBody::GetPosition()
{
    return transform->GetPosition();
}


void ElysiumEngine::RigidBody::setVelocity(Vec4 &vel)
{
    m_Velocity = vel;
}

void ElysiumEngine::RigidBody::setVelocity(float x, float y, float z)
{
    m_Velocity = Vec4(x,y,z);
}

Vec4& ElysiumEngine::RigidBody::getVelocity()
{
    return m_Velocity;
}

float ElysiumEngine::RigidBody::getMass()
{
    return m_Mass;
}

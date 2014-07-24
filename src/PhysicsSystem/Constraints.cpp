#include "Constraint.h"
#include "RigidBody.h"

Vec4 * ElysiumEngine::IJacobian::Get()
{
	return &m_Elements[0];
}

void ElysiumEngine::DistanceJacobian::Calculate()
{
	Vec4 D = (m_PointTwo+m_One->GetPosition() - m_PointOne + m_One->GetPosition());
	m_Elements[0] = -D;
	m_Elements[1] = -((m_PointOne-m_One->GetPosition()).Cross(D));
	m_Elements[2] = D;
	m_Elements[3] = -((m_PointTwo-m_Two->GetPosition()).Cross(D));
}

ElysiumEngine::DistanceJacobian::DistanceJacobian( RigidBody *one, RigidBody *two, Vec4 pointONe, Vec4 pointTwo ) : m_One(one), m_Two(two), m_PointOne(pointONe), m_PointTwo(pointTwo)
{
}


ElysiumEngine::IConstraint::IConstraint(RigidBody *one, RigidBody *two, IJacobian *jOne, IJacobian *jTwo, int lifetime ) : m_Jacobian(jOne), m_One(one), m_Two(two), m_LifeTime(lifetime)
{
    lambda = 0.0f;
    contact = false;
}

ElysiumEngine::ContactJacobian::ContactJacobian( Vec4 normal, Vec4 r1, Vec4 r2, Vec4 x1, Vec4 x2, float penetrationDepth) : m_Normal(normal), m_R1(r1), m_R2(r2), m_X1(x1), m_X2(x2), penetrationDepth(penetrationDepth)
{

}

float Beta = 6.5f;
float slop = -0.01f;
float ElysiumEngine::ContactJacobian::GetBias()
{
    if(penetrationDepth > slop)
        return 0.0f;

	float ret = Beta * (penetrationDepth - slop);
    
    return ret;
}

void ElysiumEngine::ContactJacobian::Calculate()
{
	m_Elements[0] = -m_Normal;
	m_Elements[1] = -m_R1.Cross(m_Normal);
	m_Elements[2] = m_Normal;
	m_Elements[3] = m_R2.Cross(m_Normal);
}

ElysiumEngine::FrictionJacobian::FrictionJacobian(IConstraint *contact, RigidBody *one, RigidBody *two, Vec4 contactPoint, Vec4 normal, Vec4 dir) : one(one), two(two), contact(contact), contactPoint(contactPoint), normal(normal), dir(dir)
{
    r1 = this->contactPoint - this->one->GetPosition();
    r2 = this->contactPoint - this->two->GetPosition();
}


void ElysiumEngine::FrictionJacobian::Calculate()
{
    m_Elements[0] = -dir;
    m_Elements[1] = -(r1.Cross(dir));
    m_Elements[2] = dir;
    m_Elements[3] = r2.Cross(dir);
}

static float mu = 0.5f;
float ElysiumEngine::FrictionJacobian::GetMin()
{

    float fn = (contact->m_Jacobian->Get()[0] * contact->lambda).Length();
    return -mu * fn;
}

float ElysiumEngine::FrictionJacobian::GetMax()
{
    float fn = (contact->m_Jacobian->Get()[0] * contact->lambda).Length();
    return mu * fn;
}

float ElysiumEngine::FrictionJacobian::GetBias()
{
    return 0.0f;
}
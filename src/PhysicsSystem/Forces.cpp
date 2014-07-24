#include "Forces.h"
#include <algorithm>

void ElysiumEngine::ForceAccumulator::AddForce(IForce *force)
{
	m_Forces.push_back(force);
}

void ElysiumEngine::ForceAccumulator::RemoveForce( IForce *force )
{
	std::remove(m_Forces.begin(),m_Forces.end(),force);
}

void ElysiumEngine::ForceAccumulator::ClearForces()
{
	std::for_each(m_Forces.begin(),m_Forces.end(),[](IForce *force){delete force;});
	m_Forces.clear();
}

ElysiumEngine::ForceAccumulator::~ForceAccumulator()
{
	ClearForces();
}

Vec4 ElysiumEngine::ForceAccumulator::Calculate()
{
    for(auto force : m_Forces)
    {
        m_AccumlatedForce += force->GetForce();
    }
	return m_AccumlatedForce;
}

ElysiumEngine::Gravity::Gravity(Vec4 magnitude, float mass ) : m_Mass(mass), m_Magnitude(magnitude)
{

}

Vec4 ElysiumEngine::Gravity::GetForce()
{
	return -m_Magnitude * m_Mass;
}

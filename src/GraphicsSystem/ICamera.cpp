#include "ICamera.h"
#include "GraphicsSystem.h"


ElysiumEngine::ICamera::ICamera( std::string name ) : IComponent(name)
{
    GraphicsSystem::g_GraphicsSystem->addCamera(this);
}

ElysiumEngine::ICamera::~ICamera()
{
	GraphicsSystem::g_GraphicsSystem->removeCamera(this);
}


const Vec4& ElysiumEngine::FreeCamera::getEye()
{
	if(m_Transform)
		return m_Transform->GetPosition();
    else
        return m_Eye;
}

const Vec4& ElysiumEngine::FreeCamera::getLookAt()
{
	if(m_Transform)
		m_LookAt = m_Transform->GetPosition() + m_Direction;
    else
        m_LookAt.Zero();
    
	return m_LookAt;
}

void ElysiumEngine::FreeCamera::lateInitialize()
{
	if(hasSibling("Transform"))
		m_Transform = getSibling<Transform>("Transform");
}

void ElysiumEngine::FreeCamera::deserialize()
{

}

void ElysiumEngine::FreeCamera::serialize( FileIO::IReader &reader )
{
	reader.GetFirstChild();
	while(reader.ReadNextChild())
	{
		if(reader.GetTag() == "dir")
		{
			reader.GetFloatValueAtr("x",&m_Direction.x);
			reader.GetFloatValueAtr("y",&m_Direction.y);
			reader.GetFloatValueAtr("z",&m_Direction.z);
		}
	}
	reader.GetParent();
}

void ElysiumEngine::FreeCamera::setDirection( Vec4 &direction )
{
	m_Direction = direction;
}
